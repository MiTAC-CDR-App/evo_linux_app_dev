/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2022 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/mp4/Mp4Util.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  11/24/22 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  General MP4 utility.                                                                        *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_MP4_MP4_UTIL_H
#define _SUPPORT_MP4_MP4_UTIL_H

// Standard include
#include <string>

namespace base
{

class Mp4Util
{
  public:
    // 1. ndx is 0-based.
    // 2. If no data track available, or any error, empty string is returned.
    static std::string getDataStr(const char *path, int ndx);
};

}

#endif//_SUPPORT_MP4_MP4_UTIL_H
