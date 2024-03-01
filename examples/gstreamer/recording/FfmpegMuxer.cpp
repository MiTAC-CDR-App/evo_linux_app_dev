/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2023 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Recording                                                                                   *
 * BINARY NAME :  Recording                                                                                   *
 * FILE NAME   :  FfmpegMuxer.cpp                                                                             *
 * CREATED BY  :  Forest Yeh <forest.yeh@mic.com.tw>                                                          *
 * CREATED DATE:  12/28/23 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  A Muxer entity class implemented with ffmpeg libraries.                                     *
 *                All the interfaces are not thread-safe, make sure invoking them in the same thread,         *
 *                or in a thread-safe way.                                                                    *
 *------------------------------------------------------------------------------------------------------------*/

extern "C"
{
#include <libavformat/avformat.h>
#include <libavutil/pixdesc.h>
}

#include "FfmpegMuxer.h"

#define STREAM_SAMPLE_FMT AV_SAMPLE_FMT_S16P // audio sample format
#define STREAM_PIX_FMT AV_PIX_FMT_YUV420P // video pixel format
#define VIDEO_TIMESCALE 90000

#define MAX_DURATION_IN_SEC 60

FfmpegMuxer::FfmpegMuxer()
{
}

FfmpegMuxer::~FfmpegMuxer()
{
    if (isFileOpened())
    {
        closeFile();
    }
}

static int getMonotonicTime(timeval &time);
static int64_t time2us(const timeval &time);

bool FfmpegMuxer::openFile(std::string _filepath, timeval startTime)
{
    if (_filepath.empty())
    {
        printf("Cannot open file with empty path\n");
        return false;
    }

    int ret = 0;
    filepath = _filepath;
    if ((ret = avformat_alloc_output_context2(&context, NULL, "mp4", filepath.c_str())) < 0)
    {
        printf("Failed to allocate output context: (%d)\n", ret);
        return false;
    }

    context->oformat->video_codec = AV_CODEC_ID_H264;
    context->oformat->audio_codec = AV_CODEC_ID_NONE;
    if ((ret = avio_open(&context->pb, filepath.c_str(), AVIO_FLAG_WRITE)) < 0)
    {
        printf("Failed to open file: %s (%d)\n", filepath.c_str(), ret);
        avformat_free_context(context);
        context = NULL;
        return false;
    }
    printf("File opened: %s\n", filepath.c_str());
    return true;
}

bool FfmpegMuxer::closeFile()
{
    if (!context)
    {
        printf("Null format context for: %s\n", filepath.c_str());
        return true;
    }

    if (!context->pb)
    {
        printf("Null I/O context for: %s\n", filepath.c_str());
        return true;
    }

    // Flush internal buffer before closing file
    int ret;
    if ((ret = av_interleaved_write_frame(context, NULL)) < 0)
    {
        printf("Failed to flush internal buffer: (%d)\n", ret);
    }

    printf("Writing trailer for: %s\n", filepath.c_str());
    av_write_trailer(context);
    printf("Finished writing trailer for: %s\n", filepath.c_str());

    closeFileImpl();
    cleanup();

    printf("File closed: %s\n", filepath.c_str());
    return true;
}

bool FfmpegMuxer::initVideoStream(uint8_t *extradata, size_t extradataSize, timeval timestamp)
{
    bool success = false;
    if (addStream(&videoStream, context->oformat->video_codec, extradata, extradataSize) &&
        postAddingStreams())
    {
        setFileStartTime(timestamp);
        success = true;
    }
    if (!success)
    {
        closeFileImpl();
        cleanup();
    }
    return success;
}

bool FfmpegMuxer::writeVideoFrame(void *data, size_t size, timeval timestamp, bool isKeyFrame)
{
    if (!isFileOpened())
    {
        return false;
    }

    if (!couldWriteVideoFrame())
    {
        return false;
    }

    return writeFrame(videoStream, data, size, timestamp, VIDEO_TIMESCALE, isKeyFrame);
}

bool FfmpegMuxer::isFileOpened()
{
    if (context && context->pb)
    {
        return true;
    }

    return false;
}

void FfmpegMuxer::closeFileImpl()
{
    if (!context)
    {
        return;
    }

    if (context->pb)
    {
        avio_closep(&context->pb);
    }
    avformat_free_context(context);
}

void FfmpegMuxer::setFileStartTime(timeval timestamp)
{
    timerclear(&videoStream.lastTimestamp);
    if (timerisset(&timestamp))
    {
        videoStream.lastTimestamp = timestamp;
    }
    else
    {
        timeval time;
        getMonotonicTime(time);
        videoStream.lastTimestamp = time;
    }
}

