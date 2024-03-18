/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  util/TimeUtil.h                                                                             *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  09/03/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Date/time related utility functions.                                                        *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _UTIL_TIME_UTIL_H
#define _UTIL_TIME_UTIL_H

// Standard includes
#include <stdint.h>
#include <string>

#define UTC_2_MP4_TIME_SECONDS  2082844800

// Follow modern datetime representation, ms after 1970/1/1.
class TimeUtil
{
  public:
    static int64_t now(void);
    // By local timezone.
    static int64_t fromYMD(int yy, int mm, int dd);
    static int64_t fromYMDHMS(int yy, int MM, int dd, int hh, int mm, int ss, bool isLocal);
    // 1. Return -1ll if parsing error.
    // 2. By local time.
    // 3. Legal values: 1 <= month <= 12, 1 <= day <= 31, 0 <= hour <= 23, 0 <= minute/second <= 59.
    static int64_t parseYYYYMMDDHHMMSS(const char *ptrYYYYMMDD, const char *ptrHHMMSS);
    // 1. Return -1ll if parsing error.
    // 2. By local time.
    // 3. Legal values: 1 <= month <= 12, 1 <= day <= 31, 0 <= hour <= 23, 0 <= minute/second <= 59.
    static int64_t parseYYMMDDHHMMSS(const char *ptrYYMMDD, const char *ptrHHMMSS);
    // By local timezone.
    static int64_t cutToDate(int64_t datetime);
    // By local timezone.
    static bool isSameDay(int64_t datetime1, int64_t datetime2);
    // In hours.
    static void getYMD(int64_t datetime, bool isLocal, int *yyHolder, int *mmHolder, int *ddHolder);
    static void getYMDHDS(int64_t datetime, bool isLocal, int *yyHolder, int *MMHolder, int *ddHolder,
                                                                 int *hhHolder, int *mmHolder, int *ssHolder);
    static int getGMTBias(void);
    static void getYMDWithGMTBias(int64_t datetime, int gmtBias, int *yyHolder, int *mmHolder, int *ddHolder);
    // By seconds.
    static int offsetFromUtc(void);
    // dateStr's format is YYYYMMDD, timeStr's format is hhmmss.
    static void getDateTimeStr(int64_t datetime, bool isLocal, std::string &dateStrHodler,
                                                                                   std::string &timeStrHolder);
    static void getCurrentDateTimeStr(bool isLocal, std::string &dateStrHodler, std::string &timeStrHolder);
    static int64_t monotonicNowInMS(void);
};

#endif//_UTIL_TIME_UTIL_H
