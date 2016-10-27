//编译出错改configuration里的common run language support!
//Properties属性，在C/C++ -> CodeGeneration下找到Basic Runtime Checks 属性，改为：Default
//任务调度会出现问题，只用一个任务比较好
//子函数要慎重使用，容易作用不了

/*************************************************************
	A box and one dynamic gravity well and one static gravity well
	--> Finished by Hao wang 2014.08
*************************************************************/

#pragma warning (disable:4996)//用于屏蔽警告4996，即 getch()

#include <iostream>		//要包含这个头文件，要不然会出现链接错误
#include <stdio.h>
#include <assert.h>		//防御式编程，可以输出错误代码

#if defined(WIN32)
# include <windows.h>	
# include <conio.h>		//Console Input/Output控制台输入/输出的简写
#else
# include "conio.h"
# include <string.h>
#endif

#include "hd\hd.h"

#include <HDU\hduError.h>         

#include <HDU\hduVector.h>


/***************************************************************/
/*****************Parameter modification Area*******************/
// The parameter of the box
#define EdgeX 100	// The edge of Box_x
#define EdgeY 0		// The edge of Box_y	(Had better not change this, or there might be potential problems)
#define EdgeZ 60	// The edge of Box_z
#define PlainStiffness 1	//The stiffness of the box
#define PopthroughForceThreshold 30	//The force needed to pop through

#define GravityWellSpeed 5		//The speed of Gravity Well's movement

//The parameter of the Gravity well 1
#define GravityWellStiff1 0.075	//The Stiffness of Gravity Well 1
#define GravityWellRadius1 20	//The Radius of Gravity Well 1
#define GravityWell1XRange 50	//The Range of Gravity Well 1 X
#define GravityWell1PosY 0		//Define Y parameter of Gravity Well 1
#define GravityWell1PosZ 0		//Define Z parameter of Gravity Well 1

//The parameter of the Gravity well 2
#define GravityWellStiff2 0.1	//The Stiffness of Gravity Well 2
#define GravityWellRadius2 10	//The Radius of Gravity Well 2
#define GravityWell2PosX 80		//Define X parameter of Gravity Well 2
#define GravityWell2PosY 0	//The Range of Gravity Well 2 Y
#define GravityWell2PosZ 20		//Define Z parameter of Gravity Well 2
/***************************************************************/
/***************************************************************/


hduVector3Dd position;			//The position of Mouse
static double directionFlag[3] = {0};	// Mark the direction the mouse faces
    // Plane direction changes whenever the user applies sufficient
    // force to popthrough it.
    // 1 means the plane is facing +Y. 设备用从下到上的力量创建平面
    // -1 means the plane is facing -Y.设备用从上到下的力量创建平面

//int store[12000][2] = {0};		//该矩阵用来存储黑线的横纵坐标值

int FlagEdge = 0;					//标记是否计算过图形的中心
hduVector3Dd wellPos3;				//不规则形状的中心
hduVector3Dd positionTwell3;		//不规则形状的距离

