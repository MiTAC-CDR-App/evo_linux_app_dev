/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  util/FileUtil.h                                                                             *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/23/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  File related utility functions.                                                             *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _UTIL_FILE_UTIL_H
#define _UTIL_FILE_UTIL_H

// Standard include
#include <string>

class FileUtil
{
  public:
    static std::string getFileName(const char *filePath);
    static std::string getFileName(const std::string &filePath);
    static std::string getFileExt(const char *filePath);
    static std::string getFileExt(const std::string &filePath);
    // Comparison is case-insensitive.
    static bool isFileExt(const char *filePath, const char *ext);
    static bool isFileExt(const std::string &filePath, const std::string &ext);
    static bool isFileExist(const char *filePath);
    static bool isFileExist(const std::string &filePath);
    // If file doen's exist, MIO_RESULT_IN_CORRECT_STATE_ALREADY is returned.  For all other cases, will log and
    // return error.
    static int remove(const char *filePath);
    static int remove(const std::string &filePath);
    // 1. With error log to ease clients' flow.
    // 2. If cross-partition, copy is used.
    static int move(const char *srcPath, const char *dstPath, bool silentForCopy = false);
    static int move(const std::string &srcPath, const std::string &dstPath, bool silentForCopy = false);
    static int copy(const char *srcPath, const char *dstPath);
    static int copy(const std::string &srcPath, const std::string &dstPath);
    // If new file is created, its attributes are user read/write and group read/write.
    static int saveFileContent(const char *filePath, const char *content);
    static int saveFileContent(const std::string &filePath, const std::string &content);
    // 1. For memory usage efficiency, only support <= 100K file, for may > 100K files, clients have better to
    //    read and parse (maybe seek too) those files.  If the file size > 100K, MIO_ERR_OUT_OF_RANGE is
    //    returned.
    // 2. For this usage, '\0' is not allowed inside the file, if '\0' is found, MIO_ERR_INVALID_DATA is
    //    returned.
    static int getFileContent(const char *filePath, std::string &contentHolder);
    static int getFileContent(const std::string &filePath, std::string &contentHolder);
    static int getFileLineContent(const char *filePath, std::string &lineHolder);
    static int getFileLineContent(const std::string &filePath, std::string &lineHolder);

    static int getInt(const char *filePath, int defaultValue);
    static int getInt(const std::string &filePath, int defaultValue);
    static int saveInt(const char *filePath, int value);
    static int saveInt(const std::string &filePath, int value);

    static float getFloat(const char *filePath, float defaultValue);
    static float getFloat(const std::string &filePath, float defaultValue);
    static int saveFloat(const char *filePath, float defaultValue);
    static int saveFloat(const std::string &filePath, float defaultValue);

    static double getDouble(const char *filePath, double defaultValue);
    static double getDouble(const std::string &filePath, double defaultValue);
    static int saveDouble(const char *filePath, double defaultValue);
    static int saveDouble(const std::string &filePath, double defaultValue);
};

inline std::string FileUtil::getFileName(const std::string &filePath)
{
    return getFileName(filePath.c_str());
}

inline std::string FileUtil::getFileExt(const std::string &filePath)
{
    return getFileExt(filePath.c_str());
}

inline bool FileUtil::isFileExt(const std::string &filePath, const std::string &ext)
{
    return isFileExt(filePath.c_str(), ext.c_str());
}

inline bool FileUtil::isFileExist(const std::string &filePath)
{
    return isFileExist(filePath.c_str());
}

inline int FileUtil::remove(const std::string &filePath)
{
    return remove(filePath.c_str());
}

inline int FileUtil::move(const std::string &srcPath, const std::string &dstPath, bool silentForCopy)
{
    return move(srcPath.c_str(), dstPath.c_str(), silentForCopy);
}

inline int FileUtil::copy(const std::string &srcPath, const std::string &dstPath)
{
    return copy(srcPath.c_str(), dstPath.c_str());
}

inline int FileUtil::saveFileContent(const std::string &filePath, const std::string &content)
{
    return copy(filePath.c_str(), content.c_str());
}

inline int FileUtil::getFileContent(const std::string &filePath, std::string &contentHolder)
{
    return getFileContent(filePath.c_str(), contentHolder);
}

inline int FileUtil::getFileLineContent(const std::string &filePath, std::string &lineHolder)
{
    return getFileLineContent(filePath.c_str(), lineHolder);
}

inline int FileUtil::getInt(const std::string &filePath, int defaultValue)
{
    return getInt(filePath.c_str(), defaultValue);
}

inline int FileUtil::saveInt(const std::string &filePath, int value)
{
    return saveInt(filePath.c_str(), value);
}

inline float FileUtil::getFloat(const std::string &filePath, float defaultValue)
{
    return getFloat(filePath.c_str(), defaultValue);
}

inline int FileUtil::saveFloat(const std::string &filePath, float value)
{
    return saveFloat(filePath.c_str(), value);
}

inline double FileUtil::getDouble(const std::string &filePath, double defaultValue)
{
    return getDouble(filePath.c_str(), defaultValue);
}

inline int FileUtil::saveDouble(const std::string &filePath, double value)
{
    return saveDouble(filePath.c_str(), value);
}

#endif//_UTIL_FILE_UTIL_H
