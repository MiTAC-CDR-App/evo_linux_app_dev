/*--------------------------------------------------------------------------------------------------------------------*
 *                                                                                                                    *
 * Copyright      2018 MiTAC International Corp.                                                                      *
 *                                                                                                                    *
 *--------------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                                    *
 * BINARY NAME :  libBase                                                                                             *
 * FILE NAME   :  util/StrUtil.h                                                                                      *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                        *
 * CREATED DATE:  06/12/19 (MM/DD/YY)                                                                                 *
 * DESCRIPTION :  String related utility functions.                                                                   *
 *--------------------------------------------------------------------------------------------------------------------*/

#ifndef _STR_TIME_UTIL_H
#define _STR_TIME_UTIL_H

// Standard include
#include <string>
// libBase include
#include <container/List.h>

class StrUtil
{
  public:
    // Return true when parsing OK.
    static bool parsePureDecimal(const char *ptr, int digits, int *valueHolder);
    // Only remove space characters.
    static std::string truncate(const char *str);
    // 1. Space characters at start/end of the splitted string will be truncated.
    // 2. If delimiter is space character, continuous space characters will be treated as one delimiter.  For
    //    example, split("str1,,str3", ',') will return "str1", "", "str3"; split("str1  str3", '\t'), will
    //    return "str1", "str3".
    static List<std::string> split(const char *str, char delimiter, bool allowEmptyString = true);
};

#endif//_STR_TIME_UTIL_H
