//
// Copyright 2008, SensAble Technologies, Inc.
//
// File: Box.h
//
// Author: Hari Vasudevan	
//
// Created: 09/11/2008
//
// Description: Declares the Box Class primitive with all associated 
// parameters.
//
//---------------------------------------------------------------------

#ifndef BOX_H
#define BOX_H


#include "ExportDef.h"
#include "Shape.h"


/*!\brief This class to creates a Box primitive.

	Use this class to load a Box. The class generates a display list for the Box and then
	supplies display list to the parent shape class.
*/

class QHAPI Box: public Shape
{
	///Virtual Function to great a display list for a box
	virtual void PureGLBox(HLdouble m_Length, HLdouble m_Width, HLdouble m_Height, hduVector3Dd m_Position);
	///Box parameters 长
	HLdouble m_Length;
	///Box parameters 宽
	HLdouble m_Width; 
	///Box parameters 高
	HLdouble m_Height;
		
	///This variable holds the X translation of the box X方向平移
	HLdouble m_PositionX;
	///This variable holds the Y translation of the box Y方向平移
	HLdouble m_PositionY;
	///This variable holds the Z translation of the box Z方向平移
	HLdouble m_PositionZ;

	public:
		
	
		/*! \brief Default Constructor
		
		
		Constructs a Box with the following parameters:\n
		L = 0.5\n
		W = 0.5\n
		H = 0.5\n
		Centered at the origin
		创建默认大小的盒子 */
		Box(void);
		/*! \brief Constructor with the length, width and height specified by the user
		
		Constructs a Box with the following parameters:\n
		L = user-defined\n
		W = user-defined\n
		H = user-defined\n
		Centered at the origin
		创建给定长宽高的盒子 */
		Box(HLdouble m_Length, HLdouble m_Width, HLdouble m_Height);
		/*! \brief Constructor with m_Width, m_Height and translations

		Constructs a Box with the following parameters:\n
		L = user-defined\n
		W = user-defined\n
		H = user-defined\n
		Centered at a position determined by the user
		创建给定长宽高与一定位置的盒子 */
		Box(HLdouble m_Length, HLdouble m_Width, HLdouble m_Height, HLdouble m_PositionX, HLdouble m_PositionY, HLdouble m_PositionZ);

		/*! \brief The update functions act just like the constructor, except that they can be invoked anywhere in the program

		The update function is used to update the Box. The default update function is used to create a Box of size:\n
		L = 0.5\n
		W = 0.5\n
		H = 0.5\n
		Centered at the origin
		更新默认大小的盒子 */
		void update(void);
		/*! \brief The update functions act just like the constructors, except that they can be invoked anywhere in the program

		The update function is used to update the Box. This update function is used to create a Box of size:\n
		L = user-defined\n
		W = user-defined\n
		H = user-defined\n
		Centered at the origin
		更新给定长宽高的盒子 */
		void update(HLdouble m_Length, HLdouble m_Width, HLdouble m_Height);
		/*! \brief The update functions act just like constructors, except that they can be invoked anywhere in the program

		The update function is used to update the Box. This update function is used to create a Box of size:\n
		L = user-defined\n
		W = user-defined\n
		H = user-defined\n
		Centered at a position determined by the user
		更新给定长宽高与一定位置的盒子 */
		void update(HLdouble m_Length, HLdouble m_Width, HLdouble m_Height, HLdouble m_PositionX, HLdouble m_PositionY, HLdouble m_PositionZ);

		/*! \brief This function is used to search for a Box by a name and return a pointer to the Box Object if found.
	
		The name is assigned by the QHRenderer::setName function. If the function does not find a match it will return a NULL pointer.
		It is the responsibility of the user to detect the NULL return and handle it in a safe manner*/
		static Box* searchBox(char* BoxName);
		///利用名字来寻找盒子 Search for box by name	
		
		/*! \brief This function is used to search for a Box object by a ShapeID and return a pointer to the Box if found.
	
		A shapeID is assigned to every haptic object by OpenHaptics. This is a unique number identifying
		the object under a fixed rendering context. This function can be used in events like "touch" or "untouch" etc... 
		which can be setup to invoke the callback functions and pass the ShapeID.
		The search function with the ShapeID can then be used to return a pointer to the primitive shape.
		If the function does not find a match it will return a NULL pointer.
		It is the responsibility of the user to detect the NULL return and handle it in a safe manner*/
		static Box* searchBox(HLuint BoxID);
		///利用Haptic ID来寻找盒子 Search for box by haptic shape id

		~Box(void);//Destructor for the Box class
        ///This function returns the dimensions of the box
        hduVector3Dd getLengthWidthHeight(void);
};

#endif