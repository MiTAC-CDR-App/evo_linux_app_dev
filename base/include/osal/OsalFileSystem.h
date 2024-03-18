/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  osal/OsalFileSystem.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  03/02/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  OSAL file system.                                                                           *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _OSAL_OSAL_FILE_SYSTEM_H
#define _OSAL_OSAL_FILE_SYSTEM_H

// Standard include
#include <string>
// POSIX include
#include <sys/stat.h>
// libBase
#include <container/List.h>

/* Partial designed */
class OsalFileSystem
{
  public:
    static std::string generateFullFilePath(const char *fullDirPath, const char *filename);
    static std::string generateFullDirPath(const char *fullParentDirPath, const char *subDir);
    // Default, user: read/write + execute, group: read/write + execute, others: read + execute.
    static int mkdirs(const char *dirPath, mode_t mode = (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH));
    // Normal files only, that is, not including directory, link, and etc.
    static int scanFiles(const char *dirFullPath, List<std::string> &filenamesHolder);
};

#endif //_OSAL_OSAL_FILE_SYSTEM_H
