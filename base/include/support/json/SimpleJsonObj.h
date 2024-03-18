/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/json/SimpleJsonObj.h                                                                *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/16/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Simple JSON object.                                                                         *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_JSON_SIMPLE_JSON_OBJ_H
#define _SUPPORT_JSON_SIMPLE_JSON_OBJ_H

// Standard include
#include <string>
// libBase includes
#include <log/LogSystem.h>
#include <util/SimplePropertySet.h>
#include <support/json/EJsonError.h>

class SimpleJsonArray;
class SimpleJsonData;

class SimpleJsonObj
{
  public:
    SimpleJsonObj(void);
    ~SimpleJsonObj();

    bool has(const char *name);

    bool isNull(const char *name) /*throw (EJsonError)*/;
    bool isNull(const char *name, bool defaultValue);
    bool getBoolean(const char *name) /*throw (EJsonError)*/;
    bool getBoolean(const char *name, bool defaultValue);
    int getInt(const char *name) /*throw (EJsonError)*/;
    int getInt(const char *name, int defaultValue);
    long long getLongLong(const char *name) /*throw (EJsonError)*/;
    long long getLongLong(const char *name, long long defaultValue);
    double getDouble(const char *name) /*throw (EJsonError)*/;
    double getDouble(const char *name, double defaultValue);
    const char *getString(const char *name) /*throw (EJsonError)*/;
    const char *getString(const char *name, const char *defaultStr);
    const char *getStringOrNull(const char *name);
    SimpleJsonObj *getObj(const char *name) /*throw (EJsonError)*/;
    SimpleJsonObj *getObjOrNull(const char *name);
    SimpleJsonArray *getArray(const char *name) /*throw (EJsonError)*/;
    SimpleJsonArray *getArrayOrNull(const char *name);

    void putNull(const char *name);
    void put(const char *name, bool value);
    void put(const char *name, int value);
    void put(const char *name, long long value);
    void put(const char *name, double value);
    // If value is 0(NULL), EJsonError will be throwed.
    void put(const char *name, const char *value) /*throw (EJsonError)*/;
    void put(const char *name, const std::string &value) /*throw (EJsonError)*/;
    void putStringIfNotNull(const char *name, const char *value);
    // obj's ownership is transferred, clients should not delete obj.
    void put(const char *name, SimpleJsonObj *obj);
    // array's ownership is transferred, clients should not delete array.
    void put(const char *name, SimpleJsonArray *array);
    void remove(const char *name);

    // 1. Clients should delete the returned object once it is not required any more.
    // 2. The string should contains a complete JSON object or array, if it's a array, this array is wrapped
    //    inside a JSON object with the name "data".
    static SimpleJsonObj *parseAndGenerate(const char *string) /*throw (EJsonError)*/;
    // 1. Clients should delete the returned object once it is not required any more.
    // 2. The string should contains a complete JSON object or array, if it's a array, this array is wrapped
    //    inside a JSON object with the name "data".
    // 3. For simplicity, if filePath cannot be opened, EJsonError will be throwed too.
    static SimpleJsonObj *parseFileAndGenerate(const char *filePath) /*throw (EJsonError)*/;
    // If forDebug is true, the returned string cannot be used as standard JSON string.
    std::string generateString(bool compactMode = true, bool forDebug = false);
    int save(const char *filePath, bool compactMode = true);
    // Clients should delete the returned object once it is not required any more.
    SimpleJsonObj *dup(void);

    void dump(LogLevel logLevel, const char *tag);

  private:
    SimplePropertySet objs;

    // Declared as private to disallow the access.
    SimpleJsonObj(SimpleJsonObj &src);
    SimpleJsonObj &operator=(SimpleJsonObj &src);

    SimpleJsonData *get(const char *name) /*throw (EJsonError)*/;
    void put(const char *name, SimpleJsonData *data);

    void appendStr(std::string &str, bool compactMode, bool forDebug, int identSpaces, bool newLineFirst);

    friend class SimpleJsonArray;
    friend class SimpleJsonData;
    friend class SimpleJsonParser;
};

inline void SimpleJsonObj::put(const char *name, const std::string &value)
{
    put(name, value.c_str());
}

#endif//_SUPPORT_JSON_SIMPLE_JSON_OBJ_H
