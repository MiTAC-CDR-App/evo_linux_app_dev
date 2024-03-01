/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Recording                                                                                   *
 * BINARY NAME :  Recording                                                                                   *
 * FILE NAME   :  Recording.cpp                                                                               *
 * CREATED BY  :  Forest Yeh <forest.yeh@mic.com.tw>                                                          *
 * CREATED DATE:  02/22/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Recording example using Gstreamer                                                           *
 *------------------------------------------------------------------------------------------------------------*/

#include <gst/gst.h>
#include <string.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#include "FfmpegMuxer.h"

#define DATA_TYPE_SPS       0x67
#define DATA_TYPE_PPS       0x68
#define DATA_TYPE_I_FRAME   0x65
#define DATA_TYPE_P_FRAME   0x41

static struct timeval videoStartTime;
static int frameCount = 0;
static bool initialized = false;
static bool processing = true;

static FfmpegMuxer *muxer;
static uint8_t *spsData = NULL;
static size_t spsDataSize = 0;
static uint8_t *ppsData = NULL;
static size_t ppsDataSize = 0;

static void handleEncodedData(int dataType, void *data, int size, timeval timestamp)
{
    // Handle SPS data
    if (dataType == DATA_TYPE_SPS)
    {
        if (spsDataSize <= 0)
        {
            spsDataSize = size;
            spsData = new uint8_t[spsDataSize];
            memcpy(spsData, data, spsDataSize);
        }
    }
    // Handle PPS data
    else if (dataType == DATA_TYPE_PPS)
    {
        if (ppsDataSize <= 0)
        {
            ppsDataSize = size;
            ppsData = new uint8_t[ppsDataSize];
            memcpy(ppsData, data, ppsDataSize);
        }
    }
    // Check initialize
    if (!initialized)
    {
        if ((spsDataSize > 0) && (ppsDataSize > 0))
        {
            size_t extraDataSize = spsDataSize + ppsDataSize;
            uint8_t *extraData = new uint8_t[extraDataSize];
            memcpy(extraData, spsData, spsDataSize);
            memcpy(&extraData[spsDataSize], ppsData, ppsDataSize);
            if (muxer->openFile("/data/test/test.mp4", timestamp))
            {
                if (muxer->initVideoStream(extraData, extraDataSize, timestamp))
                {
                    initialized = true;
                }
                else
                {
                    muxer->closeFile();
                }
            }
            delete extraData;
        }
        else
        {
            printf("Initialized data is not ready, wait next data.\n");
        }
        return;
    }
    // Check recorded duration
    if ((timestamp.tv_sec - videoStartTime.tv_sec) >= 30)
    {
        processing = false;
        muxer->closeFile();
        return;
    }

    // Handle frame
    if ((dataType == DATA_TYPE_I_FRAME) || (dataType == DATA_TYPE_P_FRAME))
    {
        muxer->writeVideoFrame(data, size, timestamp, (dataType == DATA_TYPE_I_FRAME));
    }
}

static int findNalUnitType(guint8* data, int size)
{
    const int headSize = 4;
    const guint8 delimiter = 0x09;
    // check type
    if (data[headSize] == DATA_TYPE_SPS)
    {
        return DATA_TYPE_SPS;
    }
    else if (data[headSize] == DATA_TYPE_PPS)
    {
        return DATA_TYPE_PPS;
    }
    else if (data[headSize] == DATA_TYPE_I_FRAME)
    {
        return DATA_TYPE_I_FRAME;
    }
    else if (data[headSize] == DATA_TYPE_P_FRAME)
    {
        return DATA_TYPE_P_FRAME;
    }
    else if (data[headSize] == delimiter)
    {
    }
    else
    {
    }
    return -1;
}

static int findNalTag(const guint8 *data, size_t dataSize, uint32_t nalTag)
{
    for (size_t i = 0; i < dataSize - 3; i++)
    {
        uint32_t target = * ((uint32_t *) (data + i));
        if (target == nalTag)
        {
            // Pattern found at index i
            return i;
        }
    }
    // Pattern not found
    return -1;
}

