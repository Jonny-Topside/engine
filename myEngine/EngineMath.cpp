/**
* @file EngineMath.cpp
*
*/
#include "stdafx.h"
#include "EngineMath.h"

//////////////////////////////////////////////////////////////////////////
// Common math functions
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// General Utility functions
//////////////////////////////////////////////////////////////////////////

// Are two floating point numbers equal to each other
// Floating Point Error Safe
//
// IN:		a		The first number
//			b		The second number
//
// RETURN: TRUE iff |a-b| < Tolerance
//
// NOTE:	EPSILON is tolerance
bool IsEqual(float a, float b)
{

	// NOTE: Do not modify.
	return fabs(a - b) < EPSILON;
}

// Is a floating point value equal to zero
// Floating Point Error Safe
//
// IN:		a		The number to check
//
// RETURN:	TRUE iff |a| < Tolerance
//
// NOTE:	Tolerance set by EPSILON
bool IsZero(float a)
{
	// NOTE: Do not modify
	return (fabs(a)) < EPSILON;
}

// RETURN: MAX of two numbers
float Max(float a, float b)
{
	// NOTE: Do not modify.
	return (a > b) ? a : b;
}

// RETURN: MIN of two numbers
float Min(float a, float b)
{
	// NOTE: Do not modify.
	return (a < b) ? a : b;
}

// RETURN: Converts input to radian measure
float Degrees_To_Radians(float Deg)
{
	// NOTE: Do not modify.
	return Deg * PI / 180.0f;
}

// RETURN: Converts input to degree measure
float Radians_To_Degrees(float Rad)
{
	// NOTE: Do not modify.
	return Rad * 180.0f / PI;
}
////////////////////////////////////////////////////////////////////////
// Linear Algebra Functions Day 1
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// Vector Functions
//////////////////////////////////////////////////////////////////////////

// Check if two TVECTOR's are equal to each other
//
// IN:		v		First Vector
//			w		Second Vector
//
// RETURN:  True if v==w, False otherwise
//
// NOTE:	Use's all four components
//			Should be floating point error safe.
bool Vector_IsEqual(TVECTOR v, TVECTOR w)
{
	if (v.x == w.x && v.y == w.y && v.z == w.z && v.z == w.z)
	{
		return true;
	}
	else
	{
		return false;
	}
	// TODO LAB 1: Replace with your implementation.
}

// ADD two TVECTOR's togother
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v + w
//
// NOTE:	Use's all four components
TVECTOR Vector_Add(TVECTOR v, TVECTOR w)
{
	// TODO LAB 1: Replace with your implementation.
	TVECTOR addVec;
	addVec.x = v.x + w.x;
	addVec.y = v.y + w.y;
	addVec.z = v.z + w.z;
	addVec.w = v.w + w.w;
	return addVec;
}

// SUBTRACT one TVECTOR from another
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v - w
//
// NOTE:	Use's all four components
TVECTOR Vector_Sub(TVECTOR v, TVECTOR w)
{
	TVECTOR subvec;
	subvec.x = v.x - w.x;
	subvec.y = v.y - w.y;
	subvec.z = v.z - w.z;
	subvec.w = v.w - w.w;
	// TODO LAB 1: Replace with your implementation.
	return subvec;
}

// MULTIPLY all four components of a TVECTOR by a scalar
//
// IN:		v		The vector to scale
//			s		The value to scale by
//
// RETURN:  s * v
TVECTOR Vector_Scalar_Multiply(TVECTOR v, float s)
{
	v.x = v.x * s;
	v.y = v.y * s;
	v.z = v.z * s;
	v.w = v.w * s;
	// TODO LAB 1: Replace with your implementation.
	return v;
}

// NEGATE all the components of a TVECTOR
//
// IN:		v		The vector to negate
//
// RETURN:	-1 * v
//
// NOTE:	Use's all four components
TVECTOR Vector_Negate(TVECTOR v)
{
	v.x = -v.x;
	v.y = -v.y;
	v.z = -v.z;
	v.w = -v.w;
	// TODO LAB 1: Replace with your implementation.
	return v;
}

