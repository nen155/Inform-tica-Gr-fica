#ifndef _LUCES_
#define _LUCES_

#include <iostream>
#include <GL/gl.h>										
#include <GL/glu.h>
using namespace std;										

class luces {
  public:
    luces( void );
    ~luces( void );

    void diffuse(  GLfloat r, GLfloat g, GLfloat b )  { Diffuse[0] = r; Diffuse[1] = g; Diffuse[2] = b; Diffuse[3] = 1.0f; }
    void ambient(  GLfloat r, GLfloat g, GLfloat b )  { Ambient[0] = r; Ambient[1] = g; Ambient[2] = b; Ambient[3] = 1.0f; }
    void specular( GLfloat r, GLfloat g, GLfloat b )  { Specular[0]= r; Specular[1]= g; Specular[2]= b; Specular[3]= 1.0f; }
    void position( GLfloat x, GLfloat y, GLfloat z )  { Position[0]= x; Position[1]= y; Position[2]= z; }
    GLfloat *position( void ) { return Position; }
    void direction( GLfloat x, GLfloat y, GLfloat z ) { Direction[0]= x; Direction[1]= y; Direction[2]= z; }
    void obertura(        GLfloat valor ) { Spot_CutOff   = valor; }
    void atenuacion_foco( GLfloat valor ) { Spot_Exponent = valor; }
    
    void direccional( GLenum Luz_N );
    void puntual(     GLenum Luz_N );
    void focal(       GLenum Luz_N );
    void posiciona( void );
    void direcciona( void );
     
    void ver_direccional( void );
    void ver_puntual( void );
    void ver_focal( void );
    void activa(int num){ 
	glEnable(GL_LIGHTING);// Activamos la iluminación.
	glEnable(num);// Ahora ponemos la luz que ya definimos 
    };
    void desactiva(int num){ 
	//glDisable(GL_LIGHTING);// Activamos la iluminación.
	glDisable(num);// Ahora ponemos la luz que ya definimos 
    };

  private:
    GLfloat Diffuse[4], Ambient[4], Specular[4], Position[4], Direction[3];
    GLfloat Spot_CutOff, Spot_Exponent;
    GLenum Luz_Numero;
};

#endif