HDdouble GravityWell1X = - GravityWell1XRange;	//X of GravityWell1
HDdouble Direction1X = 1;			//1 increase; -1 decrease
int delay = 0;
/*******************************************************
                          伪主函数
*******************************************************/
HDCallbackCode HDCALLBACK functionCallback(void *data)
{
	// Stiffnes, i.e. k value, of the plane.  Higher stiffness results
    // in a harder surface.pingm 平面的硬度
	static const double planeStiffness = PlainStiffness;

    // Amount of force the user needs to apply in order to pop through
    // the plane. 使用者穿透平面需要的力量
	static const double popthroughForceThreshold = PopthroughForceThreshold;

	const HDdouble kStiffness1 = GravityWellStiff1; /* N/mm */			//刚度(弹性系数)
	const HDdouble kGravityWellInfluence1 = GravityWellRadius1; /* mm */		//定义gravity well的作用半径
	const HDdouble kStiffness2 = GravityWellStiff2; /* N/mm */			//刚度(弹性系数)
	const HDdouble kGravityWellInfluence2 = GravityWellRadius2; /* mm */		//定义gravity well的作用半径

	double penetrationDistance;
	double k;
	hduVector3Dd x;
	hduVector3Dd f;

	if(delay < GravityWellSpeed)
		delay ++;
	else
	{
		delay = 0;
		if(Direction1X == 1 && GravityWell1X < GravityWell1XRange)
		{
			GravityWell1X ++;
		}
		else if(Direction1X == 1 && GravityWell1X == GravityWell1XRange)
		{
			GravityWell1X --;
			Direction1X = -1;
		}
		if(Direction1X == -1 && GravityWell1X > -GravityWell1XRange)
		{
			GravityWell1X --;
		}
		else if(Direction1X == -1 && GravityWell1X == -GravityWell1XRange)
		{
			GravityWell1X ++;
			Direction1X = 1;
		}
	}

	/* This is the position of the gravity well in cartesian
    (i.e. x,y,z) space. */
    hduVector3Dd wellPos1;			//定义gravity well的位置 Position
	hduVecSet(wellPos1,GravityWell1X,GravityWell1PosY,GravityWell1PosZ);
	hduVector3Dd wellPos2;			//定义gravity well的位置 Position
	hduVecSet(wellPos2,GravityWell2PosX,GravityWell2PosY,GravityWell2PosZ);

	HDErrorInfo error;
    hduVector3Dd force;
    hduVector3Dd positionTwell1;
    hduVector3Dd positionTwell2;

	hdBeginFrame(hdGetCurrentDevice());

	hdGetDoublev(HD_CURRENT_POSITION, position);	//得到现在设备所处的位置

	memset(force, 0, sizeof(hduVector3Dd));	//清零操作
	memset(f, 0, sizeof(hduVector3Dd));		//清零操作
    
    // >  positionTwell = wellPos-position  
    hduVecSubtract(positionTwell1, wellPos1, position);// 计算 positionTwell = wellPos-position
    hduVecSubtract(positionTwell2, wellPos2, position);// 计算 positionTwell = wellPos-position

/********************************************************************************************************/
	//检测X是否超出范围
	if ((position[0] >= EdgeX) || (position[0] <= -EdgeX))
    {
		penetrationDistance = fabs(position[0]) - EdgeX;

		//judege the foce's direction
		if(position[0] >= EdgeX)
			directionFlag[0] = -1;
		else if(position[0] <= -EdgeX)
			directionFlag[0] = 1;
		else
			directionFlag[0] = 0;
	
		if(position[1] < EdgeY)
			directionFlag[1] = 1;
		else
			directionFlag[1] = 0;
	
		if(position[2] >= EdgeZ)
			directionFlag[2] = -1;
		else if(position[2] <= -EdgeZ)
			directionFlag[2] = 1;
		else
			directionFlag[2] = 0;	

		hduVector3Dd forceDirection(directionFlag[0],directionFlag[1],directionFlag[2]);

        // Hooke's law explicitly:
        k = planeStiffness;
		x = forceDirection;
        x *= penetrationDistance; // 类型不匹配，此处使用操作符重载
		f = x;
        f *= k;

        if (f.magnitude() > popthroughForceThreshold)//保护
        {
            f.set(0.0,0.0,0.0);
        }

        hdSetDoublev(HD_CURRENT_FORCE, f);
    }

	// 检测Z是否超出地图范围
	if ((position[2] >= EdgeZ) || (position[2] <= -EdgeZ))
    {
		penetrationDistance = fabs(position[2]) - EdgeZ;

		//judege the foce's direction
		if(position[0] >= EdgeX)
			directionFlag[0] = -1;
		else if(position[0] <= -EdgeX)
			directionFlag[0] = 1;
		else
			directionFlag[0] = 0;
			
		if(position[1] < EdgeY)
			directionFlag[1] = 1;
		else
			directionFlag[1] = 0;
			
		if(position[2] >= EdgeZ)
			directionFlag[2] = -1;
		else if(position[2] <= -EdgeZ)
			directionFlag[2] = 1;
		else
			directionFlag[2] = 0;	

		hduVector3Dd forceDirection(directionFlag[0],directionFlag[1],directionFlag[2]);

        // Hooke's law explicitly:
        k = planeStiffness;
		x = forceDirection;
        x *= penetrationDistance; // 类型不匹配，此处使用操作符重载
		f = x;
        f *= k;

        if (f.magnitude() > popthroughForceThreshold)
        {
            f.set(0.0,0.0,0.0);
        }

        hdSetDoublev(HD_CURRENT_FORCE, f);
    }

	// 此处创建MAP区域
    if ((position[1] <= EdgeY && position[0] <= EdgeX && position[0] >= -EdgeX && position[2] <= EdgeZ && position[2] >= -EdgeZ) || 
        (position[1] >= EdgeY && position[0] <= EdgeX && position[0] >= -EdgeX && position[2] <= EdgeZ && position[2] >= -EdgeZ))
    {
		penetrationDistance = fabs(position[1]) - EdgeY;

		//judege the foce's direction
		if(position[0] >= EdgeX)
			directionFlag[0] = -1;
		else if(position[0] <= -EdgeX)
			directionFlag[0] = 1;
		else
			directionFlag[0] = 0;

		if(position[1] < EdgeY)
			directionFlag[1] = 1;
		else
			directionFlag[1] = 0;

		if(position[2] >= EdgeZ)
			directionFlag[2] = -1;
		else if(position[2] <= -EdgeZ)
			directionFlag[2] = 1;
		else
			directionFlag[2] = 0;	

		hduVector3Dd forceDirection(directionFlag[0],directionFlag[1],directionFlag[2]);

        // Hooke's law explicitly:
        k = planeStiffness;
		x = forceDirection;
        x *= penetrationDistance; // 类型不匹配，此处使用操作符重载
		f = x;
        f *= k;

        // If the user applies sufficient force, pop through the plane
        // by reversing its direction.  Otherwise, apply the repel
        // force.
        if (f.magnitude() > popthroughForceThreshold)
        {
            f.set(0.0,0.0,0.0);
        }
		
        hdSetDoublev(HD_CURRENT_FORCE, f);
    }
/********************************************************************************************************/	
	// Two Gravity Wells
	if (hduVecMagnitude(positionTwell1) < kGravityWellInfluence1)//判断是否进入了gravity well的作用范围
    {
        // >  F = k * x  
        hduVecScale(force, positionTwell1, kStiffness1);
		hdSetDoublev(HD_CURRENT_FORCE, force);
    }
	if(hduVecMagnitude(positionTwell2) < kGravityWellInfluence2)
	{
        // >  F = k * x  
        hduVecScale(force, positionTwell2, kStiffness2);		
		hdSetDoublev(HD_CURRENT_FORCE, force);
	}
/********************************************************************************************************/	
	hdEndFrame(hdGetCurrentDevice());

    // In case of error, terminate the callback.
    if (HD_DEVICE_ERROR(error = hdGetError())) 
    {
        hduPrintError(stderr, &error, "Error detected during main scheduler callback\n");

        if (hduIsSchedulerError(&error))
        {
            return HD_CALLBACK_DONE;  
        }
    }

    return HD_CALLBACK_CONTINUE;
}


