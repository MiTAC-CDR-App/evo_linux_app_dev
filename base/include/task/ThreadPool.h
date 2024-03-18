/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  task/ThreadPool.h                                                                           *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  04/10/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Thread pool, for small and simple tasks.                                                    *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _TASK_THREAD_POOL_H
#define _TASK_THREAD_POOL_H

// libBase includes
#include <container/List.h>
#include <osal/OsalMutex.h>
#include <task/Runnable.h>

class PooledThread;

/*
1. Pooled thread means the thread which is managed by this thread pool, no matter running or idle.
2. Idle-pooled-thread means that, it is a pooled thread and not executing any work item.
*/
class ThreadPool
{
  public:
    // 1. allowExtraThreads is not usually set, for that case, GlobalThreadPool is preferred to use.
    // 2. If allowExtraThreads is false, task items will be queued to execution when no idle-pooled-threads.
    // 3. If allowExtraThreads is true, extra threads will created when no idle-pooled-threads, and some pooled
    //    threads will be released once total idle-pool-threads execeed some dynamic threshold.
    ThreadPool(int maxPooledThreads, bool allowExtraThreads = false);
    // 1. All pooled thread will be deref().
    ~ThreadPool();

    int maxRunningWorkItems(void);
    int maxQueuedWorkItems(void);

    // The execution is light-weight (except when new pooled thread is created).
    int executeTaskItem(int (*taskEntry)(void *), void *context);
    // 1. The execution is light-weight (except when new pooled thread is created).
    // 2. workItem is ref(), and deref() when end.
    int executeTaskItem(Runnable *workItem);

  private:
    int maxPooledThreads;
    bool allowExtraThreads;
    OsalMutex mutex;
    bool destructing = false;
    List<PooledThread *> idlingThreads;
    List<PooledThread *> runningThreads;
    List<Runnable *> queuedWorkItems;
    int _maxRunningWorkItems = 0;
    int _maxQueuedWorkItems = 0;

    // Private copy constructor is declared but not defined to prevent accident copy.
    ThreadPool(const ThreadPool &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    ThreadPool &operator=(const ThreadPool &);

    void _returnIdleThread(PooledThread *thread);

    friend class PooledThread;
    friend class GlobalThreadPool;
};

class GlobalThreadPool
{
  public:
    // Once shutdown, no more task item executions can be done.
    static void shutdown(void);
    static int getMaxPooledThreads(void);
    // Default is 20.
    static void setMaxPooledThreads(int maxPooledThreads);
    static int maxRunningWorkItems(void);
    static int maxQueuedWorkItems(void);
    // 1. If allowExtraThreads is false, task items will be queued to execution when no idle-pooled-threads.
    // 2. If allowExtraThreads is true, extra threads will created when no idle-pooled-threads, and some pooled
    //    threads will be released once total idle-pool-threads execeed some dynamic threshold.
    // 3. If false -> true, and there are some queued task items, new pooled threads will be created to execute
    //    them.
    static void allowExtraThreads(bool allowed);
    // 1. The execution is light-weight (except when new pooled thread is created).
    // 2. If shutdown() already, it will return error MIO_ERR_INCORRECT_STATUS.
    // 3. If afterMS <= 0, the task is executed immediately.
    static int executeTaskItem(int (*workItemEntry)(void *), void *context, int afterMS = 0);
    // 1. The execution is light-weight (except when new pooled thread is created).
    // 2. If shutdown() already, it will return error MIO_ERR_INCORRECT_STATUS.
    // 3. workItem is ref(), and deref() when end.
    // 4. If afterMS <= 0, the task is executed immediately.
    static int executeTaskItem(Runnable *workItem, int afterMS = 0);

  private:
    // Private copy constructor is declared but not defined to prevent object creation.
    GlobalThreadPool(const ThreadPool &);
};

#endif//_TASK_THREAD_POOL_H
