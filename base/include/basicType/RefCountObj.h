/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  basicType/RefCountObj.h                                                                     *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  01/14/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Base class for reference counted objects.  ref()/deref() are implemented.                   *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _BASIC_TYPE_REF_COUNT_OBJ_H
#define _BASIC_TYPE_REF_COUNT_OBJ_H

// Standard include
#include <atomic>

class RefCountObj
{
  public:
    void ref(void);
    void deref(void);

    // For general debug purpose.
    int getRefCount(void);

  protected:
    RefCountObj(void);
    virtual ~RefCountObj();

  private:
    std::atomic<int> refCnt;

    // Private copy constructor is declared but not defined to prevent accident copy.
    RefCountObj(const RefCountObj &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    RefCountObj &operator=(const RefCountObj &);
};

#endif//_BASIC_TYPE_REF_COUNT_OBJ_H