// Perform a Dot Product on two TVECTOR's
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v (DOT) w
//
// NOTE:	Use's all four components
float Vector_Dot(TVECTOR v, TVECTOR w)
{
	float a, b, c, d;

	a = v.x * w.x;
	b = v.y * w.y;
	c = v.z * w.z;
	d = v.w * w.w;
	float dotted = a + b + c + d;

	// TODO LAB 1: Replace with your implementation.
	return  dotted;
}

// Perform a Cross Product on two TVECTOR's
//
// IN:		v		First Vector. Left Hand Side
//			w		Second Vector. Right Hand Side
//
// RETURN:  v (CROSS) w
//
// NOTE:	The w-component of each vector is not used.
//			The resultant vector will have a w-component of zero.
TVECTOR Vector_Cross(TVECTOR v, TVECTOR w)
{
	TVECTOR result;
	result.x = v.y * w.z - v.z * w.y;
	result.y = v.z * w.x - v.x * w.z;
	result.z = v.x * w.y - v.y * w.x;
	// TODO LAB 1: Replace with your implementation.
	result.w = 0;
	return result;
}

// Find the squared length of a TVECTOR
//
// IN:		v		The vector to find the squared length of
//
// RETURN:	Squared Length of TVECTOR
//
// NOTE:	Use's all four components
float Vector_LengthSq(TVECTOR v)
{
	v.x = v.x * v.x;
	v.y = v.y * v.y;
	v.z = v.z * v.z;
	v.w = v.w * v.w;
	float addEm = v.x + v.y + v.z + v.w;
	// TODO LAB 1: Replace with your implementation.
	return sqrt(addEm * addEm);
}

// Find the length of a TVECTOR
//
// IN:		v		The vector to find the length of
//
// RETURN:	Length of TVECTOR
//
// NOTE:	Use's all four components
float Vector_Length(TVECTOR v)
{
	v.x = v.x * v.x;
	v.y = v.y * v.y;
	v.z = v.z * v.z;
	v.w = v.w *  v.w;
	// TODO LAB 1: Replace with your implementation.
	return sqrt(v.x + v.y + v.z + v.w);
}

// Normalize a TVECTOR
//
// IN:		v		The vector to normalize
//
// RETURN:	Normalized version of v
//
// NOTE:	Use's all four components
TVECTOR Vector_Normalize(TVECTOR v)
{
	float norm;
	float x, y, z, w;
	x = pow(v.x, 2);
	y = pow(v.y, 2);
	z = pow(v.z, 2);
	w = pow(v.w, 2);
	norm = sqrt(x + y + z + w);
	v.x = v.x / norm;
	v.y = v.y / norm;
	v.z = v.z / norm;
	v.w = v.w / norm;
	// TODO LAB 1: Replace with your implementation.
	return v;
}

// Makes a TVECTOR's w-component normalized
//
// IN:		v		The vector (point object) to homogenise
//
// RETURN:	The homogenised vector (point)
//
// NOTE:	If the w-component of the vector is 0 then the
//			function will return a zero vector with a w-component
//			of 0.
TVECTOR Vector_Homogenise(TVECTOR v)
{
	v.x = v.x / v.w;
	v.y = v.y / v.w;
	v.z = v.z / v.w;
	v.w = v.w / v.w;
	// TODO LAB 1: Replace with your implementation.
	if (IsZero(v.w))
	{
		TVECTOR zero;
		zero.x = 0;
		zero.y = 0;
		zero.z = 0;
		zero.w = 0;
		return zero;
	}
	return v;
}

// Get a TVECTOR made from the maximun components of two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A maximized vector
//
// NOTE:	Use's all four components
TVECTOR Vector_Maximize(TVECTOR v, TVECTOR w)
{
	TVECTOR max;
	max.x = Max(v.x, w.x);
	max.y = Max(v.y, w.y);
	max.z = Max(v.z, w.z);
	max.w = Max(v.w, w.w);
	// TODO LAB 1: Replace with your implementation.
	return max;
}

