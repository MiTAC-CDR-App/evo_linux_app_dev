/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2023 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Recording                                                                                   *
 * BINARY NAME :  Recording                                                                                   *
 * FILE NAME   :  FfmpegMuxer.h                                                                               *
 * CREATED BY  :  Forest Yeh <forest.yeh@mic.com.tw>                                                          *
 * CREATED DATE:  12/28/23 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  A Muxer entity class implemented with ffmpeg libraries.                                     *
 *                All the interfaces are not thread-safe, make sure invoking them in the same thread,         *
 *                or in a thread-safe way.                                                                    *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef FFMPEGMUXER_H_
#define FFMPEGMUXER_H_

#include <stdint.h>
#include <string>
#include <sys/time.h>

struct AVFormatContext;
struct AVStream;

class FfmpegMuxer
{
    public:
        FfmpegMuxer();
        virtual ~FfmpegMuxer();

        bool openFile(std::string filepath, timeval startTime);
        bool closeFile();
        bool initVideoStream(uint8_t *extradata, size_t extradataSize, timeval timestamp);
        bool writeVideoFrame(void *data, size_t size, timeval timestamp, bool isKeyFrame);
        bool isFileOpened();

    private:
        struct OutputStream
        {
            AVStream *st;
            timeval lastTimestamp;
            int64_t lastPts;
        };

        void closeFileImpl();
        void setFileStartTime(timeval timestamp);
        bool addStream(OutputStream *stream, int codecId, const void *extradata, size_t extradataSize);
        bool postAddingStreams();
        bool couldWriteVideoFrame();
        bool writeFrame(OutputStream &stream, void *data, size_t size, timeval timestamp, uint32_t timeScale, bool keyFrame);
        void cleanup();

        AVFormatContext *context;
        OutputStream videoStream;
        std::string filepath;
};

#endif /* FFMPEGMUXER_H_ */
