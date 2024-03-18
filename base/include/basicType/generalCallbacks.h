/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  basicType/generalCallbacks.h                                                                *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/26/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  General callbacks.                                                                          *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _BASIC_TYPE_GENERAL_CALLBACKS_H
#define _BASIC_TYPE_GENERAL_CALLBACKS_H

typedef int (*FuncGeneralCallback)(void *context);
typedef void (*FuncGeneralOnDone)(void *context, int result);
// progress is from 0 -> 1.
typedef void (*FuncGeneralOnProgress)(void *context, double progress);

#endif//_BASIC_TYPE_GENERAL_CALLBACKS_H
