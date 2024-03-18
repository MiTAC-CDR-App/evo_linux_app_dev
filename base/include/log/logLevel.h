/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  log/logLevel.h                                                                              *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  09/02/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Enumeration for log levels.                                                                 *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _LOG_LOG_LEVEL_H
#define _LOG_LOG_LEVEL_H

/*!
 * @brief Log level for messages.
 */
enum LogLevel
{
    //! For fatal err log messages.  System will crash due to this error.
    LOG_FATAL,
    //! For critical err log messages.  If the error is possibly caused by wrong design, please use this
    //! level.
    LOG_CRITICAL,
    //! For general error messages.
    LOG_ERROR,
    //! For warning messages.
    LOG_WARNING,
    //! For Key/major flow messages.
    LOG_KEY_INFO,
    //! For normal flow messages.
    LOG_INFO,
    //! For debug messages.
    LOG_DEBUG
};

#endif//_LOG_LOG_LEVEL_H
