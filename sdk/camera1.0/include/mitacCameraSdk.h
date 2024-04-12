/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2023 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  EVO Linux Camera SDK                                                                        *
 * BINARY NAME :  mitacCameraSdk.so                                                                           *
 * FILE NAME   :  mitacCameraSdk.h                                                                            *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  12/13/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Header file for MiTAC Camera SDK.                                                           *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _MITAC_CAMERA_SDK_H
#define _MITAC_CAMERA_SDK_H

// Standard include
#include <stdint.h>
#include <string>

#define MITAC_ERR_ILLEGAL_PARAMETERS            -4

#define CAM_ID_FRONT        0
#define CAM_ID_INCABIN      1

// 1920 x 1080.
#define RESOLUTION_1080P    0
// 1280 x 720.
#define RESOLUTION_720p     1
// 854 x 480.
#define RESOLUTION_480p     2
// 480 x 360.
#define RESOLUTION_360p     3
// 640 x 480.
#define RESOLUTION_VGA     10

#define DATA_TYPE_SPS       0x67
#define DATA_TYPE_PPS       0x68
#define DATA_TYPE_I_FRAME   0x65
#define DATA_TYPE_P_FRAME   0x41

typedef void (*FuncOnNewEncodedData)(int cameraID, int dataType, uint64_t timestampInMS, void *data, int size,
                                                                                               void *userData);
typedef void (*FuncOnNewRawData)(int cameraID, uint64_t timestampInMS, void *data, int size, void *userData);
typedef void (*FuncLogCallback)(const char *logStr);

std::string mitacCamGetSdkVer(void);

// 1. Only support RESOLUTION_1080P/RESOLUTION_720p.  If RESOLUTION_720p is used, 1080p cannot be used for main/
//    sub/AI stream.
// 2. Typically, only support the following fps (TBD): 15, 15.5, 27.5, 30.  The SDK will choose the closed one.
//    If 15/15.5 is used, 27.5/30 cannot be used for main/sub/AI stream.
// 3. This API should be called before any main/sub/AI stream's configuration.
int mitacCamConfigCamera(int cameraID, int resolution, float fps);

// 1. For main stream, it is used for recording, only support RESOLUTION_1080P/RESOLUTION_720p.
// 2. Typically, only support the following fps (TBD): 15, 15.5, 27.5, 30.  The SDK will choose the closed one.
// 3. For gop, positive value means the real gop value (after rounded to integer).  If the value is negative,
//    it means how many seconds to generate 1 I-frame, if the negative value is closed to integer, the SDK will
//    try to lock I-frame to system seconds.  Default is -1 (1 second 1 I-frame, and lock to system seconds).
//    And please notice that, implementation will try to satisfy the specified gop, however, the I-frame
//    generation may vary a little, it fully depend on H264 encode mechanism.
// 4. onNewEncodedData is the callback to receive SPS/PPS and encoded samples.
// 5. userData will be used when callback onNewEncodedData().
// 6. If byUserThread = true, onNewEncodedData() will be called inside mitacCamDispatchMainStreamLoop().
int mitacCamConfigMainStream(int cameraID, int resolution, float bitrateInMbps, float fps, float gop,
                             FuncOnNewEncodedData onNewEncodedData, void *userData, bool byUserThread = false);

// 1. dataFormat: TBD
// 2. For fps, it is constrained to fps of camera, which is set by mitacCamConfigCamera().
// 3. userData will be used when callback FuncOnNewRawData().
// 4. If byUserThread = true, onNewEncodedData() will be called inside mitacCamDispatchAiStreamLoop().
int mitacCamConfigAiStream(int cameraID, int resolution, int *dataFormat, float fps,
                                     FuncOnNewRawData onNewRawData, void *userData, bool byUserThread = false);

// 1. For sub stream, it is used for live view, only support RESOLUTION_720p and below.
// 2. Maximum fps is 15.5.  The SDK will choose the proper one.
// 3. For gop, only allow positive value,
// 4. onNewEncodedData is the callback to receive SPS/PPS and encoded samples.
// 5. userData will be used when callback onNewEncodedData().
// 6. If byUserThread = true, onNewEncodedData() will be called inside mitacCamDispatchSubStreamLoop().
int mitacCamConfigSubStream(int cameraID, int resolution, float bitrateInMbps, float fps, int gop,
                            FuncOnNewEncodedData onNewEncodedData, void *userData, bool byUserThread = false);

// 1. mitacCamConfigCamera() should be called.  Any main/sub/AI stream's configuration should be done.
// 2. Once started, cannot further configure main/sub/AI stream.
int mitacCamStartOperation(int cameraID);
// All configurations for specified camera will be reset.
int mitacCamStopOperation(int cameraID);

/*
 * The following three loop functions are used with "byUserThread = true" when configuring related stream.  The
 * major purpose is for Java integration, and the user thread is a Java thread, so that onNewEncodedData()/
 * onNewRawData() can call Java methods easily.
 */
// 1. It is used with mitacCamConfigMainStream() with "byUserThread = true".
// 2. It should call immediately after successful mitacCamStartOperation().
// 3. It will return once camera stopped.
// 4. All main stream's onNewEncodedData() of the specified camera will be callbacked inside.
void mitacCamDispatchMainStreamLoop(int cameraID);
// 1. It is used with mitacCamConfigAiStream() with "byUserThread = true".
// 2. It should call immediately after successful mitacCamStartOperation().
// 3. It will return once camera stopped.
// 4. All AI stream's onNewRawData() of the specified camera will be callbacked inside.
void mitacCamDispatchAiStreamLoop(int cameraID);
// 1. It is used with mitacCamConfigSubStream() with "byUserThread = true".
// 2. It should call immediately after successful mitacCamStartOperation().
// 3. It will return once camera stopped.
// 4. All sub-stream's onNewEncodedData() of the specified camera will be callbacked inside.
void mitacCamDispatchSubStreamLoop(int cameraID);

void mitacCamSetLogCallback(FuncLogCallback logCallback);
// Default off.
void mitacCamEnableDebugLog(bool enabled);

#endif//_MITAC_CAMERA_SDK_H