// Get a TVECTOR made from the minimum components of two TVECTOR's
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A minimum vector
//
// NOTE:	Use's all four components
TVECTOR Vector_Minimize(TVECTOR v, TVECTOR w)
{
	TVECTOR min;
	min.x = Min(v.x, w.x);
	min.y = Min(v.y, w.y);
	min.z = Min(v.z, w.z);
	min.w = Min(v.w, w.w);
	// TODO LAB 1: Replace with your implementation.
	return min;
}

// Get a TVECTOR made from the average of two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:	A vector made from the average of two vectors
//
// NOTE:	Use's all four components

TVECTOR Vector_Average(TVECTOR v, TVECTOR w)
{
	TVECTOR temp;
	temp.x = (v.x + w.x) / 2;
	temp.y = (v.y + w.y) / 2;
	temp.z = (v.z + w.z) / 2;
	temp.w = (v.w + w.w) / 2;
	// TODO LAB 1: Replace with your implementation.
	return temp;
}

// Find the angle between two TVECTORs
//
// IN:		v		The first vector
//			w		The second vector
//
// RETURN:  The angle in degrees between the two vectors
//
// NOTE:	If either vector is a zero vector then the return
//			value will be 0.
float Vector_AngleBetween(TVECTOR v, TVECTOR w)
{
	//if ((v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0) || (w.x == 0 && w.y == 0 && w.z == 0 && w.w == 0))
	//{
	//	return 0;
	//}
	float theta = acosf((Vector_Dot(v, w)) / (Vector_Length(v) * Vector_Length(w)));
	theta = Radians_To_Degrees(theta);
	// TODO LAB 1: Replace with your implementation.
	return theta;
}

// Get the distance one TVECTOR points in the direction of another
// TVECTOR
//
// IN:		v		The first vector
//			w		The direction of the component
//
// RETURN:	The distance that v points in the direction of w.
//
// NOTE:	If w or v is a zero vector then the return value is zero.
float Vector_Component(TVECTOR v, TVECTOR w)
{
	float component = Vector_Dot(v, w) / Vector_Length(w);
	// TODO LAB 1: Replace with your implementation.

	if ((w.x == 0 && w.y == 0 && w.z == 0 && w.w == 0) || (v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0))
	{
		return 0;
	}
	return component;
}

// Get the TVECTOR that represents v projected on w.
//
// IN:		v		The first vector
//			w		The direction of the projection
//
// RETURN:	The projection of v onto w
//
// NOTE:	If w or v is a zero vector then the return value is zero.
TVECTOR Vector_Project(TVECTOR v, TVECTOR w)
{
	float component = Vector_Component(v, w);
	w = Vector_Normalize(w);
	w.x = component * w.x;
	w.y = component * w.y;
	w.z = component * w.z;
	w.w = component * w.w;

	if ((w.x == 0 && w.y == 0 && w.z == 0 && w.w == 0) || (v.x == 0 && v.y == 0 && v.z == 0 && v.w == 0))
	{
		TVECTOR zero;
		zero.x = 0;
		zero.y = 0;
		zero.z = 0;
		zero.w = 0;
		return zero;
	}
	// TODO LAB 1: Replace with your implementation.
	return w;
}

////////////////////////////////////////////////////////////////////////
// Functions Lab  #2
///////////////////////////////////////////////////////////////////////


// Get the reflection of v across w
//
// IN:		v		The vector to reflect
//			w		The "axis" to reflect across
//
// RETURN:	v reflected across w
//
// NOTE:	If w is a zero vector then return -v.
TVECTOR Vector_Reflect(TVECTOR v, TVECTOR w)
{

	TVECTOR proj = Vector_Project(v, w);
	TVECTOR proj2;
	proj2.x = (proj.x * 2) - v.x;
	proj2.y = (proj.y * 2) - v.y;
	proj2.z = (proj.z * 2) - v.z;
	proj2.w = (proj.w * 2) - v.w;

	if (w.x == 0 && w.y == 0 && w.z == 0 && w.w == 0)
	{
		v.x = -v.x;
		v.y = -v.y;
		v.z = -v.z;
		v.w = -v.w;
		return v;
	}

	// TODO LAB 2: Replace with your implementation.
	return proj2;
}

