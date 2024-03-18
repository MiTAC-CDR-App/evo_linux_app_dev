/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/ConsoleLogger.h                                                                         *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  09/03/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Simple SysLogger implementation by console.                                                 *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_CONSOLE_LOGGER_H
#define _LOG_CONSOLE_LOGGER_H

// libBase include
#include <log/SysLogger.h>

/*!
 * @brief The SysLogger implementation by standard C console functions.
 */
class ConsoleLogger : public SysLogger
{
  public:
    virtual ~ConsoleLogger(void);

  protected:
    virtual void log(const char *tag, LogLevel logLevel, const char *formatStr, va_list variableArgList);
};

#endif//_LOG_CONSOLE_LOGGER_H
