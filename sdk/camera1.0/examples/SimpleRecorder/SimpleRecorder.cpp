/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  SimpleRecorder                                                                              *
 * BINARY NAME :  SimpleRecorder                                                                              *
 * FILE NAME   :  SimpleRecorder.cpp                                                                          *
 * CREATED BY  :  Forest Yeh <forest.yeh@mic.com.tw>                                                          *
 * CREATED DATE:  3/5/24 (MM/DD/YY)                                                                           *
 * DESCRIPTION :  Example code of using camera SDK to record video                                            *
 *------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "FfmpegMuxer.h"
#include "mitacCameraSdk.h"

static const float CAMERA_FPS = 30;
static const float BITRATE = 6; // Mbps

static int cameraID = CAM_ID_FRONT;
static int recordingDuration = 60; // Seconds
static uint8_t *spsData = NULL;
static size_t spsDataSize = 0;
static uint8_t *ppsData = NULL;
static size_t ppsDataSize = 0;
static bool initialized = false;
static FfmpegMuxer *muxer;
static uint64_t startTimestampInMS;
static timeval streamStartTime;

static bool processing = true;

static void onNewEncodedData(int _cameraID, int dataType, uint64_t timestampInMS, void *data, int size, void* userData)
{
    if (!processing)
    {
        return;
    }

    if (_cameraID != cameraID)
    {
        printf("Receive encoded data of camera ID %d\n", _cameraID);
        return;
    }

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
            uint64_t timestampInUS = streamStartTime.tv_sec * 1000000 + streamStartTime.tv_usec;
            timestampInUS += (timestampInMS * 1000);
            timeval startTime;
            startTime.tv_sec = timestampInUS / 1000000;
            startTime.tv_usec = timestampInUS - (startTime.tv_sec * 1000000);
            if (muxer->openFile("/data/test/test.mp4", startTime))
            {
                if (muxer->initVideoStream(extraData, extraDataSize, startTime))
                {
                    initialized = true;
                    startTimestampInMS = timestampInMS;
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
    if ((timestampInMS - startTimestampInMS) >= (recordingDuration * 1000))
    {
        processing = false;
        muxer->closeFile();
        return;
    }

    // Handle frame
    if ((dataType == DATA_TYPE_I_FRAME) || (dataType == DATA_TYPE_P_FRAME))
    {
        uint64_t timestampInUS = streamStartTime.tv_sec * 1000000 + streamStartTime.tv_usec;
        timestampInUS += (timestampInMS * 1000);
        timeval timestamp;
        timestamp.tv_sec = timestampInUS / 1000000;
        timestamp.tv_usec = timestampInUS - (timestamp.tv_sec * 1000000);
        muxer->writeVideoFrame(data, size, timestamp, (dataType == DATA_TYPE_I_FRAME));
    }
}

int main(int argc, char *argv[])
{
    for (size_t i = 0; i < argc; i++)
    {
        if (i == 1)
        {
            sscanf(argv[i], "%d", &cameraID);
        }
        else if (i == 2)
        {
            sscanf(argv[i], "%d", &recordingDuration);
        }
    }
    if ((cameraID < 0) || (cameraID > 2))
    {
        printf("Camera ID is out of range! Use default camera ID.\n");
        cameraID = CAM_ID_FRONT;
    }
    if ((recordingDuration < 1) || (recordingDuration > 60))
    {
        printf("Recording duration is out of range! Use default Recording duration.\n");
        recordingDuration = 60;
    }
    printf("Recording with Camera ID: %d,  Recording duration: %d\n", cameraID, recordingDuration);

    int result = mitacCamConfigCamera(cameraID, RESOLUTION_1080P, CAMERA_FPS);
    if (result < 0)
    {
        printf("Failed to config camera!\n");
        processing = false;
        return 0;
    }

    result = mitacCamConfigMainStream(cameraID, RESOLUTION_1080P, BITRATE, CAMERA_FPS, -1, onNewEncodedData, NULL, false);
    if (result < 0)
    {
        printf("Failed to config main stream!\n");
        processing = false;
        return 0;
    }

    gettimeofday(&streamStartTime, NULL);
    muxer = new FfmpegMuxer();
    result = mitacCamStartOperation(cameraID);
    if (result < 0)
    {
        printf("Failed to start main stream!\n");
        processing = false;
    }

    while (true)
    {
        if (!processing)
        {
            break;
        }
        sleep(1);
    }

    result = mitacCamStopOperation(cameraID);
    if (result < 0)
    {
        printf("Failed to stop operation!\n");
    }
    delete spsData;
    delete ppsData;
    delete muxer;
    return 0;
}