//////////////////////////////////////////////////////////////////////////
// Matrix Functions
//////////////////////////////////////////////////////////////////////////

// Get a [0] matrix
//
// RETURN: A 0 4x4 matrix
TMATRIX Matrix_Zero(void)
{
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m;
	for (int i = 0; i < 16; i++)
	{
		m.e[i] = 0;
	}

	return m;
}

// Get a [I] matrix
//
// RETURN: A 4x4 Identity matrix
TMATRIX Matrix_Identity(void)
{
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m = { 1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1

	};
	return m;
}

// Get a translation matrix
//
// IN:		x		Amount of translation in the x direction
//			y		Amount of translation in the y direction
//			z		Amount of translation in the z direction
//
// RETURN:	The translation matrix
TMATRIX Matrix_Create_Translation(float x, float y, float z)
{
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m = { 1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1,
	};
	m._e41 += x;
	m._e42 += y;
	m._e43 += z;
	return m;
}

// Create a scale matrix
//
// IN:		x		Amount to scale in the x direction
//			y		Amount to scale in the y direction
//			z		Amount to scale in the z direction
//
// RETURN:	The scale matrix
TMATRIX Matrix_Create_Scale(float x, float y, float z)
{
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m = { x, 0, 0, 0,
				  0, y, 0, 0,
				  0, 0, z, 0,
				  0, 0, 0, 1 };
	return m;
}

// Get a rotation matrix for rotation about the x-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A X-Rotation Matrix
TMATRIX Matrix_Create_Rotation_X(float Deg)
{
	float fuck = Degrees_To_Radians(Deg);
	// TODO LAB 2: Replace  with your implementation.
	TMATRIX m = { 1, 0,          0,         0,
				  0, cos(fuck), -sin(fuck),   0,
				  0, sin(fuck), cos(fuck),    0,
				  0, 0,              0,	    1
	};
	return m;
}

// Get a rotation matrix for rotation about the y-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A Y-Rotation Matrix
TMATRIX Matrix_Create_Rotation_Y(float Deg)
{
	float fuck = Degrees_To_Radians(Deg);
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m = { cos(fuck), 0, sin(fuck),        0,
						 0, 1,        0,               0,
				 -sin(fuck), 0, cos(fuck),        0,
						 0, 0,        0,               1 };
	return m;
}

// Get a rotation matrix for rotation about the z-axis
//
// IN:		Deg		Angle to rotate ( Degree measure)
//
// RETURN:	A Z-Rotation Matrix
TMATRIX Matrix_Create_Rotation_Z(float Deg)
{

	float fuck =  Degrees_To_Radians(Deg);
	// TODO LAB 2: Replace with your implementation.
	TMATRIX m = { cos(fuck), -sin(fuck), 0,//, 0,
				  sin(fuck), cos(fuck), 0,//, 0,
					0, 0, 1//, 0,
					//0, 0, 0, 1

	};
	return m;
}

// ADD two matrices together
//
// IN:		m		The first matrix
//			n		The second matrix
//
// RETURN: m + n
TMATRIX Matrix_Matrix_Add(TMATRIX m, TMATRIX n)
{
	TMATRIX sum;
	for (int i = 0; i < 16; i++)
	{
		sum.e[i] = m.e[i] + n.e[i];
	}
	// TODO LAB 2: Replace with your implementation.
	return sum;
}

// SUBTRACT two matrices
//
// IN:		m		The first matrix (left hand side)
//			n		The second matrix (right hand side)
//
// RETURN: m - n
TMATRIX Matrix_Matrix_Sub(TMATRIX m, TMATRIX n)
{
	TMATRIX sub;
	for (int i = 0; i < 16; i++)
	{
		sub.e[i] = m.e[i] - n.e[i];
	}
	// TODO LAB 2: Replace with your implementation.
	return sub;
}

