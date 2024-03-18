/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2024 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/mp4/Mp4Muxer.h                                                                      *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  01/31/24 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Interface for MP4 muxers.                                                                   *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_MP4_MP4_MUXER_H
#define _SUPPORT_MP4_MP4_MUXER_H

// Standard include
#include <stdint.h>
// libBase include
#include <basicType/RefCountObj.h>

#define TRACK_ID_VIDEO      0
#define TRACK_ID_AUDIO      1
#define TRACK_ID_DATA       2

#define KEY_SAMPLE          0x0001
#define START_SAMPLE        0x0002
#define END_SAMPLE          0x0004

class Mp4Muxer : public RefCountObj
{
  public:
    virtual int prepare() = 0;
    virtual int addSample(int trackID, void *data, int size, uint64_t timestamp, int flags) = 0;
    virtual int close() = 0;

  protected:
    Mp4Muxer();
    virtual ~Mp4Muxer();

  private:
    // Private copy constructor is declared but not defined to prevent accident copy.
    Mp4Muxer(const Mp4Muxer &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    Mp4Muxer &operator=(const Mp4Muxer &);
};

#endif//_SUPPORT_MP4_MP4_MUXER_H