static void processBufferData(guint8* data, int size, timeval timestamp)
{
    const guint8 tag[] = { 0x00, 0x00, 0x00, 0x01 };
    const int tagSize = 4;
    uint32_t nalTag = * ((uint32_t *) tag);

    guint8 *unitData = 0;
    int unitSize = 0;
    int unitType = -1;
    int currOffset = 0;
    int remainedSize = size;
    int tagIndex = -1;

    while (true)
    {
        tagIndex = findNalTag(data + currOffset, remainedSize, nalTag);
        if (tagIndex != -1) // found head
        {
            if (unitSize > 0) // has previous found head
            {
                // adjust unitSize, process previous unit data
                unitSize = tagIndex + tagSize;
                unitType = findNalUnitType(unitData, unitSize);
                if (unitType != -1)
                {
                    handleEncodedData(unitType, (void *) unitData, unitSize, timestamp);
                }
            }
            unitData = data + currOffset + tagIndex; // save pointer
            unitSize = remainedSize - tagIndex; // save tmp size
            // prepare to find next head
            currOffset = currOffset + tagIndex + tagSize; // move to after the found head
            remainedSize = size - currOffset;
        }
        else // no (more) head found
        {
            if (unitSize <= 0) // no previous found head
            {
                break;
            }

            // adjust unitSize, process unit data
            unitSize = remainedSize + tagSize;
            unitType = findNalUnitType(unitData, unitSize);
            handleEncodedData(unitType, (void *) unitData, unitSize, timestamp);
            break;
        }
    }
}

// The appsink has received a buffer
static GstFlowReturn new_sample(GstElement *sink)
{
    if (!processing)
    {
        return GST_FLOW_CUSTOM_SUCCESS;
    }

    GstSample *sample;
    /* Retrieve the buffer */
    g_signal_emit_by_name(sink, "pull-sample", &sample);
    if (sample)
    {
        GstBuffer *buffer = gst_sample_get_buffer(sample);
        int size = gst_buffer_get_size(buffer);
        long pts = GST_BUFFER_PTS(buffer);
        GstMapInfo map;
        gst_buffer_map(buffer, &map, GST_MAP_READ);
        timeval timestamp;
        timestamp.tv_sec = videoStartTime.tv_sec + (buffer->pts / 1000000000);
        timestamp.tv_usec = videoStartTime.tv_usec +
            ((buffer->pts / 1000) - ((buffer->pts / 1000000000) * 1000000));
        processBufferData(map.data, size, timestamp);
        gst_buffer_unmap(buffer, &map);
        gst_sample_unref(sample);
        return GST_FLOW_OK;
    }
    return GST_FLOW_ERROR;
}

int main(int argc, char *argv[])
{
    // initialize GStreamer
    gst_init(NULL, NULL);

    // create pipeline
    printf("Build pipeline.\n");
    GstElement *pipeline = gst_pipeline_new("pipeline");
    if (!pipeline)
    {
        printf("Failed to create pipeline object.\n");
        return -1;
    }

    GstElement *qmmfSrc = gst_element_factory_make("qtiqmmfsrc", "qmmfSrc");
    g_object_set(qmmfSrc, "camera", 0, NULL);
    gst_bin_add(GST_BIN(pipeline), qmmfSrc);

    char capsString[256];
    memset((void *) capsString, 0, 256);
    sprintf(capsString, "video/x-raw(memory:GBM),width=%d,height=%d,framerate=%d/%d", 1920, 1080, 30, 1);
    GstCaps *caps = gst_caps_from_string(capsString);
    if (!caps)
    {
        printf("Caps for camera cannot be created.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    GstElement *capsSettings = gst_element_factory_make("capsfilter", "capsSettings");
    g_object_set(capsSettings, "caps", caps, NULL);
    gst_bin_add(GST_BIN(pipeline), capsSettings);
    gst_caps_unref(caps);

    // config omxh264enc
    GstElement *h264enc = gst_element_factory_make("omxh264enc", "h264enc");
    g_object_set(h264enc, "target-bitrate", 6000000, "interval-intraframes", 29, "periodicity-idr", 1, NULL);
    gst_bin_add(GST_BIN(pipeline), h264enc);

    GstElement *h264parse = gst_element_factory_make("h264parse", "h264parse");
    g_object_set(h264parse, "config-interval", -1, NULL);
    gst_bin_add(GST_BIN(pipeline), h264parse);

    GstElement *appSink = gst_element_factory_make("appsink", "appSink");
    g_object_set(appSink, "sync", FALSE, "drop", FALSE, "max-buffers", 1, "emit-signals", TRUE, NULL);
    g_signal_connect(appSink, "new-sample", G_CALLBACK(new_sample), NULL);
    gst_bin_add(GST_BIN(pipeline), appSink);

    if (gst_element_link_many(qmmfSrc, capsSettings, h264enc, h264parse, appSink, NULL) != TRUE)
    {
        printf("Link main stream elements failed.\n");
        gst_object_unref(pipeline);
        return -1;
    }

    gettimeofday(&videoStartTime, NULL);
    muxer = new FfmpegMuxer();

    // start playing the pineline
    printf("set to PLAYING state.\n");
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    while (processing)
    {
        sleep(1);
    }

    printf("set to NULL state.\n");
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    delete muxer;

    return 0;
}
