#ifndef _MATERIALES_
#define _MATERIALES_

#include <iostream>
#include <GL/gl.h>										
#include <GL/glu.h>										
using namespace std;

class materiales {
  public:
    materiales( void );
    ~materiales( void );
    void diffuse(  GLfloat r, GLfloat g, GLfloat b ) { Diffuse[0] = r; Diffuse[1] = g; Diffuse[2] = b; }
    void ambient(  GLfloat r, GLfloat g, GLfloat b ) { Ambient[0] = r; Ambient[1] = g; Ambient[2] = b; }
    void specular( GLfloat r, GLfloat g, GLfloat b ) { Specular[0]= r; Specular[1]= g; Specular[2]= b; }
    void emissive( GLfloat r, GLfloat g, GLfloat b );
    void shininess(GLfloat s);
    void alpha( GLfloat a );
    bool es_Alpha( void )     { return Blend; }
    void phongsize( GLfloat p ) { Phongsize = p; }
    void activa( void );
    void desactiva( void );
  private:
    GLfloat Ambient[4], Diffuse[4], Specular[4], Emissive[4];
    GLfloat Alpha, Phongsize;
    GLfloat Shininess[1];
    bool  Blend;
};

#endif
