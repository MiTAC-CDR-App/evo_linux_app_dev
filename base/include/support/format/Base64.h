/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/format/Base64.h                                                                     *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/21/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Base64 utility class.                                                                       *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_FORMAT_BASE64_H
#define _SUPPORT_FORMAT_BASE64_H

// Standard include
#include <string>

class Base64
{
  public:
    static std::string generateEncodedData(void *data, int len);
    // 1. Clients should free() (not delete[]) the returned data.
    // 2. len should be mutiplier of 4, if it is not, len will be truncated to nearest one.
    static unsigned char *generateDecodedData(void *data, int len, int *resultLengthHolder);
    // 1. Clients should free() (not delete[]) the returned data.
    // 2. If the string length is not mutiplier of 4, it will be truncated to nearest one.
    static unsigned char *generateDecodedData(char *str, int *resultLengthHolder);
};

#endif//_SUPPORT_FORMAT_BASE64_H