// Multiply a matrix by a scalar
//
// IN:		m		The matrix to be scaled (right hand side)
//			s		The value to scale by   (left hand side)
//
// RETURN:	The matrix formed by s*[m]
TMATRIX Matrix_Scalar_Multiply(TMATRIX m, float s)
{
	TMATRIX mult;
	for (int i = 0; i < 16; i++)
	{
		mult.e[i] = m.e[i] * s;
	}
	// TODO LAB 2: Replace with your implementation.
	return mult;
}

// Negate a matrix
//
// IN:		m		The matrix to negate
//
// RETURN:  The negation of m
TMATRIX Matrix_Negate(TMATRIX m)
{
	TMATRIX neg;
	for (int i = 0; i < 16; i++)
	{
		neg.e[i] = m.e[i] * -1;
	}
	// TODO LAB 2: Replace with your implementation.
	return neg;
}

// Transpose a matrix
//
// IN:		m		The matrix to transpose
//
// RETURN:	The transpose of m
TMATRIX Matrix_Transpose(TMATRIX m)
{
	float temp = m._e12;
	m._e12 = m._e21;
	m._e21 = temp;

	temp = m._e13;
	m._e13 = m._e31;
	m._e31 = temp;

	temp = m._e32;
	m._e32 = m._e23;
	m._e23 = temp;

	temp = m._e41;
	m._e41 = m._e14;
	m._e14 = temp;

	temp = m._e24;
	m._e24 = m._e42;
	m._e42 = temp;

	temp = m._e43;
	m._e43 = m._e34;
	m._e34 = temp;

	// TODO LAB 2: Replace with your implementation.
	return m;
}

// Multipy a matrix and a vector
//
// IN:		m		The matrix (left hand side)
//			v		The vector (right hand side)
//
// RETURN:	[m]*v
TVECTOR Matrix_Vector_Multiply(TMATRIX m, TVECTOR v)
{
	m._e11 = m._e11 * v.x;
	m._e12 = m._e12 * v.y;
	m._e13 = m._e13 * v.z;
	m._e14 = m._e14 * v.w;
	m._e21 = m._e21 * v.x;
	m._e22 = m._e22 * v.y;
	m._e23 = m._e23 * v.z;
	m._e24 = m._e24 * v.w;
	m._e31 = m._e31 * v.x;
	m._e32 = m._e32 * v.y;
	m._e33 = m._e33 * v.z;
	m._e34 = m._e34 * v.w;
	m._e41 = m._e41 * v.x;
	m._e42 = m._e42 * v.y;
	m._e43 = m._e43 * v.z;
	m._e44 = m._e44 * v.w;

	v.x = m._e11 + m._e12 + m._e13 + m._e14;
	v.y = m._e21 + m._e22 + m._e23 + m._e24;
	v.z = m._e31 + m._e32 + m._e33 + m._e34;
	v.w = m._e41 + m._e42 + m._e43 + m._e44;
	// TODO LAB 2: Replace with your implementation.
	return v;
}

// Multipy a vector and a matrix
//
// IN:		v		The vector ( left hand side)
//			m		The matrix (right hand side)
//
// RETURN:	v*[m]
TVECTOR Vector_Matrix_Multiply(TVECTOR v, TMATRIX m)
{

	m._e11 = m._e11 * v.x;
	m._e21 = m._e21	* v.y;
	m._e31 = m._e31	* v.z;
	m._e41 = m._e41	* v.w;

	m._e12 = m._e12	* v.x;
	m._e22 = m._e22	* v.y;
	m._e32 = m._e32	* v.z;
	m._e42 = m._e42	* v.w;

	m._e13 = m._e13	* v.x;
	m._e23 = m._e23	* v.y;
	m._e33 = m._e33	* v.z;
	m._e43 = m._e43	* v.w;


	m._e14 = m._e14	* v.x;
	m._e24 = m._e24	* v.y;
	m._e34 = m._e34	* v.z;
	m._e44 = m._e44 * v.w;



	v.x = m._e11 + m._e21 + m._e31 + m._e41;
	v.y = m._e12 + m._e22 + m._e32 + m._e42;
	v.z = m._e13 + m._e23 + m._e33 + m._e43;
	v.w = m._e14 + m._e24 + m._e34 + m._e44;
	// TODO LAB 2: Replace with your implementation.
	return v;
}

