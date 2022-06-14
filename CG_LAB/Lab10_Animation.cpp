//// SphereWorld.cpp
//// OpenGL SuperBible
//// Demonstrates an immersive 3D environment using actors
//// and a camera. This version adds lights and material properties
//// and shadows.
//// Program by Richard S. Wright Jr.
//
//#include ".\GL\freeglut.h"
//#include <cmath>
//#include <string.h>	// Memcpy lives here on most systems
//
//#pragma region gltools
//
//// GLTools.h
//// OpenGL SuperBible
//// Copyright 1998 - 2003 Richard S. Wright Jr.
//// Code by Richard S. Wright Jr.
//// All Macros prefixed with GLT_, all functions prefixed with glt... This
//// should avoid most namespace problems
//// Some of these functions allocate memory. Use CRT functions to free
//// Report bugs to rwright@starstonesoftware.com
//
//#ifndef __GLTOOLS__LIBRARY
//#define __GLTOOLS__LIBRARY
//
//#define FREEGLUT_STATIC
//
//// Bring in OpenGL 
//// Windows
//#ifdef WIN32
//#include <windows.h>		// Must have for Windows platform builds
////#include "glee.h"			// OpenGL Extension "autoloader"
//#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
//#include <gl\glu.h>			// OpenGL Utilities
////#include "glut.h"			// Glut (Free-Glut on Windows)
//#endif
//
//// Mac OS X
//#ifdef __APPLE__
//#include <Carbon/Carbon.h>	// Brings in most Apple specific stuff
//#include "glee.h"		// OpenGL Extension "autoloader"
//#include <OpenGL/gl.h>		// Apple OpenGL haders (version depends on OS X SDK version)
//#include <OpenGL/glu.h>		// OpenGL Utilities
//#include <Glut/glut.h>		// Apples Implementation of GLUT
//
//// Just ignore sleep on Apple
//#define Sleep(x)
//
//#endif
//
//#ifdef linux
//#include "GLee.h"
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <glut.h>
//#include <stdlib.h>
//
//// Just ignore sleep in linux too
//#define Sleep(x)
//#endif
//
//
//#ifdef linux
//typedef GLvoid(*CallBack)();
//#else
//
//#ifndef WIN32
//typedef GLvoid(*CallBack)(...);            //  XCode (GNU) style
//#else
//typedef GLvoid(_stdcall* CallBack)();      // Visual C++ style
//#endif
//
//#endif
//
//// Needed for NURBS callbacks... VC++ vs. GNU
///*#ifndef WIN32
//#define CALLBACK (GLvoid (*)(...))
//#else
//#define CALLBACK (GLvoid (__stdcall*)())
//#endif
//*/
//
//// Universal includes
//#include <math.h>
//
//
//
//// There is a static block allocated for loading shaders to prevent heap fragmentation
//#define MAX_SHADER_LENGTH   8192
//
//
/////////////////////////////////////////////////////////
//// Macros for big/little endian happiness
//// These are intentionally written to be easy to understand what they 
//// are doing... no flames please on the inefficiency of these.
//#ifdef __BIG_ENDIAN__
/////////////////////////////////////////////////////////////
//// This function says, "this pointer is a little endian value"
//// If the value must be changed it is... otherwise, this
//// function is defined away below (on Intel systems for example)
//inline void LITTLE_ENDIAN_WORD(void* pWord)
//{
//	unsigned char* pBytes = (unsigned char*)pWord;
//	unsigned char temp;
//
//	temp = pBytes[0];
//	pBytes[0] = pBytes[1];
//	pBytes[1] = temp;
//}
//
/////////////////////////////////////////////////////////////
//// This function says, "this pointer is a little endian value"
//// If the value must be changed it is... otherwise, this
//// function is defined away below (on Intel systems for example)
//inline void LITTLE_ENDIAN_DWORD(void* pWord)
//{
//	unsigned char* pBytes = (unsigned char*)pWord;
//	unsigned char temp;
//
//	// Swap outer bytes
//	temp = pBytes[3];
//	pBytes[3] = pBytes[0];
//	pBytes[0] = temp;
//
//	// Swap inner bytes
//	temp = pBytes[1];
//	pBytes[1] = pBytes[2];
//	pBytes[2] = temp;
//}
//#else
//
//// Define them away on little endian systems
//#define LITTLE_ENDIAN_WORD 
//#define LITTLE_ENDIAN_DWORD 
//#endif
//
//
/////////////////////////////////////////////////////////////////////////////////
////         THE LIBRARY....
/////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////
//// Load a .TGA file
//GLbyte* gltLoadTGA(const char* szFileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);
//
//// Capute the frame buffer and write it as a .tga
//GLint gltWriteTGA(const char* szFileName);
//
//// Draw a Torus
//void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius, GLint numMajor, GLint numMinor);
//
//// Just draw a simple sphere with normals and texture coordinates
//void gltDrawSphere(GLfloat fRadius, GLint iSlices, GLint iStacks);
//
//// Draw a 3D unit Axis set
//void gltDrawUnitAxes(void);
//
//// Shader loading support
////bool bLoadShaderFile(const char* szFile, GLhandleARB shader);
////GLhandleARB gltLoadShaderPair(const char* szVertexProg, const char* szFragmentProg);
//
//// Get the OpenGL version, returns fals on error
//bool gltGetOpenGLVersion(int& nMajor, int& nMinor);
//
//// Check to see if an exension is supported
//int gltIsExtSupported(const char* szExtension);
//
//// Get the function pointer for an extension
//void* gltGetExtensionPointer(const char* szFunctionName);
//
/////////////////////////////////////////////////////////////////////////////////
//// Win32 Only
//#ifdef WIN32
//int gltIsWGLExtSupported(HDC hDC, const char* szExtension);
//#endif
//
//
//#endif
//
//
//#pragma endregion
//
//#pragma region math3d
//
//#pragma region header
//
/////////////////////////////////////////////////////////////////////////////////
//// Data structures and containers
//// Much thought went into how these are declared. Many libraries declare these
//// as structures with x, y, z data members. However structure alignment issues
//// could limit the portability of code based on such structures, or the binary
//// compatibility of data files (more likely) that contain such structures across
//// compilers/platforms. Arrays are always tightly packed, and are more efficient 
//// for moving blocks of data around (usually).
//// Sigh... yes, I probably should use GLfloat, etc. But that requires that we
//// always include OpenGL. Since this library is also useful for non-graphical
//// applications, I shall risk the wrath of the portability gods...
//
//typedef float	M3DVector2f[2];		// 3D points = 3D Vectors, but we need a 
//typedef double	M3DVector2d[2];		// 2D representations sometimes... (x,y) order
//
//typedef float	M3DVector3f[3];		// Vector of three floats (x, y, z)
//typedef double	M3DVector3d[3];		// Vector of three doubles (x, y, z)
//
//typedef float	M3DVector4f[4];		// Lesser used... Do we really need these?
//typedef double	M3DVector4d[4];		// Yes, occasionaly we do need a trailing w component
//
//
//
//// 3x3 matrix - column major. X vector is 0, 1, 2, etc.
////		0	3	6	
////		1	4	7
////		2	5	8
//typedef float	M3DMatrix33f[9];		// A 3 x 3 matrix, column major (floats) - OpenGL Style
//typedef double	M3DMatrix33d[9];		// A 3 x 3 matrix, column major (doubles) - OpenGL Style
//
//
//// 4x4 matrix - column major. X vector is 0, 1, 2, etc.
////	0	4	8	12
////	1	5	9	13
////	2	6	10	14
////	3	7	11	15
//typedef float M3DMatrix44f[16];		// A 4 X 4 matrix, column major (floats) - OpenGL style
//typedef double M3DMatrix44d[16];	// A 4 x 4 matrix, column major (doubles) - OpenGL style
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Useful constants
//#define M3D_PI (3.14159265358979323846)
//#define M3D_2PI (2.0 * M3D_PI)
//#define M3D_PI_DIV_180 (0.017453292519943296)
//#define M3D_INV_PI_DIV_180 (57.2957795130823229)
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Useful shortcuts and macros
//// Radians are king... but we need a way to swap back and forth for programmers and presentation.
//// Leaving these as Macros instead of inline functions, causes constants
//// to be evaluated at compile time instead of run time, e.g. m3dDegToRad(90.0)
//#define m3dDegToRad(x)	((x)*M3D_PI_DIV_180)
//#define m3dRadToDeg(x)	((x)*M3D_INV_PI_DIV_180)
//
//// Hour angles
//#define m3dHrToDeg(x)	((x) * (1.0 / 15.0))
//#define m3dHrToRad(x)	m3dDegToRad(m3dHrToDeg(x))
//
//#define m3dDegToHr(x)	((x) * 15.0))
//#define m3dRadToHr(x)	m3dDegToHr(m3dRadToDeg(x))
//
//
//// Returns the same number if it is a power of
//// two. Returns a larger integer if it is not a 
//// power of two. The larger integer is the next
//// highest power of two.
//inline unsigned int m3dIsPOW2(unsigned int iValue)
//{
//	unsigned int nPow2 = 1;
//
//	while (iValue > nPow2)
//		nPow2 = (nPow2 << 1);
//
//	return nPow2;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Inline accessor functions (Macros) for people who just can't count to 3 or 4
//// Really... you should learn to count before you learn to program ;-)
//// 0 = x
//// 1 = y
//// 2 = z
//// 3 = w
//#define	m3dGetVectorX(v) (v[0])
//#define m3dGetVectorY(v) (v[1])
//#define m3dGetVectorZ(v) (v[2])
//#define m3dGetVectorW(v) (v[3])
//
//#define m3dSetVectorX(v, x)	((v)[0] = (x))
//#define m3dSetVectorY(v, y)	((v)[1] = (y))
//#define m3dSetVectorZ(v, z)	((v)[2] = (z))
//#define m3dSetVectorW(v, w)	((v)[3] = (w))
//
/////////////////////////////////////////////////////////////////////////////////
//// Inline vector functions
//// Load Vector with (x, y, z, w).
//inline void m3dLoadVector2(M3DVector2f v, const float x, const float y)
//{
//	v[0] = x; v[1] = y;
//}
//inline void m3dLoadVector2(M3DVector2d v, const float x, const float y)
//{
//	v[0] = x; v[1] = y;
//}
//inline void m3dLoadVector3(M3DVector3f v, const float x, const float y, const float z)
//{
//	v[0] = x; v[1] = y; v[2] = z;
//}
//inline void m3dLoadVector3(M3DVector3d v, const double x, const double y, const double z)
//{
//	v[0] = x; v[1] = y; v[2] = z;
//}
//inline void m3dLoadVector4(M3DVector4f v, const float x, const float y, const float z, const float w)
//{
//	v[0] = x; v[1] = y; v[2] = z; v[3] = w;
//}
//inline void m3dLoadVector4(M3DVector4d v, const double x, const double y, const double z, const double w)
//{
//	v[0] = x; v[1] = y; v[2] = z; v[3] = w;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//// Copy vector src into vector dst
//inline void	m3dCopyVector2(M3DVector2f dst, const M3DVector2f src) { memcpy(dst, src, sizeof(M3DVector2f)); }
//inline void	m3dCopyVector2(M3DVector2d dst, const M3DVector2d src) { memcpy(dst, src, sizeof(M3DVector2d)); }
//
//inline void	m3dCopyVector3(M3DVector3f dst, const M3DVector3f src) { memcpy(dst, src, sizeof(M3DVector3f)); }
//inline void	m3dCopyVector3(M3DVector3d dst, const M3DVector3d src) { memcpy(dst, src, sizeof(M3DVector3d)); }
//
//inline void	m3dCopyVector4(M3DVector4f dst, const M3DVector4f src) { memcpy(dst, src, sizeof(M3DVector4f)); }
//inline void	m3dCopyVector4(M3DVector4d dst, const M3DVector4d src) { memcpy(dst, src, sizeof(M3DVector4d)); }
//
//
//////////////////////////////////////////////////////////////////////////////////
//// Add Vectors (r, a, b) r = a + b
//inline void m3dAddVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1];
//}
//inline void m3dAddVectors2(M3DVector2d r, const M3DVector2d a, const M3DVector2d b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1];
//}
//
//inline void m3dAddVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2];
//}
//inline void m3dAddVectors3(M3DVector3d r, const M3DVector3d a, const M3DVector3d b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2];
//}
//
//inline void m3dAddVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2]; r[3] = a[3] + b[3];
//}
//inline void m3dAddVectors4(M3DVector4d r, const M3DVector4d a, const M3DVector4d b)
//{
//	r[0] = a[0] + b[0];	r[1] = a[1] + b[1]; r[2] = a[2] + b[2]; r[3] = a[3] + b[3];
//}
//
//////////////////////////////////////////////////////////////////////////////////
//// Subtract Vectors (r, a, b) r = a - b
//inline void m3dSubtractVectors2(M3DVector2f r, const M3DVector2f a, const M3DVector2f b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1];
//}
//inline void m3dSubtractVectors2(M3DVector2d r, const M3DVector2d a, const M3DVector2d b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1];
//}
//
//inline void m3dSubtractVectors3(M3DVector3f r, const M3DVector3f a, const M3DVector3f b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2];
//}
//inline void m3dSubtractVectors3(M3DVector3d r, const M3DVector3d a, const M3DVector3d b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2];
//}
//
//inline void m3dSubtractVectors4(M3DVector4f r, const M3DVector4f a, const M3DVector4f b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2]; r[3] = a[3] - b[3];
//}
//inline void m3dSubtractVectors4(M3DVector4d r, const M3DVector4d a, const M3DVector4d b)
//{
//	r[0] = a[0] - b[0]; r[1] = a[1] - b[1]; r[2] = a[2] - b[2]; r[3] = a[3] - b[3];
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
//// Scale Vectors (in place)
//inline void m3dScaleVector2(M3DVector2f v, const float scale)
//{
//	v[0] *= scale; v[1] *= scale;
//}
//inline void m3dScaleVector2(M3DVector2d v, const double scale)
//{
//	v[0] *= scale; v[1] *= scale;
//}
//
//inline void m3dScaleVector3(M3DVector3f v, const float scale)
//{
//	v[0] *= scale; v[1] *= scale; v[2] *= scale;
//}
//inline void m3dScaleVector3(M3DVector3d v, const double scale)
//{
//	v[0] *= scale; v[1] *= scale; v[2] *= scale;
//}
//
//inline void m3dScaleVector4(M3DVector4f v, const float scale)
//{
//	v[0] *= scale; v[1] *= scale; v[2] *= scale; v[3] *= scale;
//}
//inline void m3dScaleVector4(M3DVector4d v, const double scale)
//{
//	v[0] *= scale; v[1] *= scale; v[2] *= scale; v[3] *= scale;
//}
//
//
////////////////////////////////////////////////////////////////////////////////////////
//// Cross Product
//// u x v = result
//// 3 component vectors only.
//inline void m3dCrossProduct33(M3DVector3f result, const M3DVector3f u, const M3DVector3f v)
//{
//	result[0] = u[1] * v[2] - v[1] * u[2];
//	result[1] = -u[0] * v[2] + v[0] * u[2];
//	result[2] = u[0] * v[1] - v[0] * u[1];
//}
//
//inline void m3dCrossProduct33(M3DVector3d result, const M3DVector3d u, const M3DVector3d v)
//{
//	result[0] = u[1] * v[2] - v[1] * u[2];
//	result[1] = -u[0] * v[2] + v[0] * u[2];
//	result[2] = u[0] * v[1] - v[0] * u[1];
//}
//
////////////////////////////////////////////////////////////////////////////////////////
//// Dot Product, only for three component vectors
//// return u dot v
//inline float m3dDotProduct3(const M3DVector3f u, const M3DVector3f v)
//{
//	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
//}
//
//inline double m3dDotProduct3(const M3DVector3d u, const M3DVector3d v)
//{
//	return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
//}
//
////////////////////////////////////////////////////////////////////////////////////////
//// Angle between vectors, only for three component vectors. Angle is in radians...
//inline float m3dGetAngleBetweenVectors3(const M3DVector3f u, const M3DVector3f v)
//{
//	float dTemp = m3dDotProduct3(u, v);
//	return float(acos(double(dTemp)));	// Double cast just gets rid of compiler warning, no real need
//}
//
//inline double m3dGetAngleBetweenVectors3(const M3DVector3d u, const M3DVector3d v)
//{
//	double dTemp = m3dDotProduct3(u, v);
//	return acos(dTemp);
//}
//
////////////////////////////////////////////////////////////////////////////////////////
//// Get Square of a vectors length
//// Only for three component vectors
//inline float m3dGetVectorLengthSquared3(const M3DVector3f u)
//{
//	return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
//}
//
//inline double m3dGetVectorLengthSquared3(const M3DVector3d u)
//{
//	return (u[0] * u[0]) + (u[1] * u[1]) + (u[2] * u[2]);
//}
//
////////////////////////////////////////////////////////////////////////////////////////
//// Get lenght of vector
//// Only for three component vectors.
//inline float m3dGetVectorLength3(const M3DVector3f u)
//{
//	return sqrtf(m3dGetVectorLengthSquared3(u));
//}
//
//inline double m3dGetVectorLength3(const M3DVector3d u)
//{
//	return sqrt(m3dGetVectorLengthSquared3(u));
//}
//
////////////////////////////////////////////////////////////////////////////////////////
//// Normalize a vector
//// Scale a vector to unit length. Easy, just scale the vector by it's length
//inline void m3dNormalizeVector3(M3DVector3f u)
//{
//	m3dScaleVector3(u, 1.0f / m3dGetVectorLength3(u));
//}
//
//inline void m3dNormalizeVector3(M3DVector3d u)
//{
//	m3dScaleVector3(u, 1.0 / m3dGetVectorLength3(u));
//}
//
//
////////////////////////////////////////////////////////////////////////////////////////
//// Get the distance between two points. The distance between two points is just
//// the magnitude of the difference between two vectors
//// Located in math.cpp
//float m3dGetDistanceSquared3(const M3DVector3f u, const M3DVector3f v);
//double m3dGetDistanceSquared3(const M3DVector3d u, const M3DVector3d v);
//
//inline double m3dGetDistance3(const M3DVector3d u, const M3DVector3d v)
//{
//	return sqrt(m3dGetDistanceSquared3(u, v));
//}
//
//inline float m3dGetDistance3(const M3DVector3f u, const M3DVector3f v)
//{
//	return sqrtf(m3dGetDistanceSquared3(u, v));
//}
//
//inline float m3dGetMagnitudeSquared3(const M3DVector3f u) { return u[0] * u[0] + u[1] * u[1] + u[2] * u[2]; }
//inline double m3dGetMagnitudeSquared3(const M3DVector3d u) { return u[0] * u[0] + u[1] * u[1] + u[2] * u[2]; }
//
//inline float m3dGetMagnitude3(const M3DVector3f u) { return sqrtf(m3dGetMagnitudeSquared3(u)); }
//inline double m3dGetMagnitude3(const M3DVector3d u) { return sqrt(m3dGetMagnitudeSquared3(u)); }
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Matrix functions
//// Both floating point and double precision 3x3 and 4x4 matricies are supported.
//// No support is included for arbitrarily dimensioned matricies on purpose, since
//// the 3x3 and 4x4 matrix routines are the most common for the purposes of this
//// library. Matrices are column major, like OpenGL matrices.
//// Unlike the vector functions, some of these are going to have to not be inlined,
//// although many will be.
//
//// Copy Matrix
//// Brain-dead memcpy
//inline void m3dCopyMatrix33(M3DMatrix33f dst, const M3DMatrix33f src)
//{
//	memcpy(dst, src, sizeof(M3DMatrix33f));
//}
//
//inline void m3dCopyMatrix33(M3DMatrix33d dst, const M3DMatrix33d src)
//{
//	memcpy(dst, src, sizeof(M3DMatrix33d));
//}
//
//inline void m3dCopyMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
//{
//	memcpy(dst, src, sizeof(M3DMatrix44f));
//}
//
//inline void m3dCopyMatrix44(M3DMatrix44d dst, const M3DMatrix44d src)
//{
//	memcpy(dst, src, sizeof(M3DMatrix44d));
//}
//
//// LoadIdentity
//// Implemented in Math3d.cpp
//void m3dLoadIdentity33(M3DMatrix33f m);
//void m3dLoadIdentity33(M3DMatrix33d m);
//void m3dLoadIdentity44(M3DMatrix44f m);
//void m3dLoadIdentity44(M3DMatrix44d m);
//
///////////////////////////////////////////////////////////////////////////////
//// Get/Set Column.
//inline void m3dGetMatrixColumn33(M3DVector3f dst, const M3DMatrix33f src, const int column)
//{
//	memcpy(dst, src + (3 * column), sizeof(float) * 3);
//}
//
//inline void m3dGetMatrixColumn33(M3DVector3d dst, const M3DMatrix33d src, const int column)
//{
//	memcpy(dst, src + (3 * column), sizeof(double) * 3);
//}
//
//inline void m3dSetMatrixColumn33(M3DMatrix33f dst, const M3DVector3f src, const int column)
//{
//	memcpy(dst + (3 * column), src, sizeof(float) * 3);
//}
//
//inline void m3dSetMatrixColumn33(M3DMatrix33d dst, const M3DVector3d src, const int column)
//{
//	memcpy(dst + (3 * column), src, sizeof(double) * 3);
//}
//
//inline void m3dGetMatrixColumn44(M3DVector4f dst, const M3DMatrix44f src, const int column)
//{
//	memcpy(dst, src + (4 * column), sizeof(float) * 4);
//}
//
//inline void m3dGetMatrixColumn44(M3DVector4d dst, const M3DMatrix44d src, const int column)
//{
//	memcpy(dst, src + (4 * column), sizeof(double) * 4);
//}
//
//inline void m3dSetMatrixColumn44(M3DMatrix44f dst, const M3DVector4f src, const int column)
//{
//	memcpy(dst + (4 * column), src, sizeof(float) * 4);
//}
//
//inline void m3dSetMatrixColumn44(M3DMatrix44d dst, const M3DVector4d src, const int column)
//{
//	memcpy(dst + (4 * column), src, sizeof(double) * 4);
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Extract a rotation matrix from a 4x4 matrix
//// Extracts the rotation matrix (3x3) from a 4x4 matrix
//inline void m3dExtractRotationMatrix33(M3DMatrix33f dst, const M3DMatrix44f src)
//{
//	memcpy(dst, src, sizeof(float) * 3); // X column
//	memcpy(dst + 3, src + 4, sizeof(float) * 3); // Y column
//	memcpy(dst + 6, src + 8, sizeof(float) * 3); // Z column
//}
//
//// Ditto above, but for doubles
//inline void m3dExtractRotationMatrix33(M3DMatrix33d dst, const M3DMatrix44d src)
//{
//	memcpy(dst, src, sizeof(double) * 3); // X column
//	memcpy(dst + 3, src + 4, sizeof(double) * 3); // Y column
//	memcpy(dst + 6, src + 8, sizeof(double) * 3); // Z column
//}
//
//// Inject Rotation (3x3) into a full 4x4 matrix...
//inline void m3dInjectRotationMatrix44(M3DMatrix44f dst, const M3DMatrix33f src)
//{
//	memcpy(dst, src, sizeof(float) * 4);
//	memcpy(dst + 4, src + 4, sizeof(float) * 4);
//	memcpy(dst + 8, src + 8, sizeof(float) * 4);
//}
//
//// Ditto above for doubles
//inline void m3dInjectRotationMatrix44(M3DMatrix44d dst, const M3DMatrix33d src)
//{
//	memcpy(dst, src, sizeof(double) * 4);
//	memcpy(dst + 4, src + 4, sizeof(double) * 4);
//	memcpy(dst + 8, src + 8, sizeof(double) * 4);
//}
//
//////////////////////////////////////////////////////////////////////////////////
//// MultMatrix
//// Implemented in Math.cpp
//void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b);
//void m3dMatrixMultiply44(M3DMatrix44d product, const M3DMatrix44d a, const M3DMatrix44d b);
//void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b);
//void m3dMatrixMultiply33(M3DMatrix33d product, const M3DMatrix33d a, const M3DMatrix33d b);
//
//
//// Transform - Does rotation and translation via a 4x4 matrix. Transforms
//// a point or vector.
//// By-the-way __inline means I'm asking the compiler to do a cost/benefit analysis. If 
//// these are used frequently, they may not be inlined to save memory. I'm experimenting
//// with this....
//// Just transform a 3 compoment vector
//__inline void m3dTransformVector3(M3DVector3f vOut, const M3DVector3f v, const M3DMatrix44f m)
//{
//	vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12];// * v[3];	// Assuming 1 
//	vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13];// * v[3];	
//	vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];// * v[3];	
//	//vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
//}
//
//// Ditto above, but for doubles
//__inline void m3dTransformVector3(M3DVector3d vOut, const M3DVector3d v, const M3DMatrix44d m)
//{
//	vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12];// * v[3];	 
//	vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13];// * v[3];	
//	vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];// * v[3];	
//	//vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
//}
//
//// Full four component transform
//__inline void m3dTransformVector4(M3DVector4f vOut, const M3DVector4f v, const M3DMatrix44f m)
//{
//	vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
//	vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
//	vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
//	vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
//}
//
//// Ditto above, but for doubles
//__inline void m3dTransformVector4(M3DVector4d vOut, const M3DVector4d v, const M3DMatrix44d m)
//{
//	vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
//	vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
//	vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
//	vOut[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
//}
//
//
//
//// Just do the rotation, not the translation... this is usually done with a 3x3
//// Matrix.
//__inline void m3dRotateVector(M3DVector3f vOut, const M3DVector3f p, const M3DMatrix33f m)
//{
//	vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];
//	vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];
//	vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];
//}
//
//// Ditto above, but for doubles
//__inline void m3dRotateVector(M3DVector3d vOut, const M3DVector3d p, const M3DMatrix33d m)
//{
//	vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];
//	vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];
//	vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];
//}
//
//
//// Create a Scaling Matrix
//inline void m3dScaleMatrix33(M3DMatrix33f m, float xScale, float yScale, float zScale)
//{
//	m3dLoadIdentity33(m); m[0] = xScale; m[4] = yScale; m[8] = zScale;
//}
//
//inline void m3dScaleMatrix33(M3DMatrix33f m, const M3DVector3f vScale)
//{
//	m3dLoadIdentity33(m); m[0] = vScale[0]; m[4] = vScale[1]; m[8] = vScale[2];
//}
//
//inline void m3dScaleMatrix33(M3DMatrix33d m, double xScale, double yScale, double zScale)
//{
//	m3dLoadIdentity33(m); m[0] = xScale; m[4] = yScale; m[8] = zScale;
//}
//
//inline void m3dScaleMatrix33(M3DMatrix33d m, const M3DVector3d vScale)
//{
//	m3dLoadIdentity33(m); m[0] = vScale[0]; m[4] = vScale[1]; m[8] = vScale[2];
//}
//
//inline void m3dScaleMatrix44(M3DMatrix44f m, float xScale, float yScale, float zScale)
//{
//	m3dLoadIdentity44(m); m[0] = xScale; m[5] = yScale; m[10] = zScale;
//}
//
//inline void m3dScaleMatrix44(M3DMatrix44f m, const M3DVector3f vScale)
//{
//	m3dLoadIdentity44(m); m[0] = vScale[0]; m[5] = vScale[1]; m[10] = vScale[2];
//}
//
//inline void m3dScaleMatrix44(M3DMatrix44d m, double xScale, double yScale, double zScale)
//{
//	m3dLoadIdentity44(m); m[0] = xScale; m[5] = yScale; m[10] = zScale;
//}
//
//inline void m3dScaleMatrix44(M3DMatrix44d m, const M3DVector3d vScale)
//{
//	m3dLoadIdentity44(m); m[0] = vScale[0]; m[5] = vScale[1]; m[10] = vScale[2];
//}
//
//
//void m3dMakePerspectiveMatrix(M3DMatrix44f mProjection, float fFov, float fAspect, float zMin, float zMax);
//void m3dMakeOrthographicMatrix(M3DMatrix44f mProjection, float xMin, float xMax, float yMin, float yMax, float zMin, float zMax);
//
//
//// Create a Rotation matrix
//// Implemented in math3d.cpp
//void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z);
//void m3dRotationMatrix33(M3DMatrix33d m, double angle, double x, double y, double z);
//void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z);
//void m3dRotationMatrix44(M3DMatrix44d m, double angle, double x, double y, double z);
//
//// Create a Translation matrix. Only 4x4 matrices have translation components
//inline void m3dTranslationMatrix44(M3DMatrix44f m, float x, float y, float z)
//{
//	m3dLoadIdentity44(m); m[12] = x; m[13] = y; m[14] = z;
//}
//
//inline void m3dTranslationMatrix44(M3DMatrix44d m, double x, double y, double z)
//{
//	m3dLoadIdentity44(m); m[12] = x; m[13] = y; m[14] = z;
//}
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// Other Miscellaneous functions
//
//// Find a normal from three points
//// Implemented in math3d.cpp
//void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
//	const M3DVector3f point3);
//void m3dFindNormal(M3DVector3d result, const M3DVector3d point1, const M3DVector3d point2,
//	const M3DVector3d point3);
//
//
//// Calculates the signed distance of a point to a plane
//inline float m3dGetDistanceToPlane(const M3DVector3f point, const M3DVector4f plane)
//{
//	return point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
//}
//
//inline double m3dGetDistanceToPlane(const M3DVector3d point, const M3DVector4d plane)
//{
//	return point[0] * plane[0] + point[1] * plane[1] + point[2] * plane[2] + plane[3];
//}
//
//
//// Get plane equation from three points
//void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3);
//void m3dGetPlaneEquation(M3DVector4d planeEq, const M3DVector3d p1, const M3DVector3d p2, const M3DVector3d p3);
//
//// Determine if a ray intersects a sphere
//// Return value is < 0 if the ray does not intersect
//// Return value is 0.0 if ray is tangent
//// Positive value is distance to the intersection point
//double m3dRaySphereTest(const M3DVector3d point, const M3DVector3d ray, const M3DVector3d sphereCenter, double sphereRadius);
//float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius);
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Faster (and one shortcut) replacements for gluProject
//void m3dProjectXY(M3DVector2f vPointOut, const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn);
//void m3dProjectXYZ(M3DVector3f vPointOut, const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn);
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//// This function does a three dimensional Catmull-Rom "spline" interpolation between p1 and p2
//void m3dCatmullRom(M3DVector3f vOut, const M3DVector3f vP0, const M3DVector3f vP1, const M3DVector3f vP2, const M3DVector3f vP3, float t);
//void m3dCatmullRom(M3DVector3d vOut, const M3DVector3d vP0, const M3DVector3d vP1, const M3DVector3d vP2, const M3DVector3d vP3, double t);
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//// Compare floats and doubles... 
//inline bool m3dCloseEnough(const float fCandidate, const float fCompare, const float fEpsilon)
//{
//	return (fabs(fCandidate - fCompare) < fEpsilon);
//}
//
//inline bool m3dCloseEnough(const double dCandidate, const double dCompare, const double dEpsilon)
//{
//	return (fabs(dCandidate - dCompare) < dEpsilon);
//}
//
//////////////////////////////////////////////////////////////////////////////
//// Used for normal mapping. Finds the tangent bases for a triangle...
//// Only a floating point implementation is provided. This has no practical use as doubles.
//void m3dCalculateTangentBasis(M3DVector3f vTangent, const M3DVector3f pvTriangle[3], const M3DVector2f pvTexCoords[3], const M3DVector3f N);
//
//////////////////////////////////////////////////////////////////////////////
//// Smoothly step between 0 and 1 between edge1 and edge 2
//double m3dSmoothStep(const double edge1, const double edge2, const double x);
//float m3dSmoothStep(const float edge1, const float edge2, const float x);
//
///////////////////////////////////////////////////////////////////////////////
//// Planar shadow Matrix
//void m3dMakePlanarShadowMatrix(M3DMatrix44d proj, const M3DVector4d planeEq, const M3DVector3d vLightPos);
//void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos);
//
///////////////////////////////////////////////////////////////////////////////
//// Closest point on a ray to another point in space
//double m3dClosestPointOnRay(M3DVector3d vPointOnRay, const M3DVector3d vRayOrigin, const M3DVector3d vUnitRayDir,
//	const M3DVector3d vPointInSpace);
//
//float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir,
//	const M3DVector3f vPointInSpace);
//
//
//
//
//#pragma endregion
//
//////////////////////////////////////////////////////////////
//// LoadIdentity
//// For 3x3 and 4x4 float and double matricies.
//// 3x3 float
//void m3dLoadIdentity33(M3DMatrix33f m)
//{
//	// Don't be fooled, this is still column major
//	static M3DMatrix33f	identity = { 1.0f, 0.0f, 0.0f ,
//									 0.0f, 1.0f, 0.0f,
//									 0.0f, 0.0f, 1.0f };
//
//	memcpy(m, identity, sizeof(M3DMatrix33f));
//}
//
//// 3x3 double
//void m3dLoadIdentity33(M3DMatrix33d m)
//{
//	// Don't be fooled, this is still column major
//	static M3DMatrix33d	identity = { 1.0, 0.0, 0.0 ,
//									 0.0, 1.0, 0.0,
//									 0.0, 0.0, 1.0 };
//
//	memcpy(m, identity, sizeof(M3DMatrix33d));
//}
//
//// 4x4 float
//void m3dLoadIdentity44(M3DMatrix44f m)
//{
//	// Don't be fooled, this is still column major
//	static M3DMatrix44f	identity = { 1.0f, 0.0f, 0.0f, 0.0f,
//									 0.0f, 1.0f, 0.0f, 0.0f,
//									 0.0f, 0.0f, 1.0f, 0.0f,
//									 0.0f, 0.0f, 0.0f, 1.0f };
//
//	memcpy(m, identity, sizeof(M3DMatrix44f));
//}
//
//// 4x4 double
//void m3dLoadIdentity44(M3DMatrix44d m)
//{
//	static M3DMatrix44d	identity = { 1.0, 0.0, 0.0, 0.0,
//									 0.0, 1.0, 0.0, 0.0,
//									 0.0, 0.0, 1.0, 0.0,
//									 0.0, 0.0, 0.0, 1.0 };
//
//	memcpy(m, identity, sizeof(M3DMatrix44d));
//}
//
//
//////////////////////////////////////////////////////////////////////////
//// Return the square of the distance between two points
//// Should these be inlined...?
//float m3dGetDistanceSquared(const M3DVector3f u, const M3DVector3f v)
//{
//	float x = u[0] - v[0];
//	x = x * x;
//
//	float y = u[1] - v[1];
//	y = y * y;
//
//	float z = u[2] - v[2];
//	z = z * z;
//
//	return (x + y + z);
//}
//
//// Ditto above, but for doubles
//double m3dGetDistanceSquared(const M3DVector3d u, const M3DVector3d v)
//{
//	double x = u[0] - v[0];
//	x = x * x;
//
//	double y = u[1] - v[1];
//	y = y * y;
//
//	double z = u[2] - v[2];
//	z = z * z;
//
//	return (x + y + z);
//}
//
//#define A(row,col)  a[(col<<2)+row]
//#define B(row,col)  b[(col<<2)+row]
//#define P(row,col)  product[(col<<2)+row]
//
/////////////////////////////////////////////////////////////////////////////////
//// Multiply two 4x4 matricies
//void m3dMatrixMultiply44(M3DMatrix44f product, const M3DMatrix44f a, const M3DMatrix44f b)
//{
//	for (int i = 0; i < 4; i++) {
//		float ai0 = A(i, 0), ai1 = A(i, 1), ai2 = A(i, 2), ai3 = A(i, 3);
//		P(i, 0) = ai0 * B(0, 0) + ai1 * B(1, 0) + ai2 * B(2, 0) + ai3 * B(3, 0);
//		P(i, 1) = ai0 * B(0, 1) + ai1 * B(1, 1) + ai2 * B(2, 1) + ai3 * B(3, 1);
//		P(i, 2) = ai0 * B(0, 2) + ai1 * B(1, 2) + ai2 * B(2, 2) + ai3 * B(3, 2);
//		P(i, 3) = ai0 * B(0, 3) + ai1 * B(1, 3) + ai2 * B(2, 3) + ai3 * B(3, 3);
//	}
//}
//
//// Ditto above, but for doubles
//void m3dMatrixMultiply(M3DMatrix44d product, const M3DMatrix44d a, const M3DMatrix44d b)
//{
//	for (int i = 0; i < 4; i++) {
//		double ai0 = A(i, 0), ai1 = A(i, 1), ai2 = A(i, 2), ai3 = A(i, 3);
//		P(i, 0) = ai0 * B(0, 0) + ai1 * B(1, 0) + ai2 * B(2, 0) + ai3 * B(3, 0);
//		P(i, 1) = ai0 * B(0, 1) + ai1 * B(1, 1) + ai2 * B(2, 1) + ai3 * B(3, 1);
//		P(i, 2) = ai0 * B(0, 2) + ai1 * B(1, 2) + ai2 * B(2, 2) + ai3 * B(3, 2);
//		P(i, 3) = ai0 * B(0, 3) + ai1 * B(1, 3) + ai2 * B(2, 3) + ai3 * B(3, 3);
//	}
//}
//#undef A
//#undef B
//#undef P
//
//
//#define A33(row,col)  a[(col*3)+row]
//#define B33(row,col)  b[(col*3)+row]
//#define P33(row,col)  product[(col*3)+row]
//
/////////////////////////////////////////////////////////////////////////////////
//// Multiply two 3x3 matricies
//void m3dMatrixMultiply33(M3DMatrix33f product, const M3DMatrix33f a, const M3DMatrix33f b)
//{
//	for (int i = 0; i < 3; i++) {
//		float ai0 = A33(i, 0), ai1 = A33(i, 1), ai2 = A33(i, 2);
//		P33(i, 0) = ai0 * B33(0, 0) + ai1 * B33(1, 0) + ai2 * B33(2, 0);
//		P33(i, 1) = ai0 * B33(0, 1) + ai1 * B33(1, 1) + ai2 * B33(2, 1);
//		P33(i, 2) = ai0 * B33(0, 2) + ai1 * B33(1, 2) + ai2 * B33(2, 2);
//	}
//}
//
//// Ditto above, but for doubles
//void m3dMatrixMultiply44(M3DMatrix33d product, const M3DMatrix33d a, const M3DMatrix33d b)
//{
//	for (int i = 0; i < 3; i++) {
//		double ai0 = A33(i, 0), ai1 = A33(i, 1), ai2 = A33(i, 2);
//		P33(i, 0) = ai0 * B33(0, 0) + ai1 * B33(1, 0) + ai2 * B33(2, 0);
//		P33(i, 1) = ai0 * B33(0, 1) + ai1 * B33(1, 1) + ai2 * B33(2, 1);
//		P33(i, 2) = ai0 * B33(0, 2) + ai1 * B33(1, 2) + ai2 * B33(2, 2);
//	}
//}
//
//#undef A33
//#undef B33
//#undef P33
//
//#define M33(row,col)  m[col*3+row]
//
/////////////////////////////////////////////////////////////////////////////////
//// Creates a 3x3 rotation matrix, takes radians NOT degrees
//void m3dRotationMatrix33(M3DMatrix33f m, float angle, float x, float y, float z)
//{
//
//	float mag, s, c;
//	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
//
//	s = float(sin(angle));
//	c = float(cos(angle));
//
//	mag = float(sqrt(x * x + y * y + z * z));
//
//	// Identity matrix
//	if (mag == 0.0f) {
//		m3dLoadIdentity33(m);
//		return;
//	}
//
//	// Rotation matrix is normalized
//	x /= mag;
//	y /= mag;
//	z /= mag;
//
//
//
//	xx = x * x;
//	yy = y * y;
//	zz = z * z;
//	xy = x * y;
//	yz = y * z;
//	zx = z * x;
//	xs = x * s;
//	ys = y * s;
//	zs = z * s;
//	one_c = 1.0f - c;
//
//	M33(0, 0) = (one_c * xx) + c;
//	M33(0, 1) = (one_c * xy) - zs;
//	M33(0, 2) = (one_c * zx) + ys;
//
//	M33(1, 0) = (one_c * xy) + zs;
//	M33(1, 1) = (one_c * yy) + c;
//	M33(1, 2) = (one_c * yz) - xs;
//
//	M33(2, 0) = (one_c * zx) - ys;
//	M33(2, 1) = (one_c * yz) + xs;
//	M33(2, 2) = (one_c * zz) + c;
//}
//
//#undef M33
//
/////////////////////////////////////////////////////////////////////////////////
//// Creates a 4x4 rotation matrix, takes radians NOT degrees
//void m3dRotationMatrix44(M3DMatrix44f m, float angle, float x, float y, float z)
//{
//	float mag, s, c;
//	float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
//
//	s = float(sin(angle));
//	c = float(cos(angle));
//
//	mag = float(sqrt(x * x + y * y + z * z));
//
//	// Identity matrix
//	if (mag == 0.0f) {
//		m3dLoadIdentity44(m);
//		return;
//	}
//
//	// Rotation matrix is normalized
//	x /= mag;
//	y /= mag;
//	z /= mag;
//
//#define M(row,col)  m[col*4+row]
//
//	xx = x * x;
//	yy = y * y;
//	zz = z * z;
//	xy = x * y;
//	yz = y * z;
//	zx = z * x;
//	xs = x * s;
//	ys = y * s;
//	zs = z * s;
//	one_c = 1.0f - c;
//
//	M(0, 0) = (one_c * xx) + c;
//	M(0, 1) = (one_c * xy) - zs;
//	M(0, 2) = (one_c * zx) + ys;
//	M(0, 3) = 0.0f;
//
//	M(1, 0) = (one_c * xy) + zs;
//	M(1, 1) = (one_c * yy) + c;
//	M(1, 2) = (one_c * yz) - xs;
//	M(1, 3) = 0.0f;
//
//	M(2, 0) = (one_c * zx) - ys;
//	M(2, 1) = (one_c * yz) + xs;
//	M(2, 2) = (one_c * zz) + c;
//	M(2, 3) = 0.0f;
//
//	M(3, 0) = 0.0f;
//	M(3, 1) = 0.0f;
//	M(3, 2) = 0.0f;
//	M(3, 3) = 1.0f;
//
//#undef M
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Ditto above, but for doubles
//void m3dRotationMatrix33(M3DMatrix33d m, double angle, double x, double y, double z)
//{
//	double mag, s, c;
//	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
//
//	s = sin(angle);
//	c = cos(angle);
//
//	mag = sqrt(x * x + y * y + z * z);
//
//	// Identity matrix
//	if (mag == 0.0) {
//		m3dLoadIdentity33(m);
//		return;
//	}
//
//	// Rotation matrix is normalized
//	x /= mag;
//	y /= mag;
//	z /= mag;
//
//#define M(row,col)  m[col*3+row]
//
//	xx = x * x;
//	yy = y * y;
//	zz = z * z;
//	xy = x * y;
//	yz = y * z;
//	zx = z * x;
//	xs = x * s;
//	ys = y * s;
//	zs = z * s;
//	one_c = 1.0 - c;
//
//	M(0, 0) = (one_c * xx) + c;
//	M(0, 1) = (one_c * xy) - zs;
//	M(0, 2) = (one_c * zx) + ys;
//
//	M(1, 0) = (one_c * xy) + zs;
//	M(1, 1) = (one_c * yy) + c;
//	M(1, 2) = (one_c * yz) - xs;
//
//	M(2, 0) = (one_c * zx) - ys;
//	M(2, 1) = (one_c * yz) + xs;
//	M(2, 2) = (one_c * zz) + c;
//
//#undef M
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Creates a 4x4 rotation matrix, takes radians NOT degrees
//void m3dRotationMatrix44(M3DMatrix44d m, double angle, double x, double y, double z)
//{
//	double mag, s, c;
//	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
//
//	s = sin(angle);
//	c = cos(angle);
//
//	mag = sqrt(x * x + y * y + z * z);
//
//	// Identity matrix
//	if (mag == 0.0) {
//		m3dLoadIdentity44(m);
//		return;
//	}
//
//	// Rotation matrix is normalized
//	x /= mag;
//	y /= mag;
//	z /= mag;
//
//#define M(row,col)  m[col*4+row]
//
//	xx = x * x;
//	yy = y * y;
//	zz = z * z;
//	xy = x * y;
//	yz = y * z;
//	zx = z * x;
//	xs = x * s;
//	ys = y * s;
//	zs = z * s;
//	one_c = 1.0f - c;
//
//	M(0, 0) = (one_c * xx) + c;
//	M(0, 1) = (one_c * xy) - zs;
//	M(0, 2) = (one_c * zx) + ys;
//	M(0, 3) = 0.0;
//
//	M(1, 0) = (one_c * xy) + zs;
//	M(1, 1) = (one_c * yy) + c;
//	M(1, 2) = (one_c * yz) - xs;
//	M(1, 3) = 0.0;
//
//	M(2, 0) = (one_c * zx) - ys;
//	M(2, 1) = (one_c * yz) + xs;
//	M(2, 2) = (one_c * zz) + c;
//	M(2, 3) = 0.0;
//
//	M(3, 0) = 0.0;
//	M(3, 1) = 0.0;
//	M(3, 2) = 0.0;
//	M(3, 3) = 1.0;
//
//#undef M
//}
//
//// Lifted from Mesa
///*
// * Compute inverse of 4x4 transformation matrix.
// * Code contributed by Jacques Leroy jle@star.be
// * Return GL_TRUE for success, GL_FALSE for failure (singular matrix)
// */
//bool m3dInvertMatrix44(M3DMatrix44f dst, const M3DMatrix44f src)
//{
//#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
//#define MAT(m,r,c) (m)[(c)*4+(r)]
//
//	float wtmp[4][8];
//	float m0, m1, m2, m3, s;
//	float* r0, * r1, * r2, * r3;
//
//	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
//
//	r0[0] = MAT(src, 0, 0), r0[1] = MAT(src, 0, 1),
//		r0[2] = MAT(src, 0, 2), r0[3] = MAT(src, 0, 3),
//		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
//
//		r1[0] = MAT(src, 1, 0), r1[1] = MAT(src, 1, 1),
//		r1[2] = MAT(src, 1, 2), r1[3] = MAT(src, 1, 3),
//		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
//
//		r2[0] = MAT(src, 2, 0), r2[1] = MAT(src, 2, 1),
//		r2[2] = MAT(src, 2, 2), r2[3] = MAT(src, 2, 3),
//		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
//
//		r3[0] = MAT(src, 3, 0), r3[1] = MAT(src, 3, 1),
//		r3[2] = MAT(src, 3, 2), r3[3] = MAT(src, 3, 3),
//		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
//
//	/* choose pivot - or die */
//	if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
//	if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
//	if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
//	if (0.0 == r0[0])  return false;
//
//	/* eliminate first variable     */
//	m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
//	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
//	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
//	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
//	s = r0[4];
//	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
//	s = r0[5];
//	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
//	s = r0[6];
//	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
//	s = r0[7];
//	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }
//
//	/* choose pivot - or die */
//	if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
//	if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
//	if (0.0 == r1[1])  return false;
//
//	/* eliminate second variable */
//	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
//	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
//	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
//	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
//	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
//	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
//	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }
//
//	/* choose pivot - or die */
//	if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
//	if (0.0 == r2[2])  return false;
//
//	/* eliminate third variable */
//	m3 = r3[2] / r2[2];
//	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
//		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
//		r3[7] -= m3 * r2[7];
//
//	/* last check */
//	if (0.0 == r3[3]) return false;
//
//	s = 1.0f / r3[3];              /* now back substitute row 3 */
//	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;
//
//	m2 = r2[3];                 /* now back substitute row 2 */
//	s = 1.0f / r2[2];
//	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
//		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
//	m1 = r1[3];
//	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
//		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
//	m0 = r0[3];
//	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
//		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
//
//	m1 = r1[2];                 /* now back substitute row 1 */
//	s = 1.0f / r1[1];
//	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
//		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
//	m0 = r0[2];
//	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
//		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
//
//	m0 = r0[1];                 /* now back substitute row 0 */
//	s = 1.0f / r0[0];
//	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
//		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
//
//	MAT(dst, 0, 0) = r0[4]; MAT(dst, 0, 1) = r0[5],
//		MAT(dst, 0, 2) = r0[6]; MAT(dst, 0, 3) = r0[7],
//		MAT(dst, 1, 0) = r1[4]; MAT(dst, 1, 1) = r1[5],
//		MAT(dst, 1, 2) = r1[6]; MAT(dst, 1, 3) = r1[7],
//		MAT(dst, 2, 0) = r2[4]; MAT(dst, 2, 1) = r2[5],
//		MAT(dst, 2, 2) = r2[6]; MAT(dst, 2, 3) = r2[7],
//		MAT(dst, 3, 0) = r3[4]; MAT(dst, 3, 1) = r3[5],
//		MAT(dst, 3, 2) = r3[6]; MAT(dst, 3, 3) = r3[7];
//
//	return true;
//
//#undef MAT
//#undef SWAP_ROWS
//}
//
//
//// Ditto above, but for doubles
//bool m3dInvertMatrix44(M3DMatrix44d dst, const M3DMatrix44d src)
//{
//#define SWAP_ROWS(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
//#define MAT(m,r,c) (m)[(c)*4+(r)]
//
//	double wtmp[4][8];
//	double m0, m1, m2, m3, s;
//	double* r0, * r1, * r2, * r3;
//
//	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
//
//	r0[0] = MAT(src, 0, 0), r0[1] = MAT(src, 0, 1),
//		r0[2] = MAT(src, 0, 2), r0[3] = MAT(src, 0, 3),
//		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
//
//		r1[0] = MAT(src, 1, 0), r1[1] = MAT(src, 1, 1),
//		r1[2] = MAT(src, 1, 2), r1[3] = MAT(src, 1, 3),
//		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
//
//		r2[0] = MAT(src, 2, 0), r2[1] = MAT(src, 2, 1),
//		r2[2] = MAT(src, 2, 2), r2[3] = MAT(src, 2, 3),
//		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
//
//		r3[0] = MAT(src, 3, 0), r3[1] = MAT(src, 3, 1),
//		r3[2] = MAT(src, 3, 2), r3[3] = MAT(src, 3, 3),
//		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
//
//	// choose pivot - or die 
//	if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
//	if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
//	if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
//	if (0.0 == r0[0])  return false;
//
//	// eliminate first variable     
//	m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
//	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
//	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
//	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
//	s = r0[4];
//	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
//	s = r0[5];
//	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
//	s = r0[6];
//	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
//	s = r0[7];
//	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }
//
//	// choose pivot - or die 
//	if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
//	if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
//	if (0.0 == r1[1])  return false;
//
//	// eliminate second variable 
//	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
//	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
//	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
//	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
//	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
//	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
//	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }
//
//	// choose pivot - or die 
//	if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
//	if (0.0 == r2[2])  return false;
//
//	// eliminate third variable 
//	m3 = r3[2] / r2[2];
//	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
//		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6],
//		r3[7] -= m3 * r2[7];
//
//	// last check 
//	if (0.0 == r3[3]) return false;
//
//	s = 1.0f / r3[3];              // now back substitute row 3 
//	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;
//
//	m2 = r2[3];                 // now back substitute row 2 
//	s = 1.0f / r2[2];
//	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
//		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
//	m1 = r1[3];
//	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
//		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
//	m0 = r0[3];
//	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
//		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
//
//	m1 = r1[2];                 // now back substitute row 1 
//	s = 1.0f / r1[1];
//	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
//		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
//	m0 = r0[2];
//	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
//		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
//
//	m0 = r0[1];                 // now back substitute row 0 
//	s = 1.0f / r0[0];
//	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
//		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
//
//	MAT(dst, 0, 0) = r0[4]; MAT(dst, 0, 1) = r0[5],
//		MAT(dst, 0, 2) = r0[6]; MAT(dst, 0, 3) = r0[7],
//		MAT(dst, 1, 0) = r1[4]; MAT(dst, 1, 1) = r1[5],
//		MAT(dst, 1, 2) = r1[6]; MAT(dst, 1, 3) = r1[7],
//		MAT(dst, 2, 0) = r2[4]; MAT(dst, 2, 1) = r2[5],
//		MAT(dst, 2, 2) = r2[6]; MAT(dst, 2, 3) = r2[7],
//		MAT(dst, 3, 0) = r3[4]; MAT(dst, 3, 1) = r3[5],
//		MAT(dst, 3, 2) = r3[6]; MAT(dst, 3, 3) = r3[7];
//
//	return true;
//
//#undef MAT
//#undef SWAP_ROWS
//
//	return true;
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////
//// Get Window coordinates, discard Z...
//void m3dProjectXY(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector2f vPointOut)
//{
//	M3DVector4f vBack, vForth;
//
//	memcpy(vBack, vPointIn, sizeof(float) * 3);
//	vBack[3] = 1.0f;
//
//	m3dTransformVector4(vForth, vBack, mModelView);
//	m3dTransformVector4(vBack, vForth, mProjection);
//
//	if (!m3dCloseEnough(vBack[3], 0.0f, 0.000001f)) {
//		float div = 1.0f / vBack[3];
//		vBack[0] *= div;
//		vBack[1] *= div;
//	}
//
//
//	vPointOut[0] = vBack[0] * 0.5f + 0.5f;
//	vPointOut[1] = vBack[1] * 0.5f + 0.5f;
//
//	/* Map x,y to viewport */
//	vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
//	vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
//}
//
/////////////////////////////////////////////////////////////////////////////////////////
//// Get window coordinates, we also want Z....
//void m3dProjectXYZ(const M3DMatrix44f mModelView, const M3DMatrix44f mProjection, const int iViewPort[4], const M3DVector3f vPointIn, M3DVector3f vPointOut)
//{
//	M3DVector4f vBack, vForth;
//
//	memcpy(vBack, vPointIn, sizeof(float) * 3);
//	vBack[3] = 1.0f;
//
//	m3dTransformVector4(vForth, vBack, mModelView);
//	m3dTransformVector4(vBack, vForth, mProjection);
//
//	if (!m3dCloseEnough(vBack[3], 0.0f, 0.000001f)) {
//		float div = 1.0f / vBack[3];
//		vBack[0] *= div;
//		vBack[1] *= div;
//		vBack[2] *= div;
//	}
//
//	vPointOut[0] = vBack[0] * 0.5f + 0.5f;
//	vPointOut[1] = vBack[1] * 0.5f + 0.5f;
//	vPointOut[2] = vBack[2] * 0.5f + 0.5f;
//
//	/* Map x,y to viewport */
//	vPointOut[0] = (vPointOut[0] * iViewPort[2]) + iViewPort[0];
//	vPointOut[1] = (vPointOut[1] * iViewPort[3]) + iViewPort[1];
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//// Misc. Utilities
/////////////////////////////////////////////////////////////////////////////////
//// Calculates the normal of a triangle specified by the three points
//// p1, p2, and p3. Each pointer points to an array of three floats. The
//// triangle is assumed to be wound counter clockwise. 
//void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2,
//	const M3DVector3f point3)
//{
//	M3DVector3f v1, v2;		// Temporary vectors
//
//	// Calculate two vectors from the three points. Assumes counter clockwise
//	// winding!
//	v1[0] = point1[0] - point2[0];
//	v1[1] = point1[1] - point2[1];
//	v1[2] = point1[2] - point2[2];
//
//	v2[0] = point2[0] - point3[0];
//	v2[1] = point2[1] - point3[1];
//	v2[2] = point2[2] - point3[2];
//
//	// Take the cross product of the two vectors to get
//	// the normal vector.
//	m3dCrossProduct33(result, v1, v2);
//}
//
//
//
//// Ditto above, but for doubles
//void m3dFindNormal(M3DVector3d result, const M3DVector3d point1, const M3DVector3d point2,
//	const M3DVector3d point3)
//{
//	M3DVector3d v1, v2;		// Temporary vectors
//
//	// Calculate two vectors from the three points. Assumes counter clockwise
//	// winding!
//	v1[0] = point1[0] - point2[0];
//	v1[1] = point1[1] - point2[1];
//	v1[2] = point1[2] - point2[2];
//
//	v2[0] = point2[0] - point3[0];
//	v2[1] = point2[1] - point3[1];
//	v2[2] = point2[2] - point3[2];
//
//	// Take the cross product of the two vectors to get
//	// the normal vector.
//	m3dCrossProduct33(result, v1, v2);
//}
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////
//// Calculate the plane equation of the plane that the three specified points lay in. The
//// points are given in clockwise winding order, with normal pointing out of clockwise face
//// planeEq contains the A,B,C, and D of the plane equation coefficients
//void m3dGetPlaneEquation(M3DVector4f planeEq, const M3DVector3f p1, const M3DVector3f p2, const M3DVector3f p3)
//{
//	// Get two vectors... do the cross product
//	M3DVector3f v1, v2;
//
//	// V1 = p3 - p1
//	v1[0] = p3[0] - p1[0];
//	v1[1] = p3[1] - p1[1];
//	v1[2] = p3[2] - p1[2];
//
//	// V2 = P2 - p1
//	v2[0] = p2[0] - p1[0];
//	v2[1] = p2[1] - p1[1];
//	v2[2] = p2[2] - p1[2];
//
//	// Unit normal to plane - Not sure which is the best way here
//	m3dCrossProduct33(planeEq, v1, v2);
//	m3dNormalizeVector3(planeEq);
//	// Back substitute to get D
//	planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
//}
//
//
//// Ditto above, but for doubles
//void m3dGetPlaneEquation(M3DVector4d planeEq, const M3DVector3d p1, const M3DVector3d p2, const M3DVector3d p3)
//{
//	// Get two vectors... do the cross product
//	M3DVector3d v1, v2;
//
//	// V1 = p3 - p1
//	v1[0] = p3[0] - p1[0];
//	v1[1] = p3[1] - p1[1];
//	v1[2] = p3[2] - p1[2];
//
//	// V2 = P2 - p1
//	v2[0] = p2[0] - p1[0];
//	v2[1] = p2[1] - p1[1];
//	v2[2] = p2[2] - p1[2];
//
//	// Unit normal to plane - Not sure which is the best way here
//	m3dCrossProduct33(planeEq, v1, v2);
//	m3dNormalizeVector3(planeEq);
//	// Back substitute to get D
//	planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
//}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//// This function does a three dimensional Catmull-Rom curve interpolation. Pass four points, and a
//// floating point number between 0.0 and 1.0. The curve is interpolated between the middle two points.
//// Coded by RSW
//// http://www.mvps.org/directx/articles/catmull/
//void m3dCatmullRom3(M3DVector3f vOut, M3DVector3f vP0, M3DVector3f vP1, M3DVector3f vP2, M3DVector3f vP3, float t)
//{
//	// Unrolled loop to speed things up a little bit...
//	float t2 = t * t;
//	float t3 = t2 * t;
//
//	// X    
//	vOut[0] = 0.5f * ((2.0f * vP1[0]) +
//		(-vP0[0] + vP2[0]) * t +
//		(2.0f * vP0[0] - 5.0f * vP1[0] + 4.0f * vP2[0] - vP3[0]) * t2 +
//		(-vP0[0] + 3.0f * vP1[0] - 3.0f * vP2[0] + vP3[0]) * t3);
//	// Y
//	vOut[1] = 0.5f * ((2.0f * vP1[1]) +
//		(-vP0[1] + vP2[1]) * t +
//		(2.0f * vP0[1] - 5.0f * vP1[1] + 4.0f * vP2[1] - vP3[1]) * t2 +
//		(-vP0[1] + 3.0f * vP1[1] - 3.0f * vP2[1] + vP3[1]) * t3);
//
//	// Z
//	vOut[2] = 0.5f * ((2.0f * vP1[2]) +
//		(-vP0[2] + vP2[2]) * t +
//		(2.0f * vP0[2] - 5.0f * vP1[2] + 4.0f * vP2[2] - vP3[2]) * t2 +
//		(-vP0[2] + 3.0f * vP1[2] - 3.0f * vP2[2] + vP3[2]) * t3);
//}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
//// This function does a three dimensional Catmull-Rom curve interpolation. Pass four points, and a
//// floating point number between 0.0 and 1.0. The curve is interpolated between the middle two points.
//// Coded by RSW
//// http://www.mvps.org/directx/articles/catmull/
//void m3dCatmullRom3(M3DVector3d vOut, M3DVector3d vP0, M3DVector3d vP1, M3DVector3d vP2, M3DVector3d vP3, double t)
//{
//	// Unrolled loop to speed things up a little bit...
//	double t2 = t * t;
//	double t3 = t2 * t;
//
//	// X    
//	vOut[0] = 0.5 * ((2.0 * vP1[0]) +
//		(-vP0[0] + vP2[0]) * t +
//		(2.0 * vP0[0] - 5.0 * vP1[0] + 4.0 * vP2[0] - vP3[0]) * t2 +
//		(-vP0[0] + 3.0 * vP1[0] - 3.0 * vP2[0] + vP3[0]) * t3);
//	// Y
//	vOut[1] = 0.5 * ((2.0 * vP1[1]) +
//		(-vP0[1] + vP2[1]) * t +
//		(2.0 * vP0[1] - 5.0 * vP1[1] + 4.0 * vP2[1] - vP3[1]) * t2 +
//		(-vP0[1] + 3 * vP1[1] - 3.0 * vP2[1] + vP3[1]) * t3);
//
//	// Z
//	vOut[2] = 0.5 * ((2.0 * vP1[2]) +
//		(-vP0[2] + vP2[2]) * t +
//		(2.0 * vP0[2] - 5.0 * vP1[2] + 4.0 * vP2[2] - vP3[2]) * t2 +
//		(-vP0[2] + 3.0 * vP1[2] - 3.0 * vP2[2] + vP3[2]) * t3);
//}
//
//
/////////////////////////////////////////////////////////////////////////////////
//// Determine if the ray (starting at point) intersects the sphere centered at
//// sphereCenter with radius sphereRadius
//// Return value is < 0 if the ray does not intersect
//// Return value is 0.0 if ray is tangent
//// Positive value is distance to the intersection point
//// Algorithm from "3D Math Primer for Graphics and Game Development"
//double m3dRaySphereTest(const M3DVector3d point, const M3DVector3d ray, const M3DVector3d sphereCenter, double sphereRadius)
//{
//	//m3dNormalizeVector(ray);	// Make sure ray is unit length
//
//	M3DVector3d rayToCenter;	// Ray to center of sphere
//	rayToCenter[0] = sphereCenter[0] - point[0];
//	rayToCenter[1] = sphereCenter[1] - point[1];
//	rayToCenter[2] = sphereCenter[2] - point[2];
//
//	// Project rayToCenter on ray to test
//	double a = m3dDotProduct3(rayToCenter, ray);
//
//	// Distance to center of sphere
//	double distance2 = m3dDotProduct3(rayToCenter, rayToCenter);	// Or length
//
//
//	double dRet = (sphereRadius * sphereRadius) - distance2 + (a * a);
//
//	if (dRet > 0.0)			// Return distance to intersection
//		dRet = a - sqrt(dRet);
//
//	return dRet;
//}
//
/////////////////////////////////////////////////////////////////////////////////
//// Determine if the ray (starting at point) intersects the sphere centered at
//// ditto above, but uses floating point math
//float m3dRaySphereTest(const M3DVector3f point, const M3DVector3f ray, const M3DVector3f sphereCenter, float sphereRadius)
//{
//	//m3dNormalizeVectorf(ray);	// Make sure ray is unit length
//
//	M3DVector3f rayToCenter;	// Ray to center of sphere
//	rayToCenter[0] = sphereCenter[0] - point[0];
//	rayToCenter[1] = sphereCenter[1] - point[1];
//	rayToCenter[2] = sphereCenter[2] - point[2];
//
//	// Project rayToCenter on ray to test
//	float a = m3dDotProduct3(rayToCenter, ray);
//
//	// Distance to center of sphere
//	float distance2 = m3dDotProduct3(rayToCenter, rayToCenter);	// Or length
//
//	float dRet = (sphereRadius * sphereRadius) - distance2 + (a * a);
//
//	if (dRet > 0.0)			// Return distance to intersection
//		dRet = a - sqrtf(dRet);
//
//	return dRet;
//}
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
//// Calculate the tangent basis for a triangle on the surface of a model
//// This vector is needed for most normal mapping shaders 
//void m3dCalculateTangentBasis(const M3DVector3f vTriangle[3], const M3DVector2f vTexCoords[3], const M3DVector3f N, M3DVector3f vTangent)
//{
//	M3DVector3f dv2v1, dv3v1;
//	float dc2c1t, dc2c1b, dc3c1t, dc3c1b;
//	float M;
//
//	m3dSubtractVectors3(dv2v1, vTriangle[1], vTriangle[0]);
//	m3dSubtractVectors3(dv3v1, vTriangle[2], vTriangle[0]);
//
//	dc2c1t = vTexCoords[1][0] - vTexCoords[0][0];
//	dc2c1b = vTexCoords[1][1] - vTexCoords[0][1];
//	dc3c1t = vTexCoords[2][0] - vTexCoords[0][0];
//	dc3c1b = vTexCoords[2][1] - vTexCoords[0][1];
//
//	M = (dc2c1t * dc3c1b) - (dc3c1t * dc2c1b);
//	M = 1.0f / M;
//
//	m3dScaleVector3(dv2v1, dc3c1b);
//	m3dScaleVector3(dv3v1, dc2c1b);
//
//	m3dSubtractVectors3(vTangent, dv2v1, dv3v1);
//	m3dScaleVector3(vTangent, M);  // This potentially changes the direction of the vector
//	m3dNormalizeVector3(vTangent);
//
//	M3DVector3f B;
//	m3dCrossProduct33(B, N, vTangent);
//	m3dCrossProduct33(vTangent, B, N);
//	m3dNormalizeVector3(vTangent);
//}
//
//
//////////////////////////////////////////////////////////////////////////////
//// Smoothly step between 0 and 1 between edge1 and edge 2
//double m3dSmoothStep(double edge1, double edge2, double x)
//{
//	double t;
//	t = (x - edge1) / (edge2 - edge1);
//	if (t > 1.0)
//		t = 1.0;
//
//	if (t < 0.0)
//		t = 0.0f;
//
//	return t * t * (3.0 - 2.0 * t);
//}
//
//////////////////////////////////////////////////////////////////////////////
//// Smoothly step between 0 and 1 between edge1 and edge 2
//float m3dSmoothStep(float edge1, float edge2, float x)
//{
//	float t;
//	t = (x - edge1) / (edge2 - edge1);
//	if (t > 1.0f)
//		t = 1.0f;
//
//	if (t < 0.0)
//		t = 0.0f;
//
//	return t * t * (3.0f - 2.0f * t);
//}
//
//
//
/////////////////////////////////////////////////////////////////////////////
//// Creae a projection to "squish" an object into the plane.
//// Use m3dGetPlaneEquationf(planeEq, point1, point2, point3);
//// to get a plane equation.
//void m3dMakePlanarShadowMatrix(M3DMatrix44f proj, const M3DVector4f planeEq, const M3DVector3f vLightPos)
//{
//	// These just make the code below easier to read. They will be 
//	// removed by the optimizer.	
//	float a = planeEq[0];
//	float b = planeEq[1];
//	float c = planeEq[2];
//	float d = planeEq[3];
//
//	float dx = -vLightPos[0];
//	float dy = -vLightPos[1];
//	float dz = -vLightPos[2];
//
//	// Now build the projection matrix
//	proj[0] = b * dy + c * dz;
//	proj[1] = -a * dy;
//	proj[2] = -a * dz;
//	proj[3] = 0.0;
//
//	proj[4] = -b * dx;
//	proj[5] = a * dx + c * dz;
//	proj[6] = -b * dz;
//	proj[7] = 0.0;
//
//	proj[8] = -c * dx;
//	proj[9] = -c * dy;
//	proj[10] = a * dx + b * dy;
//	proj[11] = 0.0;
//
//	proj[12] = -d * dx;
//	proj[13] = -d * dy;
//	proj[14] = -d * dz;
//	proj[15] = a * dx + b * dy + c * dz;
//	// Shadow matrix ready
//}
//
//
/////////////////////////////////////////////////////////////////////////////
//// Creae a projection to "squish" an object into the plane.
//// Use m3dGetPlaneEquationd(planeEq, point1, point2, point3);
//// to get a plane equation.
//void m3dMakePlanarShadowMatrix(M3DMatrix44d proj, const M3DVector4d planeEq, const M3DVector3f vLightPos)
//{
//	// These just make the code below easier to read. They will be 
//	// removed by the optimizer.	
//	double a = planeEq[0];
//	double b = planeEq[1];
//	double c = planeEq[2];
//	double d = planeEq[3];
//
//	double dx = -vLightPos[0];
//	double dy = -vLightPos[1];
//	double dz = -vLightPos[2];
//
//	// Now build the projection matrix
//	proj[0] = b * dy + c * dz;
//	proj[1] = -a * dy;
//	proj[2] = -a * dz;
//	proj[3] = 0.0;
//
//	proj[4] = -b * dx;
//	proj[5] = a * dx + c * dz;
//	proj[6] = -b * dz;
//	proj[7] = 0.0;
//
//	proj[8] = -c * dx;
//	proj[9] = -c * dy;
//	proj[10] = a * dx + b * dy;
//	proj[11] = 0.0;
//
//	proj[12] = -d * dx;
//	proj[13] = -d * dy;
//	proj[14] = -d * dz;
//	proj[15] = a * dx + b * dy + c * dz;
//	// Shadow matrix ready
//}
//
//
///////////////////////////////////////////////////////////////////////////////
//// I want to know the point on a ray, closest to another given point in space.
//// As a bonus, return the distance squared of the two points.
//// In: vRayOrigin is the origin of the ray.
//// In: vUnitRayDir is the unit vector of the ray
//// In: vPointInSpace is the point in space
//// Out: vPointOnRay is the poing on the ray closest to vPointInSpace
//// Return: The square of the distance to the ray
//double m3dClosestPointOnRay(M3DVector3d vPointOnRay, const M3DVector3d vRayOrigin, const M3DVector3d vUnitRayDir,
//	const M3DVector3d vPointInSpace)
//{
//	M3DVector3d v;
//	m3dSubtractVectors3(v, vPointInSpace, vRayOrigin);
//
//	double t = m3dDotProduct3(vUnitRayDir, v);
//
//	// This is the point on the ray
//	vPointOnRay[0] = vRayOrigin[0] + (t * vUnitRayDir[0]);
//	vPointOnRay[1] = vRayOrigin[1] + (t * vUnitRayDir[1]);
//	vPointOnRay[2] = vRayOrigin[2] + (t * vUnitRayDir[2]);
//
//	return m3dGetDistanceSquared(vPointOnRay, vPointInSpace);
//}
//
//// ditto above... but with floats
//float m3dClosestPointOnRay(M3DVector3f vPointOnRay, const M3DVector3f vRayOrigin, const M3DVector3f vUnitRayDir,
//	const M3DVector3f vPointInSpace)
//{
//	M3DVector3f v;
//	m3dSubtractVectors3(v, vPointInSpace, vRayOrigin);
//
//	float t = m3dDotProduct3(vUnitRayDir, v);
//
//	// This is the point on the ray
//	vPointOnRay[0] = vRayOrigin[0] + (t * vUnitRayDir[0]);
//	vPointOnRay[1] = vRayOrigin[1] + (t * vUnitRayDir[1]);
//	vPointOnRay[2] = vRayOrigin[2] + (t * vUnitRayDir[2]);
//
//	return m3dGetDistanceSquared(vPointOnRay, vPointInSpace);
//}
//
//#pragma endregion
//
//#pragma region glframe
//
//// Frame.h
//// Implementation of the GLFrame Class
//// Richard S. Wright Jr.
//// Code by Richard S. Wright Jr.
//
//#ifndef _ORTHO_FRAME_
//#define _ORTHO_FRAME_
//
//// The GLFrame (OrthonormalFrame) class. Possibly the most useful little piece of 3D graphics
//// code for OpenGL immersive environments.
//// Richard S. Wright Jr.
//class GLFrame
//{
//protected:
//	M3DVector3f vOrigin;	// Where am I?
//	M3DVector3f vForward;	// Where am I going?
//	M3DVector3f vUp;		// Which way is up?
//
//public:
//	// Default position and orientation. At the origin, looking
//	// down the positive Z axis (right handed coordinate system).
//	GLFrame(void) {
//		// At origin
//		vOrigin[0] = 0.0f; vOrigin[1] = 0.0f; vOrigin[2] = 0.0f;
//
//		// Up is up (+Y)
//		vUp[0] = 0.0f; vUp[1] = 1.0f; vUp[2] = 0.0f;
//
//		// Forward is -Z (default OpenGL)
//		vForward[0] = 0.0f; vForward[1] = 0.0f; vForward[2] = -1.0f;
//	}
//
//
//	/////////////////////////////////////////////////////////////
//	// Set Location
//	inline void SetOrigin(const M3DVector3f vPoint) {
//		m3dCopyVector3(vOrigin, vPoint);
//	}
//
//	inline void SetOrigin(float x, float y, float z) {
//		vOrigin[0] = x; vOrigin[1] = y; vOrigin[2] = z;
//	}
//
//	inline void GetOrigin(M3DVector3f vPoint) {
//		m3dCopyVector3(vPoint, vOrigin);
//	}
//
//	inline float GetOriginX(void) { return vOrigin[0]; }
//	inline float GetOriginY(void) { return vOrigin[1]; }
//	inline float GetOriginZ(void) { return vOrigin[2]; }
//
//	/////////////////////////////////////////////////////////////
//	// Set Forward Direction
//	inline void SetForwardVector(const M3DVector3f vDirection) {
//		m3dCopyVector3(vForward, vDirection);
//	}
//
//	inline void SetForwardVector(float x, float y, float z)
//	{
//		vForward[0] = x; vForward[1] = y; vForward[2] = z;
//	}
//
//	inline void GetForwardVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vForward); }
//
//	/////////////////////////////////////////////////////////////
//	// Set Up Direction
//	inline void SetUpVector(const M3DVector3f vDirection) {
//		m3dCopyVector3(vUp, vDirection);
//	}
//
//	inline void SetUpVector(float x, float y, float z)
//	{
//		vUp[0] = x; vUp[1] = y; vUp[2] = z;
//	}
//
//	inline void GetUpVector(M3DVector3f vVector) { m3dCopyVector3(vVector, vUp); }
//
//
//	/////////////////////////////////////////////////////////////
//	// Get Axes
//	inline void GetZAxis(M3DVector3f vVector) { GetForwardVector(vVector); }
//	inline void GetYAxis(M3DVector3f vVector) { GetUpVector(vVector); }
//	inline void GetXAxis(M3DVector3f vVector) { m3dCrossProduct33(vVector, vUp, vForward); }
//
//
//	/////////////////////////////////////////////////////////////
//	// Translate along orthonormal axis... world or local
//	inline void TranslateWorld(float x, float y, float z)
//	{
//		vOrigin[0] += x; vOrigin[1] += y; vOrigin[2] += z;
//	}
//
//	inline void TranslateLocal(float x, float y, float z)
//	{
//		MoveForward(z); MoveUp(y); MoveRight(x);
//	}
//
//
//	/////////////////////////////////////////////////////////////
//	// Move Forward (along Z axis)
//	inline void MoveForward(float fDelta)
//	{
//		// Move along direction of front direction
//		vOrigin[0] += vForward[0] * fDelta;
//		vOrigin[1] += vForward[1] * fDelta;
//		vOrigin[2] += vForward[2] * fDelta;
//	}
//
//	// Move along Y axis
//	inline void MoveUp(float fDelta)
//	{
//		// Move along direction of up direction
//		vOrigin[0] += vUp[0] * fDelta;
//		vOrigin[1] += vUp[1] * fDelta;
//		vOrigin[2] += vUp[2] * fDelta;
//	}
//
//	// Move along X axis
//	inline void MoveRight(float fDelta)
//	{
//		// Move along direction of right vector
//		M3DVector3f vCross;
//		m3dCrossProduct33(vCross, vUp, vForward);
//
//		vOrigin[0] += vCross[0] * fDelta;
//		vOrigin[1] += vCross[1] * fDelta;
//		vOrigin[2] += vCross[2] * fDelta;
//	}
//	///////////////////////////////////////////////////////////////////////
//	// Just assemble the matrix
//	void GetMatrix(M3DMatrix44f	matrix, bool bRotationOnly = false)
//	{
//		// Calculate the right side (x) vector, drop it right into the matrix
//		M3DVector3f vXAxis;
//		m3dCrossProduct33(vXAxis, vUp, vForward);
//
//		// Set matrix column does not fill in the fourth value...
//		m3dSetMatrixColumn44(matrix, vXAxis, 0);
//		matrix[3] = 0.0f;
//
//		// Y Column
//		m3dSetMatrixColumn44(matrix, vUp, 1);
//		matrix[7] = 0.0f;
//
//		// Z Column
//		m3dSetMatrixColumn44(matrix, vForward, 2);
//		matrix[11] = 0.0f;
//
//		// Translation (already done)
//		if (bRotationOnly == true)
//		{
//			matrix[12] = 0.0f;
//			matrix[13] = 0.0f;
//			matrix[14] = 0.0f;
//		}
//		else
//			m3dSetMatrixColumn44(matrix, vOrigin, 3);
//
//		matrix[15] = 1.0f;
//	}
//
//
//	/////////////////////////////////////////////////////////////
//	// Get a 4x4 transformation matrix that describes the ccamera
//	// orientation.
//	inline void GetCameraOrientation(M3DMatrix44f m)
//	{
//		M3DVector3f x, z;
//
//		// Make rotation matrix
//		// Z vector is reversed
//		z[0] = -vForward[0];
//		z[1] = -vForward[1];
//		z[2] = -vForward[2];
//
//		// X vector = Y cross Z 
//		m3dCrossProduct33(x, vUp, z);
//
//		// Matrix has no translation information and is
//		// transposed.... (rows instead of columns)
//#define M(row,col)  m[col*4+row]
//		M(0, 0) = x[0];
//		M(0, 1) = x[1];
//		M(0, 2) = x[2];
//		M(0, 3) = 0.0;
//		M(1, 0) = vUp[0];
//		M(1, 1) = vUp[1];
//		M(1, 2) = vUp[2];
//		M(1, 3) = 0.0;
//		M(2, 0) = z[0];
//		M(2, 1) = z[1];
//		M(2, 2) = z[2];
//		M(2, 3) = 0.0;
//		M(3, 0) = 0.0;
//		M(3, 1) = 0.0;
//		M(3, 2) = 0.0;
//		M(3, 3) = 1.0;
//#undef M 
//	}
//
//
//	/////////////////////////////////////////////////////////////
//	// Perform viewing or modeling transformations
//	// Position as the camera (for viewing). Apply this transformation
//	// first as your viewing transformation
//	// The default implementation of gluLookAt can be considerably sped up
//	// since it uses doubles for everything... then again profile before you
//	// tune... ;-) You might get a boost form page fault reduction too... if
//	// no other glu routines are used...
//	// This will get called once per frame.... go ahead and inline
//	inline void ApplyCameraTransform(bool bRotOnly = false)
//	{
//		M3DMatrix44f m;
//
//		GetCameraOrientation(m);
//
//		// Camera Transform   
//		glMultMatrixf(m);
//
//		// If Rotation only, then do not do the translation
//		if (!bRotOnly)
//			glTranslatef(-vOrigin[0], -vOrigin[1], -vOrigin[2]);
//
//		/*gluLookAt(vOrigin[0], vOrigin[1], vOrigin[2],
//					vOrigin[0] + vForward[0],
//					vOrigin[1] + vForward[1],
//					vOrigin[2] + vForward[2],
//					vUp[0], vUp[1], vUp[2]);
//		*/
//	}
//
//
//	// Position as an object in the scene. This places and orients a
//	// coordinate frame for other objects (besides the camera)
//	// There is ample room for optimization here... 
//	// This is going to be called alot... don't inline
//	// Add flag to perform actor rotation only and not the translation
//	void ApplyActorTransform(bool bRotationOnly = false)
//	{
//		M3DMatrix44f rotMat;
//
//		GetMatrix(rotMat, bRotationOnly);
//
//		// Apply rotation to the current matrix
//		glMultMatrixf(rotMat);
//	}
//
//
//	// Rotate around local X Axes - Note all rotations are in radians
//	void RotateLocalX(float fAngle)
//	{
//		M3DMatrix44f rotMat;
//		M3DVector3f vCross;
//		m3dCrossProduct33(vCross, vUp, vForward);
//		m3dRotationMatrix44(rotMat, fAngle,
//			vCross[0], vCross[1], vCross[2]);
//
//		M3DVector3f newVect;
//		// Inline 3x3 matrix multiply for rotation only
//		newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] * vForward[2];
//		newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] * vForward[2];
//		newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
//		m3dCopyVector3(vForward, newVect);
//
//		// Update pointing up vector
//		newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] * vUp[2];
//		newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] * vUp[2];
//		newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];
//		m3dCopyVector3(vUp, newVect);
//	}
//
//	// Rotate around local Y
//	void RotateLocalY(float fAngle)
//	{
//		M3DMatrix44f rotMat;
//
//		// Just Rotate around the up vector
//		// Create a rotation matrix around my Up (Y) vector
//		m3dRotationMatrix44(rotMat, fAngle,
//			vUp[0], vUp[1], vUp[2]);
//
//		M3DVector3f newVect;
//
//		// Rotate forward pointing vector (inlined 3x3 transform)
//		newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] * vForward[2];
//		newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] * vForward[2];
//		newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
//		m3dCopyVector3(vForward, newVect);
//	}
//
//
//	// Rotate around local Z
//	void RotateLocalZ(float fAngle)
//	{
//		M3DMatrix44f rotMat;
//
//		// Only the up vector needs to be rotated
//		m3dRotationMatrix44(rotMat, fAngle,
//			vForward[0], vForward[1], vForward[2]);
//
//		M3DVector3f newVect;
//		newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] * vUp[2];
//		newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] * vUp[2];
//		newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];
//		m3dCopyVector3(vUp, newVect);
//	}
//
//
//	// Reset axes to make sure they are orthonormal. This should be called on occasion
//	// if the matrix is long-lived and frequently transformed.
//	void Normalize(void)
//	{
//		M3DVector3f vCross;
//
//		// Calculate cross product of up and forward vectors
//		m3dCrossProduct33(vCross, vUp, vForward);
//
//		// Use result to recalculate forward vector
//		m3dCrossProduct33(vForward, vCross, vUp);
//
//		// Also check for unit length...
//		m3dNormalizeVector3(vUp);
//		m3dNormalizeVector3(vForward);
//	}
//
//
//	// Rotate in world coordinates...
//	void RotateWorld(float fAngle, float x, float y, float z)
//	{
//		M3DMatrix44f rotMat;
//
//		// Create the Rotation matrix
//		m3dRotationMatrix44(rotMat, fAngle, x, y, z);
//
//		M3DVector3f newVect;
//
//		// Transform the up axis (inlined 3x3 rotation)
//		newVect[0] = rotMat[0] * vUp[0] + rotMat[4] * vUp[1] + rotMat[8] * vUp[2];
//		newVect[1] = rotMat[1] * vUp[0] + rotMat[5] * vUp[1] + rotMat[9] * vUp[2];
//		newVect[2] = rotMat[2] * vUp[0] + rotMat[6] * vUp[1] + rotMat[10] * vUp[2];
//		m3dCopyVector3(vUp, newVect);
//
//		// Transform the forward axis
//		newVect[0] = rotMat[0] * vForward[0] + rotMat[4] * vForward[1] + rotMat[8] * vForward[2];
//		newVect[1] = rotMat[1] * vForward[0] + rotMat[5] * vForward[1] + rotMat[9] * vForward[2];
//		newVect[2] = rotMat[2] * vForward[0] + rotMat[6] * vForward[1] + rotMat[10] * vForward[2];
//		m3dCopyVector3(vForward, newVect);
//	}
//
//
//	// Rotate around a local axis
//	void RotateLocal(float fAngle, float x, float y, float z)
//	{
//		M3DVector3f vWorldVect;
//		M3DVector3f vLocalVect;
//		m3dLoadVector3(vLocalVect, x, y, z);
//
//		LocalToWorld(vLocalVect, vWorldVect);
//		RotateWorld(fAngle, vWorldVect[0], vWorldVect[1], vWorldVect[2]);
//	}
//
//
//	// Convert Coordinate Systems
//	// This is pretty much, do the transformation represented by the rotation
//	// and position on the point
//	// Is it better to stick to the convention that the destination always comes
//	// first, or use the conventions that "sounds" like the function...
//	void LocalToWorld(const M3DVector3f vLocal, M3DVector3f vWorld)
//	{
//		// Create the rotation matrix based on the vectors
//		M3DMatrix44f rotMat;
//
//		GetMatrix(rotMat, true);
//
//		// Do the rotation (inline it, and remove 4th column...)
//		vWorld[0] = rotMat[0] * vLocal[0] + rotMat[4] * vLocal[1] + rotMat[8] * vLocal[2];
//		vWorld[1] = rotMat[1] * vLocal[0] + rotMat[5] * vLocal[1] + rotMat[9] * vLocal[2];
//		vWorld[2] = rotMat[2] * vLocal[0] + rotMat[6] * vLocal[1] + rotMat[10] * vLocal[2];
//
//		// Translate the point
//		vWorld[0] += vOrigin[0];
//		vWorld[1] += vOrigin[1];
//		vWorld[2] += vOrigin[2];
//	}
//
//	// Change world coordinates into "local" coordinates
//	void WorldToLocal(const M3DVector3f vWorld, M3DVector3f vLocal)
//	{
//		////////////////////////////////////////////////
//		// Translate the origin
//		M3DVector3f vNewWorld;
//		vNewWorld[0] = vWorld[0] - vOrigin[0];
//		vNewWorld[1] = vWorld[1] - vOrigin[1];
//		vNewWorld[2] = vWorld[2] - vOrigin[2];
//
//		// Create the rotation matrix based on the vectors
//		M3DMatrix44f rotMat;
//		M3DMatrix44f invMat;
//		GetMatrix(rotMat, true);
//
//		// Do the rotation based on inverted matrix
//		m3dInvertMatrix44(invMat, rotMat);
//
//		vLocal[0] = invMat[0] * vNewWorld[0] + invMat[4] * vNewWorld[1] + invMat[8] * vNewWorld[2];
//		vLocal[1] = invMat[1] * vNewWorld[0] + invMat[5] * vNewWorld[1] + invMat[9] * vNewWorld[2];
//		vLocal[2] = invMat[2] * vNewWorld[0] + invMat[6] * vNewWorld[1] + invMat[10] * vNewWorld[2];
//	}
//
//	/////////////////////////////////////////////////////////////////////////////
//	// Transform a point by frame matrix
//	void TransformPoint(M3DVector3f vPointSrc, M3DVector3f vPointDst)
//	{
//		M3DMatrix44f m;
//		GetMatrix(m, false);    // Rotate and translate
//		vPointDst[0] = m[0] * vPointSrc[0] + m[4] * vPointSrc[1] + m[8] * vPointSrc[2] + m[12];// * v[3];	 
//		vPointDst[1] = m[1] * vPointSrc[0] + m[5] * vPointSrc[1] + m[9] * vPointSrc[2] + m[13];// * v[3];	
//		vPointDst[2] = m[2] * vPointSrc[0] + m[6] * vPointSrc[1] + m[10] * vPointSrc[2] + m[14];// * v[3];	
//	}
//
//	////////////////////////////////////////////////////////////////////////////
//	// Rotate a vector by frame matrix
//	void RotateVector(M3DVector3f vVectorSrc, M3DVector3f vVectorDst)
//	{
//		M3DMatrix44f m;
//		GetMatrix(m, true);    // Rotate only
//
//		vVectorDst[0] = m[0] * vVectorSrc[0] + m[4] * vVectorSrc[1] + m[8] * vVectorSrc[2];
//		vVectorDst[1] = m[1] * vVectorSrc[0] + m[5] * vVectorSrc[1] + m[9] * vVectorSrc[2];
//		vVectorDst[2] = m[2] * vVectorSrc[0] + m[6] * vVectorSrc[1] + m[10] * vVectorSrc[2];
//	}
//};
//
//
//#endif
//
//
//
//#pragma endregion
//
//#define GL_CLAMP_TO_EDGE 0x812F
//
//#define NUM_SPHERES      30
//GLFrame    spheres[NUM_SPHERES];
//GLFrame    frameCamera;
//
//// Light and material Data
//GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
//GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
//GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
//GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//
//M3DMatrix44f mShadowMatrix;
//
//#define GROUND_TEXTURE  0
//#define TORUS_TEXTURE   1
//#define SPHERE_TEXTURE  2
//#define NUM_TEXTURES    3
//GLuint  textureObjects[NUM_TEXTURES];
//
//const char *szTextureFiles[] = {"../material/grass.tga", "../material/wood.tga", "../material/orb.tga"};
//
//        
////////////////////////////////////////////////////////////////////
//// This function does any needed initialization on the rendering
//// context. 
//void SetupRC()
//    {
//    M3DVector3f vPoints[3] = {{ 0.0f, -0.4f, 0.0f },
//                             { 10.0f, -0.4f, 0.0f },
//                             { 5.0f, -0.4f, -5.0f }};
//    int iSphere;
//    int i;
//    
//    // Grayish background
//    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);
//   
//    // Clear stencil buffer with zero, increment by one whenever anybody
//    // draws into it. When stencil function is enabled, only write where
//    // stencil value is zero. This prevents the transparent shadow from drawing
//    // over itself
//    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
//    glClearStencil(0);
//    glStencilFunc(GL_EQUAL, 0x0, 0x01);
//    
//    // Cull backs of polygons
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CCW);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
//    //glEnable(GL_MULTISAMPLE_ARB);
//    
//    // Setup light parameters
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//        
//    // Calculate shadow matrix
//    M3DVector4f pPlane;
//    m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
//    m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);
//    
//    // Mostly use material tracking
//    glEnable(GL_COLOR_MATERIAL);
//    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
//    glMateriali(GL_FRONT, GL_SHININESS, 128);
//  
//    
//    // Randomly place the sphere inhabitants
//    for(iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
//        {
//        // Pick a random location between -20 and 20 at .1 increments
//        spheres[iSphere].SetOrigin(((float)((rand() % 400) - 200) * 0.1f), 0.0, (float)((rand() % 400) - 200) * 0.1f);
//        }
//      
//    // Set up texture maps
//    glEnable(GL_TEXTURE_2D);
//    glGenTextures(NUM_TEXTURES, textureObjects);
//    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//    
//    
//    for(i = 0; i < NUM_TEXTURES; i++)
//        {
//        GLbyte *pBytes;
//        GLint iWidth, iHeight, iComponents;
//        GLenum eFormat;
//        
//        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
//        
//        // Load this texture map
//        pBytes = gltLoadTGA(szTextureFiles[i], &iWidth, &iHeight, &iComponents, &eFormat);
//        gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
//        free(pBytes);
//        
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        }
//
//    }
//
//////////////////////////////////////////////////////////////////////////
//// Do shutdown for the rendering context
//void ShutdownRC(void)
//    {
//    // Delete the textures
//    glDeleteTextures(NUM_TEXTURES, textureObjects);
//    }
//
//
/////////////////////////////////////////////////////////////
//// Draw the ground as a series of triangle strips
//void DrawGround(void)
//    {
//    GLfloat fExtent = 20.0f;
//    GLfloat fStep = 1.0f;
//    GLfloat y = -0.4f;
//    GLfloat iStrip, iRun;
//    GLfloat s = 0.0f;
//    GLfloat t = 0.0f;
//    GLfloat texStep = 1.0f / (fExtent * .075f);
//    
//    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    
//    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
//        {
//        t = 0.0f;
//        glBegin(GL_TRIANGLE_STRIP);
//
//            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
//                {
//                glTexCoord2f(s, t);
//                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
//                glVertex3f(iStrip, y, iRun);
//                
//                glTexCoord2f(s + texStep, t);
//                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
//                glVertex3f(iStrip + fStep, y, iRun);
//                
//                t += texStep;
//                }
//        glEnd();
//        s += texStep;
//        }
//    }
//
/////////////////////////////////////////////////////////////////////////
//// Draw random inhabitants and the rotating torus/sphere duo
//void DrawInhabitants(GLint nShadow)
//    {
//    static GLfloat yRot = 0.0f;         // Rotation angle for animation
//    GLint i;
//
//    if(nShadow == 0)
//        {
//        yRot += 0.5f;
//        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//        }
//    else
//        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color
//  
//        
//    // Draw the randomly located spheres
//    glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
//    for(i = 0; i < NUM_SPHERES; i++)
//        {
//        glPushMatrix();
//        spheres[i].ApplyActorTransform();
//        gltDrawSphere(0.3f, 21, 11);
//        glPopMatrix();
//        }
//
//    glPushMatrix();
//        glTranslatef(0.0f, 0.1f, -2.5f);
//    
//        glPushMatrix();
//            glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
//            glTranslatef(1.0f, 0.0f, 0.0f);
//            gltDrawSphere(0.1f,21, 11);
//        glPopMatrix();
//    
//        if(nShadow == 0)
//            {
//            // Torus alone will be specular
//            glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
//            }
//        
//        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
//        glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
//        gltDrawTorus(0.35, 0.15, 61, 37);
//        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
//    glPopMatrix();
//    }
//
//        
//// Called to draw scene
//void RenderScene(void)
//    {
//    // Clear the window with current clearing color
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//        
//    glPushMatrix();
//        frameCamera.ApplyCameraTransform();
//        
//        // Position light before any other transformations
//        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
//        
//        // Draw the ground
//        glColor3f(1.0f, 1.0f, 1.0f);
//        DrawGround();
//        
//        // Draw shadows first
//        glDisable(GL_DEPTH_TEST);
//        glDisable(GL_LIGHTING);
//        glDisable(GL_TEXTURE_2D);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glEnable(GL_STENCIL_TEST);
//        glPushMatrix();
//            glMultMatrixf(mShadowMatrix);
//            DrawInhabitants(1);
//        glPopMatrix();
//        glDisable(GL_STENCIL_TEST);
//        glDisable(GL_BLEND);
//        glEnable(GL_LIGHTING);
//        glEnable(GL_TEXTURE_2D);
//        glEnable(GL_DEPTH_TEST);
//        
//        // Draw inhabitants normally
//        DrawInhabitants(0);
//
//    glPopMatrix();
//        
//    // Do the buffer Swap
//    glutSwapBuffers();
//    }
//
//
//
//// Respond to arrow keys by moving the camera frame of reference
//void SpecialKeys(int key, int x, int y)
//    {
//    if(key == GLUT_KEY_UP)
//        frameCamera.MoveForward(0.1f);
//
//    if(key == GLUT_KEY_DOWN)
//		frameCamera.MoveForward(-0.1f);
//
//    if(key == GLUT_KEY_LEFT)
//		frameCamera.RotateLocalY(0.1f);
//	   
//    if(key == GLUT_KEY_RIGHT)
//        frameCamera.RotateLocalY(-0.1f);
//                        
//    // Refresh the Window
//    glutPostRedisplay();
//    }
//
/////////////////////////////////////////////////////////////
//// Called by GLUT library when idle (window not being
//// resized or moved)
//void TimerFunction(int value)
//    {
//    // Redraw the scene with new coordinates
//    glutPostRedisplay();
//    glutTimerFunc(3,TimerFunction, 1);
//    }
//
//void ChangeSize(int w, int h)
//    {
//    GLfloat fAspect;
//
//    // Prevent a divide by zero, when window is too short
//    // (you cant make a window of zero width).
//    if(h == 0)
//        h = 1;
//
//    glViewport(0, 0, w, h);
//        
//    fAspect = (GLfloat)w / (GLfloat)h;
//
//    // Reset the coordinate system before modifying
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//	
//    // Set the clipping volume
//    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
//        
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();    
//    }
//
//int main(int argc, char* argv[])
//    {
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
//    glutInitWindowSize(800,600);
//    glutCreateWindow("OpenGL SphereWorld Demo + Texture Maps");
//    glutReshapeFunc(ChangeSize);
//    glutDisplayFunc(RenderScene);
//    glutSpecialFunc(SpecialKeys);
//
//    SetupRC();
//    glutTimerFunc(33, TimerFunction, 1);
//
//    glutMainLoop();
//    
//    ShutdownRC();
//
//    return 0;
//    }
