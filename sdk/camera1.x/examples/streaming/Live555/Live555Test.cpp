/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Live555Test                                                                                 *
 * BINARY NAME :  Live555Test                                                                                 *
 * FILE NAME   :  Live555Test.cpp                                                                             *
 * CREATED BY  :  Sony Li <sony.li@mic.com.tw>                                                                *
 * CREATED DATE:  5/13/24 (MM/DD/YY)                                                                          *
 * DESCRIPTION :  Example code of using camera SDK and live555 libary for rtsp stream                         *
 *------------------------------------------------------------------------------------------------------------*/
#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include "mitacCameraSdk.h"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#define _PATH_NAME_ "./test.264"

static const float CAMERA_FPS = 10;
static const float BITRATE = 1; // Mbps

static UsageEnvironment* env;
static Boolean reuseFirstSource = False;
static int cameraID = CAM_ID_FRONT;
static int live555_fd = 0;

static void onMainStreamData(int _cameraID, int dataType, uint64_t timestampInMS, void *data, int size, void* userData)
{
}

static void onSubStreamData(int _cameraID, int dataType, uint64_t timestampInMS, void *data, int size, void* userData)
{
    int len = write(live555_fd, data, size);
}

void* live555_start(void *arg)
{
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    env = BasicUsageEnvironment::createNew(*scheduler);
    RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, NULL);
    if (rtspServer == NULL)
    {
        *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
        return nullptr;
    }
    OutPacketBuffer::maxSize = 300000;
    char const* descriptionString = "Session streamed by \"live555Test\"";
    char const* streamName = "live555Test";
    char const* inputFileName = _PATH_NAME_;
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, inputFileName, reuseFirstSource));
    rtspServer->addServerMediaSession(sms);

    char const* httpProtocolStr = "HTTP";
    if (rtspServer->setUpTunnelingOverHTTP(80) || rtspServer->setUpTunnelingOverHTTP(8000) || rtspServer->setUpTunnelingOverHTTP(8080))
    {
        *env << "\n(We use port " << rtspServer->httpServerPortNum() << " for optional RTSP-over-" << httpProtocolStr << " tunneling.)\n";
    }
    else
    {
        *env << "\n(RTSP-over-" << httpProtocolStr << " tunneling is not available.)\n";
    }
    env->taskScheduler().doEventLoop();
    return nullptr;
}

void* stream_start(void *arg)
{
    live555_fd = open(_PATH_NAME_, O_WRONLY);
    if (live555_fd == 0)
    {
        printf("open fifo error\n");
        return nullptr;
    }

    int result = mitacCamConfigCamera(cameraID, RESOLUTION_720P, CAMERA_FPS);
    if (result < 0)
    {
        printf("Failed to config camera!\n");
        return nullptr;
    }
    result = mitacCamConfigMainStream(cameraID, RESOLUTION_720P, BITRATE, CAMERA_FPS, -1, onMainStreamData, NULL, false);
    if (result < 0)
    {
        printf("Failed to config main stream!\n");
        return nullptr;
    }
    result = mitacCamConfigSubStream(cameraID, RESOLUTION_VGA, BITRATE, CAMERA_FPS, CAMERA_FPS, onSubStreamData, NULL, false);
    if (result < 0)
    {
        printf("Failed to config sub stream!\n");
        return nullptr;
    }
    result = mitacCamStartOperation(cameraID);
    if (result < 0)
    {
        printf("Failed to start main stream!\n");
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (access(_PATH_NAME_, F_OK) == -1)
    {
        int ret = mkfifo(_PATH_NAME_, S_IFIFO | 0666);
        if (ret == -1) {
            printf("make fifo error\n");
            return 1;
        }
    }

    pthread_t tid;
    pthread_create(&tid, nullptr, live555_start, nullptr);
    pthread_t tid1;
    pthread_create(&tid1, nullptr, stream_start, nullptr);

    for (;;)
    {
        std::string cmd;
        std::cin >> cmd;
        if(cmd == "end")
        {
            break;
        }
    }

    printf("Stop operation!\n");
    int result = mitacCamStopOperation(cameraID);
    if (result < 0)
    {
        printf("Failed to stop operation!\n");
    }
    pthread_cancel(tid);
    pthread_cancel(tid1);
    pthread_join(tid, nullptr);
    pthread_join(tid1, nullptr);
    return 0;
}