bool FfmpegMuxer::addStream(OutputStream *stream, int codecId, const void *extradata, size_t extradataSize)
{
    stream->st = avformat_new_stream(context, NULL);
    if (!stream->st)
    {
        printf("Could not alloc stream\n");
        return false;
    }

    stream->st->id = context->nb_streams - 1;
    AVMediaType type = avcodec_get_type((AVCodecID) codecId);
    AVCodecParameters *params = stream->st->codecpar;
    switch (type)
    {
        case AVMEDIA_TYPE_VIDEO:
        {
            bool parametersFilled = false;
            // Try to fill parameters by codec encoder/decoder
            do
            {
                AVCodecContext *avctx = NULL;
                AVCodec *avcodec = NULL;
                avcodec = avcodec_find_decoder((AVCodecID)codecId);
                if (!avcodec)
                {
                    printf("Could not find decoder for %s, try to find encoder instead.\n",
                           avcodec_get_name((AVCodecID)codecId));
                    avcodec = avcodec_find_encoder((AVCodecID)codecId);
                }
                if (!avcodec)
                {
                    printf("Could not find either video encoder or decoder for %s\n",
                           avcodec_get_name((AVCodecID)codecId));
                    break;
                }

                avctx = avcodec_alloc_context3(avcodec);
                if (!avctx)
                {
                    printf("Could not allocate video codec context\n");
                    break;
                }
                avctx->time_base = (AVRational) { 1, VIDEO_TIMESCALE };
                avctx->codec_tag = 0;
                avctx->pix_fmt = STREAM_PIX_FMT;
                avctx->width = 1920;
                avctx->height = 1080;
                avctx->profile = FF_PROFILE_H264_BASELINE;
                if (extradata && (extradataSize > 0))
                {
                    avctx->extradata_size = extradataSize;
                    avctx->extradata = static_cast<uint8_t *>(av_mallocz(extradataSize +
                                                              AV_INPUT_BUFFER_PADDING_SIZE));
                    memcpy(avctx->extradata, extradata, extradataSize);
                }
                int ret;
                if ((ret = avcodec_open2(avctx, avcodec, NULL)) < 0)
                {
                    printf("Could not open codec: (%d)\n", ret);
                    break;
                }

                avcodec_parameters_from_context(params, avctx);
                parametersFilled = true;
                if (avctx)
                {
                    avcodec_free_context(&avctx);
                }
            } while (0);

            // Fill parameters manually if failed to do it by codec encoder/decoder
            if (!parametersFilled)
            {
                stream->st->time_base = (AVRational) { 1, VIDEO_TIMESCALE };
                params->codec_tag = 0;
                params->format = STREAM_PIX_FMT;
                params->width = 1920;
                params->height = 1080;
                params->profile = FF_PROFILE_H264_BASELINE;
                if (extradata && extradataSize > 0)
                {
                    params->extradata_size = extradataSize;
                    params->extradata = static_cast<uint8_t *>(av_mallocz(extradataSize +
                                                               AV_INPUT_BUFFER_PADDING_SIZE));
                    memcpy(params->extradata, extradata, extradataSize);
                }
            }
            break;
        }
        default:
            break;
    }
    return true;
}

bool FfmpegMuxer::postAddingStreams()
{
    // Write header
    int ret = 0;
    if ((ret = avformat_write_header(context, NULL)) < 0)
    {
        printf("Failed to write header: (%d)\n", ret);
        return false;
    }

    return true;
}

bool FfmpegMuxer::couldWriteVideoFrame()
{
    if (!context)
    {
        printf("Null format context\n");
        return false;
    }

    if (!context->pb)
    {
        printf("Null I/O context\n");
        return false;
    }

    if (!videoStream.st)
    {
        printf("Null video stream\n");
        return false;
    }

    return true;
}

bool FfmpegMuxer::writeFrame(OutputStream &stream, void *data, size_t size, timeval timestamp,
                             uint32_t timeScale, bool keyFrame)
{
    AVPacket packet;
    memset(&packet, 0, sizeof(packet));
    av_init_packet(&packet);
    packet.dts = AV_NOPTS_VALUE;
    packet.pts = AV_NOPTS_VALUE;
    if (keyFrame)
    {
        packet.flags |= AV_PKT_FLAG_KEY;
    }

    packet.stream_index = stream.st->index;
    packet.data = static_cast<uint8_t *>(data);
    packet.size = size;
    uint64_t duration = 1;
    if (timerisset(&stream.lastTimestamp))
    {
        if (!timercmp(&timestamp, &stream.lastTimestamp, <))
        {
            timeval dTime;
            timersub(&timestamp, &stream.lastTimestamp, &dTime);
            int64_t dUs = time2us(dTime);
            duration = av_rescale(dUs, timeScale, AV_TIME_BASE);
            if (duration >= timeScale * MAX_DURATION_IN_SEC)
            {
                printf("duration: %lu >= max value: %d, set duration to 1\n", duration,
                       (timeScale * MAX_DURATION_IN_SEC));
                duration = 1;
            }
        }
    }
    else
    {
        printf("lastTimestamp has not been set, let duration = 1\n");
    }

    stream.lastPts += duration;
    packet.pts = stream.lastPts;
    packet.dts = stream.lastPts;
    stream.lastTimestamp = timestamp;
    int ret = av_interleaved_write_frame(context, &packet);
    return ret == 0;
}

void FfmpegMuxer::cleanup()
{
    context = NULL;
    videoStream.st = NULL;
    videoStream.lastPts = 0;
    timerclear(&videoStream.lastTimestamp);
}

static int getMonotonicTime(timeval &time)
{
  time.tv_sec = 0;
  time.tv_usec = 0;

  timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 0;
  int ret = clock_gettime(CLOCK_MONOTONIC, &ts);
  if (ret == 0)
  {
    TIMESPEC_TO_TIMEVAL(&time, &ts);
  }

  return ret;
}

static int64_t time2us(const timeval &time)
{
  return static_cast<int64_t>(time.tv_sec) * 1000000 + time.tv_usec;
}
