/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/StringBufferLogger.h                                                                    *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  10/15/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  String buffer logger, to log key info into string buffer.                                   *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_STRING_BUFFER_LOGGER_H
#define _LOG_STRING_BUFFER_LOGGER_H

// Standard include
#include <string>
// libBase includes
#include <osal/OsalMutex.h>
#include <log/SysLogger.h>

class StringBufferLogger : public SysLogger
{
  public:
    // bufSize will be passed to SysLogger for SysLogger::log(const char *, LogLevel, const char *, va_list).
    StringBufferLogger(int bufSize = 4096);
    virtual ~StringBufferLogger();

    std::string getQueuedString(void);
    void reset(void);

  protected:
    /* Implementation for SysLogger */
    // start() will reset buffer too.
    virtual int start(void);
    virtual void log(const char *tag, LogLevel logLevel, const char *logStr);

  private:
    OsalMutex mutex;
    std::string buffer;
};

#endif//_LOG_STRING_BUFFER_LOGGER_H
