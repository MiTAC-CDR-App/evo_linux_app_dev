/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/FileBasedLogger.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  03/02/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  File based logger, utility base class for general file based classes.                       *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_FILE_BASED_LOGGER_H
#define _LOG_FILE_BASED_LOGGER_H

// Standard includes
#include <stdio.h>
#include <string>
// libBase includes
#include <osal/OsalMutex.h>
#include <log/SysLogger.h>

class FileBasedLogger : public SysLogger
{
  public:
    FileBasedLogger(const char *fullLogDirPath);

    /*!
     * Destructor.
     */
    virtual ~FileBasedLogger();

    /* Used by clients or child classes */
    size_t currentFileSize(void);

  protected:
    /* Implementation for SysLogger */
    virtual int start(void);
    virtual void stop(void);
    virtual void log(const char *tag, LogLevel logLevel, const char *formatStr, va_list variableArgList);
    virtual void flush();
    virtual void cut(void);
    virtual size_t currentSize(void);

    /* Should be implemented by child classes */
    // Default implementation do nothing.
    virtual void cleanupOldFiles(int lastClosedFileSize);
    // Right before new log file open, can always assume will have one new log file.
    virtual std::string generateNextFilename(void) = 0;

    /* access by child classes */
    const char *fullLogDirPath(void);

  private:
    OsalMutex mutex;
    char *_fullLogDirPath;
    FILE *fp;
};

#endif//_LOG_FILE_BASED_LOGGER_H
