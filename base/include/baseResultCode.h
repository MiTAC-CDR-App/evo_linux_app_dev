/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2018 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  baseResultCode.h                                                                            *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  07/13/18 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  Definition of all return codes in libBase.                                                  *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _BASE_RESULT_CODE_H
#define _BASE_RESULT_CODE_H

#define MIO_GENERAL_OK                                          0

#define MIO_RESULT_IN_CORRECT_STATE_ALREADY                     100

#define MIO_RESULT_LOADED                                       10000000
#define MIO_RESULT_QUEUING_FOR_LOAD                             10000001
#define MIO_RESULT_PARTIALLY_DONE                               10000002
#define MIO_RESULT_FULLY_DONE                                   10000003
#define MIO_RESULT_PARTIALLY_LOADED                             10000004
#define MIO_RESULT_NO_DATA                                      10000005
#define MIO_RESULT_BY_NORMAL_OP                                 10000006
#define MIO_RESULT_QUEUING_FOR_UPLOAD                           10000007
#define MIO_RESULT_QUEUING_FOR_ACTION                           10000008
#define MIO_RESULT_FILTERED                                     10000009
#define MIO_RESULT_NOT_ACCEPTED                                 10000010

#define MIO_WARNING_STRING_CUT                                  40000000
#define MIO_WARNING_IMPERFECT                                   40000001
#define MIO_WARNING_TIMEOUT                                     40000002
#define MIO_WARNING_BUSY                                        40000003
#define MIO_WARNING_NOT_ACCEPTED                                40000004

#define MIO_ERR_GENERAL                                         -1
#define MIO_ERR_INTERNAL                                        -2
#define MIO_ERR_INTERNAL_UNKNOWN                                -3
#define MIO_ERR_ILLEGAL_PARAMETERS                              -4
#define MIO_ERR_OUT_OF_MEMORY                                   -5
#define MIO_ERR_MAYBE_OUT_OF_MEMORY                             -6
#define MIO_ERR_NO_DATA                                         -7
#define MIO_ERR_GENERAL_INCONSIST                               -8
#define MIO_ERR_GENERAL_EXPIRED                                 -9
#define MIO_ERR_NOT_SUPPROTED                                   -10
#define MIO_ERR_DEPRECATED                                      -11
#define MIO_ERR_IO_GENERAL                                      -12
#define MIO_ERR_FORMAT_GENERAL                                  -13
#define MIO_ERR_INVALID_DATA                                    -14
#define MIO_ERR_OUT_OF_RANGE                                    -15
#define MIO_ERR_IO_NO_DEVICE                                    -16
#define MIO_ERR_IO_CLOSED										-17
#define MIO_ERR_READ_ONLY                                       -18
#define MIO_ERR_SYSTEM_BUSY                                     -20
#define MIO_ERR_PROTOCOL_MISMATCHED                             -21
#define MIO_ERR_TIMEOUT                                         -22
#define MIO_ERR_FILTERED                                        -23
#define MIO_ERR_FILE_NOT_FOUND                                  -30
#define MIO_ERR_NOT_ENOUGH_SPACE                                -31
#define MIO_ERR_SYS_LIMITATION                                  -40
#define MIO_ERR_INCORRECT_MODE                                  -10000
#define MIO_ERR_INCORRECT_STATUS                                -10001
#define MIO_ERR_CANCELD_BY_USER                                 -10002
#define MIO_ERR_SETTING_IS_REQUIRED                             -10010

#define MIO_REASON_SUBSYSTEM_STOPPED                            -40000000
#define MIO_REASON_OVERRIDED_BY_NEW_QUERY                       -40000001
#define MIO_REASON_CANCELLED                                    -40000002

#endif//_BASE_RESULT_CODE_H
