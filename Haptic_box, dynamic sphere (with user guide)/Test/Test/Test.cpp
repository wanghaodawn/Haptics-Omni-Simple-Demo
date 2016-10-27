//编译出错改configuration里的common run language support!
//Properties属性，在C/C++ -> CodeGeneration下找到Basic Runtime Checks 属性，改为：Default
//任务调度会出现问题，只用一个任务比较好

//#ifdef  _WIN64
#pragma warning (disable:4996)//用于屏蔽警告4996，即 getch()
//#pragma comment(lib, "C:\OpenHaptics\Academic\3.2\lib\Win32\DebugAcademicEdition\hd.lib")
//#pragma comment(lib, "C:\OpenHaptics\Academic\3.2\lib\Win32\DebugAcademicEdition\hl.lib")
//#endif


/******************************************************************
		A box with a dynamic sphere inside

		User Guide	--> by Hao Wang 2014.08
******************************************************************/


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

/*****************Parameter modification Area************/
#define EdgeX 100	// The edge of Map_x
#define EdgeY 0		// The edge of Map_y (Height)
#define EdgeZ 60	// The edge of Map_z

#define SphereSpeed 50	//The speed of dynamic sphere (The small the faster)
#define OrbitEdge 70	//The edge of square orbit

double SphereY = 60;	// Y parameter of Sphere (Height)
double SphereZ = 0;	// Z parameter of Sphere
const double sphereRadius = 55.0;		// The radius of the Sphere
const double sphereStiffness = 0.25;		// The Stiffness of the Sphere
/********************************************************/
//void mainLoop();

hduVector3Dd position;			//The position of Mouse
static int directionFlag[3] = {0};	// Mark the direction the mouse faces
    // Plane direction changes whenever the user applies sufficient
    // force to popthrough it.
    // 1 means the plane is facing +Y. 设备用从下到上的力量创建平面
    // -1 means the plane is facing -Y.设备用从上到下的力量创建平面

int delay = 0;
int direction = 1;	// 1 x+; -1 x-

HDdouble SphereX = -OrbitEdge;	// X parameter of Sphere
/*******************************************************
                          伪主函数
*******************************************************/
HDCallbackCode HDCALLBACK functionCallback(void *data)
{
	// Stiffnes, i.e. k value, of the plane.  Higher stiffness results
    // in a harder surface.pingm 平面的硬度
	static const double planeStiffness = 1;

    // Amount of force the user needs to apply in order to pop through
    // the plane. 使用者穿透平面需要的力量
	static const double popthroughForceThreshold = 30;

	double penetrationDistance;
	double k;
	hduVector3Dd x;
	hduVector3Dd f;

	if(delay < SphereSpeed)
		delay ++;
	else
	{
		delay = 0;
		if(direction == 1 && SphereX < OrbitEdge)
		{
			SphereX ++;
		}
		else if(direction == 1 && SphereX == OrbitEdge)
		{
			direction = -1;
			SphereX --;
		}
		else if(direction == -1 && SphereX > -OrbitEdge)
		{
			SphereX --;
		}
		else if(direction == -1 && SphereX == -OrbitEdge)
		{
			direction = 1;
			SphereX ++;
		}
	}

	HDErrorInfo error;

	hdBeginFrame(hdGetCurrentDevice());

	hdGetDoublev(HD_CURRENT_POSITION, position);	//得到现在设备所处的位置

	memset(f, 0, sizeof(hduVector3Dd));	//清零操作

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
	const hduVector3Dd spherePosition(SphereX,SphereY,SphereZ);

    double distance = (position-spherePosition).magnitude();

    if (distance < sphereRadius)
    {
        // Calculate the penetration distance.
        double penetrationDistance1 = sphereRadius-distance;

        hduVector3Dd forceDirection = (position-spherePosition)/distance; //确定力的方向

        double k = sphereStiffness;
        hduVector3Dd x = penetrationDistance1*forceDirection;
        hduVector3Dd f = k*x;
        hdSetDoublev(HD_CURRENT_FORCE, f);
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
