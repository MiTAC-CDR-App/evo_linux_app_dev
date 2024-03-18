/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2021 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  util/endianOPs.h                                                                            *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/23/21 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Some simple operations for endian transfer.                                                 *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _UTIL_ENDIAN_OPS_H
#define _UTIL_ENDIAN_OPS_H

inline int getBE32(void *ptr)
{
    const unsigned char *bytes = (const unsigned char *) ptr;
    return bytes[3] + (bytes[2] << 8) + (bytes[1] << 16) + (bytes[0] << 24);
}

inline int getInt(void *ptr)
{
    return *((int *) ptr);
}

inline int toLE32(int valBE32)
{
    return getBE32(&valBE32);
}

inline int toBE32(int valLE32)
{
    return getBE32(&valLE32);
}

inline void setBE32(void *ptr, int data)
{
    *((int *) ptr) = toBE32(data);
}

inline void setInt(void *ptr, int data)
{
    *((int *) ptr) = data;
}

#endif//_UTIL_ENDIAN_OPS_H
