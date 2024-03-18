/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/json/SimpleJsonArray.h                                                              *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/16/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Simple JSON array.                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_JSON_SIMPLE_JSON_ARRAY_H
#define _SUPPORT_JSON_SIMPLE_JSON_ARRAY_H

// Standard include
#include <string>
// libBase includes
#include <container/List.h>
#include <support/json/EJsonError.h>

class SimpleJsonData;
class SimpleJsonObj;

class SimpleJsonArray
{
  public:
    SimpleJsonArray(void);
    ~SimpleJsonArray();

    int size(void);

    bool isNull(int ndx) /*throw (EJsonError)*/;
    bool isNull(int ndx, bool defaultValue);
    bool getBoolean(int ndx) /*throw (EJsonError)*/;
    bool getBoolean(int ndx, bool defaultValue);
    int getInt(int ndx) /*throw (EJsonError)*/;
    int getInt(int ndx, int defaultValue);
    long long getLongLong(int ndx) /*throw (EJsonError)*/;
    long long getLongLong(int ndx, long long defaultValue);
    double getDouble(int ndx) /*throw (EJsonError)*/;
    double getDouble(int ndx, double defaultValue);
    const char *getString(int ndx) /*throw (EJsonError)*/;
    const char *getString(int ndx, const char *defaultStr);
    const char *getStringOrNull(int ndx);
    SimpleJsonObj *getObj(int ndx) /*throw (EJsonError)*/;
    SimpleJsonObj *getObjOrNull(int ndx);
    SimpleJsonArray *getArray(int ndx) /*throw (EJsonError)*/;
    SimpleJsonArray *getArrayOrNull(int ndx);

    // set(int ndx, ...) is not usually be used, so, only add(...)s are offered.
    void addNull(void);
    void add(bool value);
    void add(int value);
    void add(long long value);
    void add(double value);
    // If value is 0(NULL), EJsonError will be throwed.
    void add(const char *value) /*throw (EJsonError)*/;
    void add(const std::string &value) /*throw (EJsonError)*/;
    // obj's ownership is transferred, clients should not delete obj.
    void add(SimpleJsonObj *obj);
    // array's ownership is transferred, clients should not delete array.
    void add(SimpleJsonArray *array);
    // If forDebug is true, the returned string cannot be used as standard JSON string.
    std::string generateString(bool compactMode = true, bool forDebug = false);

  private:
    List<SimpleJsonData *> objs;

    // Declared as private to disallow the access.
    SimpleJsonArray(SimpleJsonArray &src);
    SimpleJsonArray &operator=(SimpleJsonArray &src);

    void add(SimpleJsonData *data);

    void ndxCheck(int ndx) /*throw (EJsonError)*/;

    void appendStr(std::string &str, bool compactMode, bool forDebug, int identSpaces, bool newLineFirst);

    friend class SimpleJsonObj;
    friend class SimpleJsonData;
    friend class SimpleJsonParser;
};

inline void SimpleJsonArray::add(const std::string &value)
{
    add(value.c_str());
}

#endif//_SUPPORT_JSON_SIMPLE_JSON_ARRAY_H
