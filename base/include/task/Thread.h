/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  task/Thread.h                                                                               *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  02/24/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Thread.                                                                                     *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _TASK_THREAD_H
#define _TASK_THREAD_H

// libBase include
#include <task/Runnable.h>

class _ThreadCtrlBridge;

class Thread : public Runnable
{
  public:
    Thread(void);
    // runnable is ref(), and deref() in dtor.
    Thread(Runnable *runnable);

    // This thread execution is heavy-weight, try to use ThreadPool::executeTaskItem();
    static int startThread(int (*threadEntry)(void *), void *context);
    // 1. This thread execution is heavy-weight, try to use ThreadPool::executeTaskItem();
    // 2. runnable is ref(), and deref() when end.
    static int startThread(Runnable *runnable);
    static int getCurrentThreadID(void);
    static void msleep(int ms);
    // Implemented by POSIX sleep() directly.
    static void sleep(int seconds);

    // 1. It will self ref(), and deref() when stopped.
    // 2. If the thread is started, MIO_ERR_INCORRECT_STATUS is returned.
    // 3. If the object is constructed with Runnable object runnable, runnable->run() is used.
    // 4. If the thread is stopped, it can be start() w/o problems, however, it's child object's reposibility,
    //    or Runnable object's reposibility to make sure 2nd, 3rd, ... run() w/o problems.
    int start(void);
    // 1. Only the Thread object is constructed by default constructor can call this without problem, otherwise,
    //    MIO_ERR_INCORRECT_MODE is returned.
    // 2. It will self ref(), and deref() when stopped.
    // 3. If the thread is started, MIO_ERR_INCORRECT_STATUS is returned.
    // 4. runnable is ref(), and deref() when end.
    // 5. run() is not used, and runnable->run() is called.
    // 6. If the thread is stopped, it can be start() w/o problems.
    int start(Runnable *runnable);
    // It will return immediately if the the thread is not started.
    int join(void);

    // Default implementation do nothing.
    virtual int run(void);

  protected:
    virtual ~Thread();

  private:
    _ThreadCtrlBridge *ctrlBridge;
    const bool shouldHoldRunnable = false;
    Runnable *runnable = 0;

    // Private copy constructor is declared but not defined to prevent accident copy.
    Thread(const Thread &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    Thread &operator=(const Thread &);

    int dispatch(void);

    // These function is used only used by PooledThread to avoid race condition.
    int __forceStart(void);
    // Silent mode is only for PooledThread.
    void __setSilent4Restart(bool silent4Restart);

    friend class _ThreadCtrlBridge;
    friend class PooledThread;
};

#endif//_TASK_THREAD_H
