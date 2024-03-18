/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  util/SystemUtil.h                                                                           *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/08/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  System utility functions.                                                                   *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _UTIL_SYSTEM_UTIL_H
#define _UTIL_SYSTEM_UTIL_H

// Standard incldue
#include <string>

class SystemUtil
{
  public:
    static int system(const char *cmd);
    static int systemWithResult(const char *cmd, std::string &resultHolder);
};

#endif//_UTIL_SYSTEM_UTIL_H
