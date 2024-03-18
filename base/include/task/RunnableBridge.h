/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  task/RunnableBridge.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  04/10/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Bridge normal function entry to Runnable object.                                            *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _TASK_RUNNABLE_BRIDGE_H
#define _TASK_RUNNABLE_BRIDGE_H

// libBase include
#include <task/Runnable.h>

class RunnableBridge : public Runnable
{
  public:
    RunnableBridge(int (*entry)(void *), void *context);

    virtual int run(void);

  protected:
    virtual ~RunnableBridge();

  private:
    int (*entry)(void *);
    void *context;

    // Private copy constructor is declared but not defined to prevent accident copy.
    RunnableBridge(const RunnableBridge &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    RunnableBridge &operator=(const RunnableBridge &);
};

#endif//_TASK_RUNNABLE_BRIDGE_H
