
#ifndef _GLINCLUDES
#define _GLINCLUDES


#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "math.h"



// Macros 

#ifndef _PI 
#define	_PI (3.14159265359)
#endif


#define RAD2DEG(a) ((GLdouble)(a) / _PI * 180)
#define DEG2RAD(a) ((GLdouble)(a) / 180 * _PI)


#define sqr(a) ((a) * (a))


#ifdef ASSERT

	#define GL_VERIFY(a)							\
		do {										\
			a;										\
			ASSERT (glGetError() == GL_NO_ERROR);	\
		} while (0)									\

#else

	static char szBuff[60];
	#define GL_VERIFY(a)													\
		do {																\
			a;																\
			if (glGetError() != GL_NO_ERROR)								\
			{																\
				sprintf(szBuff, "File '%s', line %i", THIS_FILE, __LINE__);	\
				::MessageBox(NULL, szBuff, "OpenGL error", MB_OK);			\
				::PostQuitMessage(-1);										\
			}																\
		} while (0)															\

#endif




// Structure and types

struct GLpoint {
	GLfloat x, y, z;
};


struct GLFace {
	unsigned short v1, v2, v3;
};



#endif // #ifndef _GLINCLUDES