// Multiply a matrix by a matrix
//
// IN:		m		First Matrix (left hand side)
//			n		Second Matrix (right hand side)
//
// RETURN:	[m]*[n]
TMATRIX Matrix_Matrix_Multiply(TMATRIX m, TMATRIX n)
{
	TMATRIX mult;
	//for (int i = 0; i < 16; i++)
	//{
	//	mult.e[i] = m.e[i] * n.e[i];
	//}
	mult._e11 = (m._e11 * n._e11) + (m._e12 * n._e21) + (m._e13 * n._e31) + (m._e14 * n._e41);
	mult._e12 = (m._e11 * n._e12) + (m._e12 * n._e22) + (m._e13 * n._e32) + (m._e14 * n._e42);
	mult._e13 = (m._e11 * n._e13) + (m._e12 * n._e23) + (m._e13 * n._e33) + (m._e14 * n._e43);
	mult._e14 = (m._e11 * n._e14) + (m._e12 * n._e24) + (m._e13 * n._e34) + (m._e14 * n._e44);

	mult._e21 = (m._e21 * n._e11) + (m._e22 * n._e21) + (m._e23 * n._e31) + (m._e24 * n._e41);
	mult._e22 = (m._e21 * n._e12) + (m._e22 * n._e22) + (m._e23 * n._e32) + (m._e24 * n._e42);
	mult._e23 = (m._e21 * n._e13) + (m._e22 * n._e23) + (m._e23 * n._e33) + (m._e24 * n._e43);
	mult._e24 = (m._e21 * n._e14) + (m._e22 * n._e24) + (m._e23 * n._e34) + (m._e24 * n._e44);

	mult._e31 = (m._e31 * n._e11) + (m._e32 * n._e21) + (m._e33 * n._e31) + (m._e34 * n._e41);
	mult._e32 = (m._e31 * n._e12) + (m._e32 * n._e22) + (m._e33 * n._e32) + (m._e34 * n._e42);
	mult._e33 = (m._e31 * n._e13) + (m._e32 * n._e23) + (m._e33 * n._e33) + (m._e34 * n._e43);
	mult._e34 = (m._e31 * n._e14) + (m._e32 * n._e24) + (m._e33 * n._e34) + (m._e34 * n._e44);

	mult._e41 = (m._e41 * n._e11) + (m._e42 * n._e21) + (m._e43 * n._e31) + (m._e44 * n._e41);
	mult._e42 = (m._e41 * n._e12) + (m._e42 * n._e22) + (m._e43 * n._e32) + (m._e44 * n._e42);
	mult._e43 = (m._e41 * n._e13) + (m._e42 * n._e23) + (m._e43 * n._e33) + (m._e44 * n._e43);
	mult._e44 = (m._e41 * n._e14) + (m._e42 * n._e24) + (m._e43 * n._e34) + (m._e44 * n._e44);

	// TODO LAB 2: Replace with your implementation.
	return mult;
}

////////////////////////////////////////////////////////////////////////
// Matrix Functions Lab # 3
///////////////////////////////////////////////////////////////////////

// HELPER FUNCTION  *** NOT GRADED, ONLY SUGGESTED ***
// USE THIS FUNCTION TO FIND THE DETERMINANT OF A 3*3
// MATRIX. IT CAN BE USED IN THE MATRIX DETERMINANT
// AND MATRIX INVERSE FUNCTIONS BELOW
// 
// RETURN:	The determinant of a 3x3 matrix
float Matrix_Determinant(float e_11, float e_12, float e_13,
	float e_21, float e_22, float e_23,
	float e_31, float e_32, float e_33)
{
	float det = (e_11 * ((e_22 * e_33) - (e_32 * e_23))) - (e_12 * ((e_21 * e_33) - (e_23 * e_31))) + (e_13 * ((e_21 * e_32) - (e_22 * e_31)));
	return det;
}


