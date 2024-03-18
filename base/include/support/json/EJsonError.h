/*------------------------------------------------------------------------------------------------------------*
 *                                                                                                            *
 * Copyright      2020 MiTAC International Corp.                                                              *
 *                                                                                                            *
 *------------------------------------------------------------------------------------------------------------*
 * PROJECT     :  Common Framework                                                                            *
 * BINARY NAME :  libBase                                                                                     *
 * FILE NAME   :  support/json/EJsonError.h                                                                   *
 * CREATED BY  :  Huah Tu <huah.tu@mic.com.tw>                                                                *
 * CREATED DATE:  06/16/20 (MM/DD/YY)                                                                         *
 * DESCRIPTION :  General JSON exception.                                                                     *
 *------------------------------------------------------------------------------------------------------------*/

#ifndef _SUPPORT_JSON_E_JSON_ERROR_H
#define _SUPPORT_JSON_E_JSON_ERROR_H

// Standard include
#include <exception>

class EJsonError : public std::exception
{
  public:
    virtual const char* what() const throw();
};

#endif//_SUPPORT_JSON_E_JSON_ERROR_H
