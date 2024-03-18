/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  osal/OsalCondVar.h                                                                          *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  08/01/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  OSAL condition variable.                                                                    *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _OSAL_OSAL_COND_VAR_H
#define _OSAL_OSAL_COND_VAR_H

class OsalMutex;

class OsalCondVar
{
  public:
    OsalCondVar(void);
    ~OsalCondVar();

    // Return false if timeout or something wrong.
    bool wait(OsalMutex &mutex, int timeoutInMS = 0);
    void signal(void);
    void broadcast(void);

  private:
    void *context;
};

#endif //_OSAL_OSAL_COND_VAR_H
