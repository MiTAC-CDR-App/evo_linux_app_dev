/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2023 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  jni/jniModules.h                                                                            *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/27/23 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Header to include all libBase supported JNI modules.                                        *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _JNI_JNI_MODULES_H
#define _JNI_JNI_MODULES_H

// JNI include
#include <jni.h>

void registerLogSystemMethods(JNIEnv *env);
void registerOsMethods(JNIEnv *env);
void registerTimeUtilMethods(JNIEnv *env);
void registerFileObserverMethods(JNIEnv *env);

// Include LogSystem, Os, TimeUtil, FileObserver.
void registerCommonClassesMethods(JNIEnv *env);

#endif//_JNI_JNI_MODULES_H
