/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  jni/registerJniMethods.h                                                                    *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/10/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Utility method to register JNI methods.                                                     *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _JNI_REGISTER_JNI_METHODS_H
#define _JNI_REGISTER_JNI_METHODS_H

// JNI include
#include <jni.h>

// JNINativeMethod like struct to ease the access to use const char *.
struct _JNINativeMethod
{
    const char *name;
    const char *signature;
    void * fnPtr;
};

void registerNativeMethods(JNIEnv* env, const char *className, JNINativeMethod *methods);

inline void registerNativeMethods(JNIEnv* env, const char *className, _JNINativeMethod *methods)
{
    registerNativeMethods(env, className, (JNINativeMethod *) methods);
}

#endif//_JNI_REGISTER_JNI_METHODS_H
