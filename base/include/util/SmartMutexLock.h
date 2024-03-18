/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  util/SmartMutexLock.h                                                                       *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/04/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  SmartMutexLock for automatic mutex lock/unlock.                                             *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _UTIL_SMART_MUTEX_LOCK_H
#define _UTIL_SMART_MUTEX_LOCK_H

// libBase include
#include <osal/OsalMutex.h>

/*!
 * @brief Smart mutex lock, which will lock the embedded mutex when constructing, and unlock the embedded mutex
 *        when destructing.  It's useful for complex code block.
 */
class SmartMutexLock
{
  public:
    /*!
     * Constructor.
     *
     * @mutex The mutex for smart lock/unlock.
     *
     * @remarks The corresponding mutex will be automatically locked.
     */
    SmartMutexLock(OsalMutex &mutex) : _mutex(mutex)
    {
        _mutex.lock();
    }
    /*!
     * Destructor.
     *
     * @remarks The corresponding mutex will be automatically unlocked.
     */
    ~SmartMutexLock()
    {
        _mutex.unlock();
    }

  private:
    OsalMutex &_mutex;

    // Private copy constructor is declared but not defined to prevent accident copy.
    SmartMutexLock(SmartMutexLock &);
    // Private assignment operator is declared but not defined to prevent accident assignment.
    SmartMutexLock &operator=(SmartMutexLock &);
};

#endif//_UTIL_SMART_MUTEX_LOCK_H
