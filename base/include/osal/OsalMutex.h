/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  osal/OsalMutex.h                                                                            *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/04/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  OSAL Mutex.                                                                                 *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _OSAL_OSAL_MUTEX_H
#define _OSAL_OSAL_MUTEX_H

class OsalMutex
{
  public:
    OsalMutex(void);
    ~OsalMutex();

    void lock();
    void unlock();

  private:
    void *context;

    friend class OsalCondVar;
};

#endif //_OSAL_OSAL_MUTEX_H
