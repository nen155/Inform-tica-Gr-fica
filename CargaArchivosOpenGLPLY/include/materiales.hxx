#include "materiales.h"

materiales::materiales( void ) {
  Alpha = 1.0f; Phongsize = 32.0f; Blend = false;
  Ambient[0]  = 1.0f; Ambient[1]  = 1.0f; Ambient[2]  = 1.0f; Ambient[3]  = 1.0f;
  Diffuse[0]  = 1.0f; Diffuse[1]  = 1.0f; Diffuse[2]  = 1.0f; Diffuse[3]  = 1.0f;
  Specular[0] = 1.0f; Specular[1] = 1.0f; Specular[2] = 1.0f; Specular[3] = 1.0f;
  Emissive[0] = 0.0f; Emissive[1] = 0.0f; Emissive[2] = 0.0f; Emissive[3] = 0.0f;
  Shininess[0]=5.0f;
}

materiales::~materiales( void ) {
  // cerr << "Materiales Destruido." << endl;
}

void materiales::emissive( GLfloat r, GLfloat g, GLfloat b ) { 
  Emissive[0]= r; Emissive[1]= g; Emissive[2]= b; 
  if ( ( Emissive[0] + Emissive[1] + Emissive[2] ) > 0.0f ) Emissive[3] = Alpha;
}
void materiales::shininess(GLfloat s){
	Shininess[0]=s;
}

void materiales::alpha( GLfloat a ) { 
  Alpha = a; 
  Diffuse[3] = Alpha; Ambient[3] = Alpha; Specular[3] = Alpha; 
  if ( ( Emissive[0] + Emissive[1] + Emissive[2] ) > 0.0f ) Emissive[3] = Alpha;
  if ( Alpha < 1.0f ) Blend = true; else Blend = false;
}
    
void materiales::activa( void ) {
  glColor4fv( Diffuse );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   Diffuse   );
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   Ambient   );
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  Specular  );
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  Emissive  );
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, Phongsize );
  if ( Blend ) { glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable( GL_BLEND ); }
}

void materiales::desactiva( void ) {
  if ( Blend ) glDisable( GL_BLEND );
}
