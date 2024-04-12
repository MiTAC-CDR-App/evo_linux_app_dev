/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Camera SDK Example Support                                                                  *
 * BINARY NAME :  FpsTest                                                                                  *
 * FILE NAME   :  FpsTest.cpp                                                                              *
 * CREATED BY  :  Forest Yeh <forest.yeh@mic.com.tw>                                                          *
 * CREATED DATE:  03/11/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Example code of measuring FPS                                                               *
 *------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#include "mitacCameraSdk.h"

#define MAIN_STREAM 0
#define AI_STREAM   1
#define SUB_STREAM  2

static const float CAMERA_FPS = 30;
static const float BITRATE = 6; // Mbps
static int cameraID = CAM_ID_FRONT;

static const int totalSlot = 10; // seconds to accumulate frame counts, one slot per second.
struct FpsMeasureData
{
    int totalFrames[totalSlot];
    int lastSlot = -1;
    int slotHasBeenUsed = 0;
};
static struct FpsMeasureData measureData[3]; // 3 streams

// call this method when a frame is received.
// Note: this method only applied for fps > 1.
static void measureFPS(int streamId)
{
    struct timespec curTime;
    clock_gettime(CLOCK_MONOTONIC, &curTime);
    int curSlot = curTime.tv_sec % totalSlot; // get current slot
    if (measureData[streamId].lastSlot == -1) // the first time calling this method for this stream id
    {
        measureData[streamId].slotHasBeenUsed = 1;
        measureData[streamId].totalFrames[curSlot] = 1;
        measureData[streamId].lastSlot = curSlot;
    }
    else if (curSlot == measureData[streamId].lastSlot) // current slot is not changed
    {
        measureData[streamId].totalFrames[curSlot]++; // increase count of current slot
    }
    else // current slot is changed
    {
        if (measureData[streamId].slotHasBeenUsed >= totalSlot) // counts of past <totalSlot> seconds are available.
        {
            int totalFrame = 0;
            for (int slot = 0; slot < totalSlot; slot++)
            {
                totalFrame += measureData[streamId].totalFrames[slot];
            }
            float fps = totalFrame / (float) totalSlot;
            printf("%s stream FPS: %.2f\n", (streamId == 0)? "Main" : "Ai", fps);
        }
        else // counts of past <totalSlot> seconds are not yet available.
        {
            measureData[streamId].slotHasBeenUsed++;
        }
        measureData[streamId].totalFrames[curSlot] = 1;  // overwrite count in current slot
        measureData[streamId].lastSlot = curSlot;
    }
}

static void onNewEncodedData(int _cameraID, int dataType, uint64_t timestampInMS, void *data, int size, void* userData)
{
    if ((dataType == DATA_TYPE_I_FRAME) || (dataType == DATA_TYPE_P_FRAME))
    {
        measureFPS(MAIN_STREAM);
    }
}

static void onNewAiData(int cameraID, uint64_t timestampInMS, void *data, int size, void* userData)
{
    measureFPS(AI_STREAM);
}

static int startFpsMeasure()
{
    int result = mitacCamConfigCamera(cameraID, RESOLUTION_1080P, CAMERA_FPS);
    if (result < 0)
    {
        printf("Failed to config camera!\n");
        return -1;
    }

    result = mitacCamConfigMainStream(cameraID, RESOLUTION_1080P, BITRATE, CAMERA_FPS, -1, onNewEncodedData, NULL, false);
    if (result < 0)
    {
        printf("Failed to config main stream!\n");
        return -1;
    }

    int format = 0;
    result = mitacCamConfigAiStream(cameraID, RESOLUTION_1080P, &format, CAMERA_FPS, onNewAiData, NULL, false);
    if (result < 0)
    {
        printf("Failed to config main stream!\n");
        return -1;
    }

    result = mitacCamStartOperation(cameraID);
    if (result < 0)
    {
        printf("Failed to start camera streaming!\n");
        return -1;
    }

    printf("Camera streaming started successfully.\n");
    return 0;
}

int main(int argc, char *argv[])
{
    for (int streamId = 0; streamId < 3; streamId++)
    {
        measureData[streamId].lastSlot = -1;
        measureData[streamId].slotHasBeenUsed = 0;
        for (int slot = 0; slot < totalSlot; slot++)
        {
            measureData[streamId].totalFrames[slot] = 0;
        }
    }
    puts("Please key in (with enter) the following commands to");
    puts("  \"end\": End this test.");
    if (startFpsMeasure() < 0)
    {
        printf("Failed to start FPS measure\n");
        return -1;
    }

    for (;;)
    {
        std::string cmd;
        std::cin >> cmd;
        if(cmd == "end")
        {
            break;
        }
    }

    printf("Stop camera streaming.\n");
    mitacCamStopOperation(cameraID);
    return 0;
}
