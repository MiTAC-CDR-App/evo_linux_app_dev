/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  task/PeriodicTask.h                                                                         *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  04/11/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Periodic task, to execute by a fix period.                                                  *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _TASK_PERIODIC_TASK_H
#define _TASK_PERIODIC_TASK_H

// libBase includes
#include <osal/OsalMutex.h>
#include <osal/OsalCondVar.h>
#include <task/Runnable.h>

class PeriodicTask : public Runnable
{
  public:
    // runnable->run() is called as a work item.
    PeriodicTask(int periodInMS, Runnable *runnable);
    // timerHandler() is called as a work item.
    PeriodicTask(int periodInMS, int (*timerHandler)(void *), void *context);

    // 1. firstRunAfterMS is a hint only.
    // 2. If firstRunAfterMS < 0, it assumes that 1st run will occur after periodInMS.
    // 3. To avoid run() when reference count reach zero, ref() will be called.
    // 4. Cannot start when singleShot() is working.
    int start(int firstRunAfterMS = -1);
    // 1. if ms < 0, periodInMS is used.
    // 2. To avoid run() when reference count reach zero, ref() will be called.
    // 3. stop()/stopAndJoin() still can be used to cancel unfired singleShot().
    int singleShot(int ms = -1);
    // 1. deref() will be called once stopped.
    // 2. It should be taken care that the timer is triggering and the related running objects are under
    //    destruction, or deleted.  To prevent this, stopAndJoin() can be used.
    void stop(void);
    // 1. deref() will be called once stopped.
    // 2. It cannot be invoked inside the run call.
    void stopAndJoin(void);
    // Overrun will be re-count for new start().
    int getOverrun(void);
    int getPeriodInMS(void);
    // If this task is running, new period will be applied after the next dispatch.
    int setPeriod(int periodInMS);

    // 1. Default do nothing.
    // 2. This is called as a work item.
    virtual int run(void);

  protected:
    PeriodicTask(int periodInMS);
    virtual ~PeriodicTask();

  private:
    OsalMutex mutex;
    OsalCondVar condVarJoin;
    void *osalContext = 0;
    int periodInMS;
    Runnable *runnable = 0;
    int (*timerHandler)(void *) = 0;
    void *context = 0;
    bool started = false;
    bool singleShotting = false;
    bool running = false;
    int overrun = 0;

    // Private copy constructor is declared but not defined to prevent accident copy.
    PeriodicTask(const PeriodicTask &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    PeriodicTask &operator=(const PeriodicTask &);

    // Always execute work item PeriodicTask::_run().
    void triggerOnce(void);
    int dispatch(void);
    static int _dispatch(void *context);

    friend class OsalTaskExecutor;
};

#endif//_TASK_PERIODIC_TASK_H
