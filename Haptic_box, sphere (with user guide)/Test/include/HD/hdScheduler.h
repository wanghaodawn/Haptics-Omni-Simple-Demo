/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

    hdScheduler.h
    
Description: 

    Thread management utilities for cross thread synchronization

******************************************************************************/

#ifndef HD_SCHEDULER_H_DEFINE
#define HD_SCHEDULER_H_DEFINE

#include "hdExport.h"
#include "hdCompilerConfig.h"
#include "hdDefines.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/* Return code for callbacks, determines whether the operation is finished
   and will unscheduled, or whether it will continue running. 
   返回代码，决定是否当前操作完成，即将进行调度，或者继续运行*/
typedef unsigned int HDCallbackCode;
#define HD_CALLBACK_DONE     0
#define HD_CALLBACK_CONTINUE 1

/* Scheduler operation handle. */
typedef unsigned long HDSchedulerHandle;

/* Callback to scheduler operations. */       
typedef HDCallbackCode (HDCALLBACK *HDSchedulerCallback)(void *pUserData); 

/* WaitForCompletion codes. 
 CHECK_FOR_STATUS returns whether the scheduler operation for
  the given handle is still scheduled. 
 INFINITE waits for the scheduler operation to complete. 
	 等待完成码 等待调度器完成*/
typedef unsigned int HDWaitCode;
#define HD_WAIT_CHECK_STATUS 0
#define HD_WAIT_INFINITE     1

/* Scheduler functions.  Start, stop, or set rate of the scheduler. 
	调度器函数，开始、停止、设置调度函数每秒标记回调函数的次数*/
HDAPI void HDAPIENTRY hdStartScheduler();
HDAPI void HDAPIENTRY hdStopScheduler();
HDAPI void HDAPIENTRY hdSetSchedulerRate(HDulong nRate);

/* 调度一个函数并等待至完成 Schedule a function to run in the scheduler thread and wait for completion. 
   第一个参数指向将要运行的函数 pCallback is the function to be run.
   第二个参数是将要传过去的函数 pUserData is user data to be passed into pCallback.
   第三个参数是调度优先级 nPriority is the scheduling priority, which determines what order the 
   callbacks are run when multiple callbacks are scheduled (higher priority means 
    run first). */
HDAPI void HDAPIENTRY hdScheduleSynchronous(HDSchedulerCallback pCallback, 
                                            void *pUserData, 
                                            HDushort nPriority);
    
/* 调度一个函数，无需等待其完成，回调函数直接运行，不论其优先级怎样
Schedule a function to run in the scheduler, do not wait for its completion.  
   Returned handle is used to unschedule or block waiting for completion. */
HDAPI HDSchedulerHandle HDAPIENTRY hdScheduleAsynchronous(
                                            HDSchedulerCallback pCallback, 
                                            void *pUserData, 
                                            HDushort nPriority);

/* 从调度程序移除相关回调函数，从而取消预订某操作 Unschedule the operation identified by the handle. */
HDAPI void HDAPIENTRY hdUnschedule(HDSchedulerHandle hHandle);

/* 检查回调函数是否仍被预定执行
	Wait until the operation associated with the handle is completed. 
   If HD_WAIT_CHECK_STATUS, returns true if the operation associated
   with the handle is still scheduled */
HDAPI HDboolean HDAPIENTRY hdWaitForCompletion(HDSchedulerHandle hHandle,
                                               HDWaitCode param);

/* 用于检查操作时间 Get the time in seconds since the start of the frame. */
HDAPI HDdouble HDAPIENTRY hdGetSchedulerTimeStamp();

#ifdef __cplusplus
}
#endif

#endif /* HD_SCHEDULER_H_DEFINE */

/******************************************************************************/


