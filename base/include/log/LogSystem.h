/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/LogSystem.h                                                                             *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/04/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Log system class (multi-thread-safe).                                                       *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_LOG_SYSTEM_H
#define _LOG_LOG_SYSTEM_H

// Standard includes
#include <stddef.h>
#include <stdarg.h>
// libBase includes
#include <container/List.h>
#include <osal/OsalMutex.h>
#include <log/logLevel.h>

class SysLogger;
struct LogSystemFilterItem;

#define _logSourceLocation(tag)   \
    LogSystem::i(tag, "At %s, %s, line %d", __FILE__, __PRETTY_FUNCTION__, __LINE__)

/*!
 * @brief Log control system for global log system, or specified log system.
 */
class LogSystem
{
  public:
    /*!
     * Constructor.
     */
    LogSystem(void);

    ~LogSystem();

    /*!
     * Get the system-wised log system.
     *
     * @return The system-wised LogSystem object.
     */
    static LogSystem *getSysLogSystem(void);

    /*************************
     * Log control functions *
     *************************/

    /*!
     * Set logger of the log system.
     *
     * @param logger The logger to be set for the log system.  If the logger is set as 0, it has the same
     *        effect to disable the log system.
     * @remarks If the corresponding log system started already, the SysLogger will not be set.
     */
    void setLogger(SysLogger *logger);
    /*!
     * Set log level of the log system.
     *
     * @param logLevel The log level to be set for the log system.  Log messages with lower or equal log level,
     *        will be logged by the logger.  Default is LOG_INFO.
     */
    void setLogLevel(LogLevel logLevel);

    // 1. This is used to simplify debug, not for regular usage.
    // 2. strTags are seperated by ',', for example, "main,usb", only logs with tag "main", "usb" are acceptable.
    // 3. If strTags is 0, or an empty string, filter is reset.
    // 4. Filter tags can have '*' as wildcard at the start or end.  For example, for filter tag "Test*", tags
    //    "Test123" pass the test, but tag "UsbTest" doesn't.  For filter tag "*Test", tag "UsbTest" pass the
    //    test, but tag "Test123" doesn't.  For filter tag "*Test*", both "Test123" and "UsbTest" pass the test.
    //    For simplicity, only start '*' or end '*' are treated as wildcard, middle '*'(s) is/are treated as
    //    normal characters.
    void setLogFilter(const char *strTags);

    /*!
     * Start the log mechanism of the log system.
     *
     * @remarks
     *   1. SysLogger() should be set to start the log mechanism.\n
     *   2. SysLogger::start() will be invoked, it is used for some SysLogger, for example, new file may be
     *      generated due to this call.
     */
    void startLog(void);
    /*!
     * Stop the log mechanism of the log system.
     *
     * @remarks SysLogger::stop() will be invoked, it is used for some SysLogger, for example, the log file may
     *          be closed due to this call.
     */
    void stopLog(void);

    void flushLog(void);

    void cutLog(void);

    size_t currentLogSize(void);

    /********************
     * Module functions *
     ********************/

    /*!
     * Log message.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param logLevel The log level of this message.
     * @param formatStr printf() styled UTF8 format string.
     * @param ... The arguments for formatStr substitutions, those arugments should be in POD (Plain Old Data)
     *        types.
     */
    void log(const char *tag, LogLevel logLevel, const char *formatStr, ...);

    /*!
     * Log message.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param logLevel The log level of this message.
     * @param formatStr printf() styled UTF8 format string.
     * @param variableArgList This offer the access to clients' variable argument list, it is useful if clients
     *        want to wrap log() again.
     */
    void log(const char *tag, LogLevel logLevel, const char *formatStr, va_list variableArgList);

    /*!
     * Log message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param logLevel The log level of this message.
     * @param formatStr printf() styled UTF8 format string.
     * @param ... The arguments for formatStr substitutions, those arugments should be in POD (Plain Old Data)
     *        types.
     */
    static void l(const char *tag, LogLevel logLevel, const char *formatStr, ...);

    /*!
     * Log message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param logLevel The log level of this message.
     * @param formatStr printf() styled UTF8 format string.
     * @param variableArgList This offer the access to clients' variable argument list, it is useful if clients
     *        want to wrap log() again.
     */
    static void l(const char *tag, LogLevel logLevel, const char *formatStr, va_list variableArgList);

    /*!
     * Log error message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param formatStr printf() styled UTF8 format string.
     */
    static void e(const char *tag, const char *formatStr, ...);

    /*!
     * Log warning message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param formatStr printf() styled UTF8 format string.
     */
    static void w(const char *tag, const char *formatStr, ...);

    /*!
     * Log key/major information message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param formatStr printf() styled UTF8 format string.
     */
    static void ki(const char *tag, const char *formatStr, ...);

    /*!
     * Log normal information message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param formatStr printf() styled UTF8 format string.
     */
    static void i(const char *tag, const char *formatStr, ...);

    /*!
     * Log debug message by default LogSystem object.
     *
     * @param tag The module tag, in UTF8 null-terminated string.
     * @param formatStr printf() styled UTF8 format string.
     */
    static void d(const char *tag, const char *formatStr, ...);

    static void flush(void);

  private:
    OsalMutex dataMutex;
    SysLogger *sysLogger;
    LogLevel logLevelThreshold;
    bool started;
    bool isWithFilter;
    List<LogSystemFilterItem *> filterItems;
};

#endif//_LOG_LOG_SYSTEM_H
