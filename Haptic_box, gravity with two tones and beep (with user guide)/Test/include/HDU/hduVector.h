/*****************************************************************************

Copyright (c) 2004 SensAble Technologies, Inc. All rights reserved.

OpenHaptics(TM) toolkit. The material embodied in this software and use of
this software is subject to the terms and conditions of the clickthrough
Development License Agreement.

For questions, comments or bug reports, go to forums at: 
    http://dsc.sensable.com

Module Name:

  hduVector.h

Description: 

  Provides 3D vector mathematics routines.

*******************************************************************************/

#ifndef hduVector_H_
#define hduVector_H_

#include <math.h>
#include "c:\Hao Programming\Haptic_test\Test\include\HD\hdDefines.h"

// #include "c:\Hao Programming\Haptic 1\Haptic 1\include\HD\hduVector.inl"

#ifdef __cplusplus

#include <ostream>

template <class T>
class hduVector3D 
{
public:

    /* Type of each element of hduVector3D. */
    typedef T EltType;

    hduVector3D();
    hduVector3D(T x, T y, T z);
    explicit hduVector3D(const T val[3]);

    template <typename U>			//泛型编程，此处用法同class
    hduVector3D(const hduVector3D<U> &rhs)
    {
        *this = rhs;
    }

    template <typename U>
    inline hduVector3D &operator =(const hduVector3D<U> &rhs)
    {
        m_p[0] = rhs[0];        
        m_p[1] = rhs[1];
        m_p[2] = rhs[2];
        return *this;
    }

    const T &operator[](int i) const;
    T &operator[](int i);

    /* Set from values. */
    void set(T x, T y, T z);

    /* Convert to array. */
    operator const T*() const { return &m_p[0]; }
    operator T*() { return &m_p[0]; }   

    hduVector3D &operator = (const hduVector3D &v1);
    hduVector3D &operator -= (const hduVector3D &v1);
    hduVector3D &operator += (const hduVector3D &v1);
    hduVector3D &operator *= (const hduVector3D &v1);
    hduVector3D &operator *= (HDdouble s);
    hduVector3D &operator /= (const hduVector3D &v1);
    hduVector3D &operator /= (HDdouble s);

    HDboolean isZero(T epsilon) const;
    HDdouble distance(const hduVector3D& v1) const;
    HDdouble distanceSqr(const hduVector3D& v1) const;
    HDdouble magnitude() const;
    void normalize();
    HDdouble dotProduct (const hduVector3D& v1) const;
    hduVector3D crossProduct (const hduVector3D& v1) const;

	int getLongestAxisComponent() const;
    int getSecondLongestAxisComponent() const;
    int getShortestAxisComponent() const;

private:
    T m_p[3];
};

typedef hduVector3D<HDfloat>  hduVector3Df;
typedef hduVector3D<HDdouble> hduVector3Dd;




#else

typedef HDfloat hduVector3Df[3];
typedef HDdouble hduVector3Dd[3];

#endif

#ifdef __cplusplus
extern "C"
{
#endif

__inline void hduVecSet(HDdouble *vec,
                        HDdouble x,
                        HDdouble y,
                        HDdouble z)
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

__inline HDdouble hduVecMagnitude(const HDdouble *vec)			//_inline定义内联函数（即C++一个类之内的函数）
{
    return (sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]));	//计算欧氏距离
}

__inline HDboolean hduVecIsZero(const HDdouble *vec, HDdouble epsilon)//判断矩阵是否比一个给定值要小
{
    return( (fabs(vec[0]) < epsilon) &&
            (fabs(vec[1]) < epsilon) &&
            (fabs(vec[2]) < epsilon) );				//fabs功能：求浮点数的绝对值
}

__inline void hduVecAdd(HDdouble *res,				//两个矩阵相加
                        const HDdouble *vec1, 
                        const HDdouble *vec2)
{
    res[0] = vec1[0] + vec2[0];
    res[1] = vec1[1] + vec2[1];
    res[2] = vec1[2] + vec2[2];
}

__inline void hduVecSubtract(HDdouble *res,			//两个矩阵相减
                             const HDdouble *vec1,
                             const HDdouble *vec2)
{
    res[0] = vec1[0] - vec2[0];
    res[1] = vec1[1] - vec2[1];
    res[2] = vec1[2] - vec2[2];
}

__inline void hduVecScale(HDdouble *res,			//矩阵等比例变化
                          const HDdouble *vec, 
                          HDdouble s)				
{
    res[0] = vec[0] * s;
    res[1] = vec[1] * s;
    res[2] = vec[2] * s;
}

__inline void hduVecScaleInPlace(HDdouble *vec,		//矩阵原地等比例变化
                                 HDdouble s)
{
    vec[0] = vec[0] * s;
    vec[1] = vec[1] * s;
    vec[2] = vec[2] * s;
}

__inline void hduVecScaleNonUniform(HDdouble *res,		//两个矩阵点乘
                                    const HDdouble *vec, 
                                    const HDdouble *s)
{
    res[0] = vec[0] * s[0];
    res[1] = vec[1] * s[1];
    res[2] = vec[2] * s[2];
}

__inline void hduVecScaleNonUniformInPlace(HDdouble *vec,	//矩阵原地点乘另一个矩阵
                                           const HDdouble *s)
{
    vec[0] = vec[0] * s[0];
    vec[1] = vec[1] * s[1];
    vec[2] = vec[2] * s[2];
}

__inline void hduVecNormalize(HDdouble *res,				//将矩阵进行标准化、归一化
                              const HDdouble *vec)
{
    double mag = hduVecMagnitude(vec);
    hduVecScale(res, vec, 1.0 / mag);
}

__inline void hduVecNormalizeInPlace(HDdouble *vec)			//将矩阵进行原地标准化、归一化
{
    double mag = hduVecMagnitude(vec);
    if (mag == 0) return;
    hduVecScaleInPlace(vec, 1.0 / mag);
}

__inline void hduVecCrossProduct(HDdouble *res,				//向量叉乘
                                 const HDdouble *vec1, 
                                 const HDdouble *vec2)
{
    res[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    res[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    res[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

__inline HDdouble hduVecDotProduct(const HDdouble *vec1,	//向量点乘
                                   const HDdouble *vec2)
{
    return (vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]);
}

__inline HDdouble hduVecDistance(const HDdouble *vec1,		//向量间距离
                                 const HDdouble *vec2)
{
    hduVector3Dd vec3;
    hduVecSubtract(vec3,vec1,vec2);
    return hduVecMagnitude(vec3);
}

#ifdef __cplusplus
}
#endif

#endif  /* hduVector_H_ */

/*****************************************************************************/
