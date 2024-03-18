/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/logSystemBridge.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  09/02/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Bridge LogSystem to C supporting log functions.                                             *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_LOG_SYSTEM_BRIDGE_H
#define _LOG_LOG_SYSTEM_BRIDGE_H

// liBase include
#include <log/logLevel.h>

#ifdef __cplusplus
extern "C"
{
#endif

void logSystem_l(const char *tag, enum LogLevel logLevel, const char *formatStr, ...);
void logSystem_e(const char *tag, const char *formatStr, ...);
void logSystem_w(const char *tag, const char *formatStr, ...);
void logSystem_ki(const char *tag, const char *formatStr, ...);
void logSystem_i(const char *tag, const char *formatStr, ...);
void logSystem_d(const char *tag, const char *formatStr, ...);

// Wrapper to log to specified LogSystem object.
void altLogSystem_l(void *logSystem, const char *tag, enum LogLevel logLevel, const char *formatStr, ...);

#ifdef __cplusplus
}
#endif

#endif//_LOG_LOG_SYSTEM_BRIDGE_H
