/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  task/Runnable.h                                                                             *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  04/08/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Runnable interface.                                                                         *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _TASK_RUNNABLE_H
#define _TASK_RUNNABLE_H

// libBase include
#include <basicType/RefCountObj.h>

class Runnable : public RefCountObj
{
  public:
    Runnable(void);

    virtual int run(void) = 0;

  protected:
    virtual ~Runnable();

  private:
    // Private copy constructor is declared but not defined to prevent accident copy.
    Runnable(const Runnable &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    Runnable &operator=(const Runnable &);
};

#endif//_TASK_RUNNABLE_H
