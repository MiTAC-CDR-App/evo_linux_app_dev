/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/SysLogger.h                                                                             *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/04/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  The interface to really log messages.                                                       *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_SYS_LOGGER_H
#define _LOG_SYS_LOGGER_H

// Standard includes
#include <stddef.h>
#include <stdarg.h>
// libBase include
#include <log/LogSystem.h>

/*!
 * @brief System logger, which really implement how to output log messages to underlining media, such as files,
 *        console, TCP connection, or etc.
 */
class SysLogger
{
  public:
    SysLogger(void);
    // This is used with SysLogger::log(const char *, LogLevel, const char *, va_list).
    SysLogger(int bufferSize);

    /*!
     * Destructor.
     */
    virtual ~SysLogger(void);

  protected:
    /*!
     * This is called when the log system is requested to start.  It is used for some cases, for example, new
     * file may be generated due to this call.
     *
     * @remarks
     *   1. SystemLog::log() should be not called inside start() call sequence; otherwise, it will cause
     *      unpredicatable recursive locks.
     *   2. Default implementation is do nothing and return MIO_GENERAL_OK.
     */
    virtual int start(void);
    /*!
     * This is called when the log system is requested to stop.  It is used for some cases, for example, the log
     * file may be closed due to this call.
     *
     *   1. SystemLog::log() should be not called inside start() call sequence; otherwise, it will cause
     *      unpredicatable recursive locks.
     *   2. Default implementation is do nothing.
     */
    virtual void stop(void);
    /*!
     * This is the only log entry called by LogSystem.
     *
     * @remarks
     *   1. SystemLog::log() should be not called inside stop() call sequence; otherwise, it will cause
     *      unpredicatable recursive locks.
     *   2. Default implementation will call log(const char *, LogLevel, const char *), implementation must
     *      choose to implement one of the log() methods, but not both.
     *   3. If the default implementation is used, SysLogger::SysLogger(int) should be used to construct the
     *      object with internal buffer.
     */
    virtual void log(const char *tag, LogLevel logLevel, const char *formatStr, va_list variableArgList);
    /*!
     * This is called by SysLogger's default log(const char *, LogLevel, const char *, va_list) implementation.
     * So, if the implementation overrides log(const char *, LogLevel, const char *, va_list), this function is
     * useless.  Implementation must choose to implement one of the log() methods, but not both.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param logLevel The log level of this message.
     * @param logStr The log string, in UTF8 null-terminated string.
     *
     * @remarks
     *   1. SystemLog::log() should be not called inside log() call sequence; otherwise, it will cause
     *      unpredicatable recursive locks.
     *   2. Multi-thread protection is done in LogSystem already.
     *   3. logStr passed by SysLogger::log(const char *, LogLevel, const char *, va_list) is w/o newline, so,
     *      the implementation should take care it.
     */
    virtual void log(const char *tag, LogLevel logLevel, const char *logStr);

    // Default do nothing.
    virtual void flush(void);

    // Default do nothing.
    virtual void cut(void);

    // Default implementation return 0;
    virtual size_t currentSize(void);

  private:
    const int bufferSize;
    // This is used with SysLogger::log(const char *, LogLevel, const char *, va_list).
    char *buffer;

    friend class LogSystem;
};

#endif//_LOG_SYS_LOGGER_H