// Get the determinant of a matrix
//
// IN:		m		The ONE!
//
// RETURN:	It's deterinant
float Matrix_Determinant(TMATRIX m)
{
	float firstDem = m._e11 * Matrix_Determinant(m._e22, m._e23, m._e24,
		m._e32, m._e33, m._e34,
		m._e42, m._e43, m._e44);
	float secondDem = m._e12 * Matrix_Determinant(m._e21, m._e23, m._e24,
		m._e31, m._e33, m._e34,
		m._e41, m._e43, m._e44);
	float thirdDem = m._e13 * Matrix_Determinant(m._e21, m._e22, m._e24,
		m._e31, m._e32, m._e34,
		m._e41, m._e42, m._e44);
	float fourthDem = m._e14 * Matrix_Determinant(m._e21, m._e22, m._e23,
		m._e31, m._e32, m._e33,
		m._e41, m._e42, m._e43);
	// TODO LAB 3: Replace with your implementation.
	return firstDem - secondDem + thirdDem - fourthDem;
}

// Get the inverse of a matrix
//
// IN:		m		The matrix to inverse
//
// RETURN:	The Inverse of [m]
//
// NOTE: Returns the matrix itself if m is not invertable.
TMATRIX Matrix_Inverse(TMATRIX m)
{
	TMATRIX adj;
	TMATRIX Mdet;
	float det = Matrix_Determinant(m);
	
	Mdet._e11 =  Matrix_Determinant(m._e22, m._e23, m._e24, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44);
	Mdet._e12 =  -1 * Matrix_Determinant(m._e21, m._e23, m._e24, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44);
	Mdet._e13 =  Matrix_Determinant(m._e21, m._e22, m._e24, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44);
	Mdet._e14 =  -1 * Matrix_Determinant(m._e21, m._e22, m._e23, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43);
				 				 
	Mdet._e21 =  -1 * Matrix_Determinant(m._e12, m._e13, m._e14, m._e32, m._e33, m._e34, m._e42, m._e43, m._e44);
	Mdet._e22 =  Matrix_Determinant(m._e11, m._e13, m._e14, m._e31, m._e33, m._e34, m._e41, m._e43, m._e44);
	Mdet._e23 =  -1 * Matrix_Determinant(m._e11, m._e12, m._e14, m._e31, m._e32, m._e34, m._e41, m._e42, m._e44);
    Mdet._e24 =  Matrix_Determinant(m._e11, m._e12, m._e13, m._e31, m._e32, m._e33, m._e41, m._e42, m._e43);
								 
    Mdet._e31 = Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e42, m._e43, m._e44);
    Mdet._e32 = -1 * Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e41, m._e43, m._e44);
    Mdet._e33 = Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e41, m._e42, m._e44);
    Mdet._e34 = -1 * Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e41, m._e42, m._e43);
   				 			 
    Mdet._e41 = -1 * Matrix_Determinant(m._e12, m._e13, m._e14, m._e22, m._e23, m._e24, m._e32, m._e33, m._e34);
    Mdet._e42 = Matrix_Determinant(m._e11, m._e13, m._e14, m._e21, m._e23, m._e24, m._e31, m._e33, m._e34);
    Mdet._e43 = -1 * Matrix_Determinant(m._e11, m._e12, m._e14, m._e21, m._e22, m._e24, m._e31, m._e32, m._e34);
    Mdet._e44 = Matrix_Determinant(m._e11, m._e12, m._e13, m._e21, m._e22, m._e23, m._e31, m._e32, m._e33);

	adj = Matrix_Transpose(Mdet);

	if (det == 0)
	{
		return m;
	}
	for (int i = 0; i < 16; i++)
	{
		Mdet.e[i] = adj.e[i] / det;
	}

	// TODO LAB 3: Replace with your implementation.
	return Mdet;
}



