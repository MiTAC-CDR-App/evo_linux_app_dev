/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2021 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/mp4/Mp4Context.h                                                                    *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/23/21 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  major supporting class for MP4 handling.                                                    *
 *                Because the utility functions may be different a lot for different products/applications,   *
 *                Those functions are not defined in this class.                                              *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_MP4_MP4_CONTEX_H
#define _SUPPORT_MP4_MP4_CONTEX_H

// libBase includes
#include <container/List.h>
#include <support/mp4/Mp4Atoms.h>

#define MP4_CONTEXT_ALLOW_DATA_TRACK_DURATION_0   0x00000001

// 1. Currently, only support MP4 below 2GB.
// 2. For simplicity, no synchronization protection.
class Mp4Context
{
  public:
    Mp4RootAtom *rootAtom = 0;
    int flags = 0;

    ~Mp4Context();

    // 1. Read file and build atom tree, however, the atom tree may not be fully expanded
    // 2. If readWrite is false, MP4 is opened with read-only mode.
    static Mp4Context *openMp4(const char *path, bool readWrite = false, bool silent = false);
    static Mp4Context *createMp4(const char *path);

    int fileSize();
    void close();

    Mp4Atom *locateMDatAtom();
    Mp4MoovAtom *locateMoovAtom();
    Mp4TrakAtom *locateTrackAtom(int trackType);
    Mp4TrakAtom *locateVideoTrackAtom();
    Mp4TrakAtom *locateAudioTrackAtom();
    Mp4TrakAtom *locateMDataTrackAtom();

    int moveChunk(int from, int to, int size);
    // 1. This is usually called after moov move/update (in the view of file operation).
    // 2. Last children of rootAtom is checked of the new file size, if it is smaller, ftruncate() is called.
    int repackFileSize();

    int copyFrom(Mp4Context *src, int from, int totalBytes);
    int copyFrom(Mp4Context *src, int from, int totalBytes, int to);
    int seek(int offset);
    int read(void *buf, int size);
    int readFrom(int from, void *buf, int size);
    int write(void *buf, int size);

  private:
    int fd = -1;
    int _fileSize = 0;

    Mp4Context(int fd, int fileSize);

    friend class Mp4Atom;
};

#endif//_SUPPORT_MP4_MP4_CONTEX_H
