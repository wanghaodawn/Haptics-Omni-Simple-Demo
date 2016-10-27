/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com
 
Module Name:

  hduError.h


Description: 

  Helper routines for print error messages and error handling with the HDAPI.

*******************************************************************************/

#ifndef hduError_H_
#define hduError_H_

#include "c:\Hao Programming\Haptic_test\Test\include\HD\hdDefines.h"

/******************************************************************************
 C++ Error handling helper routines.
******************************************************************************/

#ifdef __cplusplus

#include <ostream>

/******************************************************************************
 Pretty prints a message containing the error code name, the error code and
 the internal error code.
******************************************************************************/
std::ostream &operator<<(std::ostream &os, const HDErrorInfo &error);

#endif /* __cplusplus */

/******************************************************************************
 C Error handling helper routines.
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/******************************************************************************
 打印出错误名称，错误代码，内部错误代码
 Pretty prints a message containing the error code name, the error code and
 the internal error code.
******************************************************************************/
void hduPrintError(FILE *stream, const HDErrorInfo *error, const char *message);

/******************************************************************************
 指出是否有一个错误与Haptic的渲染有关
 Indicates whether this is a force error related to haptic rendering.  Typical
 handling of a force error involves resetting contact or simulation state.
******************************************************************************/
HDboolean hduIsForceError(const HDErrorInfo *error);

/******************************************************************************
 指出是否错误与通信、定时或通常的调度器错误有关
 Indicates whether this is a communication, timing or general scheduler error
 that would prevent proper operation of the servo loop
******************************************************************************/
HDboolean hduIsSchedulerError(const HDErrorInfo *error);

#ifdef __cplusplus
}
#endif

#endif // hduError_H_

/*****************************************************************************/