/*******************************************************************************
 * main function
   Initializes the device, creates a callback to handles plane forces, 
   terminates upon key press.
 ******************************************************************************/
int main(int argc, char* argv[])
{
//    int keypress;
	HDErrorInfo error;
//	HDSchedulerHandle hGravityWell;

    // Initialize the default haptic device.
    HHD hHD = hdInitDevice(HD_DEFAULT_DEVICE);
    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to initialize haptic device");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }

    if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Failed to start the scheduler");
        fprintf(stderr, "\nPress any key to quit.\n");
        getch();
        return -1;
    }

	 HDSchedulerHandle hFunction = hdScheduleAsynchronous(
        functionCallback, (void*)0,				//注意此处应该用的表达方式 (void*)
        HD_MAX_SCHEDULER_PRIORITY);
		   
	printf("\n");
    printf("Move the device up and down to feel the box and the gravity well.\n");
    printf("Push hard against the plane to popthrough to the other side.\n");
    printf("\n");
	printf("Press any key to quit.\n\n");

    // Start the servo scheduler and enable forces.
    hdEnable(HD_FORCE_OUTPUT);
    hdStartScheduler();

	while (!_kbhit())
    {       
        if (!hdWaitForCompletion(hFunction, HD_WAIT_CHECK_STATUS))
        {
            fprintf(stderr, "\nThe main scheduler callback has exited\n");
            fprintf(stderr, "\nPress any key to quit.\n");
            getch();
            break;
        }
    }

    // Cleanup and shutdown the haptic device, cleanup all callbacks.
    hdStopScheduler();
    hdUnschedule(hFunction);		//调度器取消预定某函数
	hdDisableDevice(hHD);

    return 0;
}
