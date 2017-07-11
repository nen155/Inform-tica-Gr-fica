#include "luces.h"

luces::luces( void ) {
  Diffuse[0]  =  1.0f; Diffuse[1]  =  1.0f; Diffuse[2]  =  1.0f; Diffuse[3]  = 1.0f;
  Ambient[0]  =  0.2f; Ambient[1]  =  0.2f; Ambient[2]  =  0.2f; Ambient[3]  = 1.0f;
  Specular[0] =  1.0f; Specular[1] =  1.0f; Specular[2] =  1.0f; Specular[3] = 1.0f;
  Position[0] =  5.0f; Position[1] =  5.0f; Position[2] =  5.0f; Position[3] = 0.0f; 
  Direction[0]= -1.0f; Direction[1]= -1.0f; Direction[2]= -1.0f;
  Spot_CutOff = 45.0f; Spot_Exponent =  1.0f; Luz_Numero = GL_LIGHT0;
}

luces::~luces( void ) {
  // cerr << "Luces Destruido." << endl;
}

void luces::direccional( GLenum Luz_N ) {
  Luz_Numero = Luz_N;
  Position[3] = 0.0f;
  glLightfv( Luz_Numero, GL_DIFFUSE,  Diffuse  );
  glLightfv( Luz_Numero, GL_AMBIENT,  Ambient  );
  glLightfv( Luz_Numero, GL_SPECULAR, Specular );
  glLightfv( Luz_Numero, GL_POSITION, Position );
}

void luces::puntual( GLenum Luz_N ) {
  Luz_Numero = Luz_N;
  Position[3] = 1.0f;
  glLightfv( Luz_Numero, GL_DIFFUSE,  Diffuse  );
  glLightfv( Luz_Numero, GL_AMBIENT,  Ambient  );
  glLightfv( Luz_Numero, GL_SPECULAR, Specular );
  glLightfv( Luz_Numero, GL_POSITION, Position );
}

void luces::focal( GLenum Luz_N ) {
  Luz_Numero = Luz_N;
  Position[3] = 1.0f;
  glLightfv( Luz_Numero, GL_DIFFUSE,  Diffuse  );
  glLightfv( Luz_Numero, GL_AMBIENT,  Ambient  );
  glLightfv( Luz_Numero, GL_SPECULAR, Specular );
  glLightfv( Luz_Numero, GL_POSITION, Position );
  glLightfv( Luz_Numero, GL_SPOT_DIRECTION, Direction     );
  glLightf(  Luz_Numero, GL_SPOT_CUTOFF   , Spot_CutOff   );
  glLightf(  Luz_Numero, GL_SPOT_EXPONENT , Spot_Exponent );
}

void luces::posiciona( void ) {
  glLightfv( Luz_Numero, GL_POSITION, Position );
}

void luces::direcciona( void ) {
  glLightfv( Luz_Numero, GL_SPOT_DIRECTION, Direction  );
}

void luces::ver_direccional( void ) {
  if ( glIsEnabled( Luz_Numero ) ) {
    glPointSize( 8.0f ); 
    glBegin( GL_POINTS  );
      glVertex3f( Position[0], Position[1], Position[2] );
    glEnd();
    glBegin( GL_LINES  );
      glVertex3f( Position[0], Position[1], Position[2] );
      glVertex3f( 0.0f, 0.0f, 0.0f );
    glEnd();
  }
}

void luces::ver_puntual( void ) {
  if ( glIsEnabled( Luz_Numero ) ) {
    glPointSize( 8.0f ); 
    glBegin( GL_POINTS  );
      glVertex3f( Position[0], Position[1], Position[2] );
    glEnd();
  }
}

void luces::ver_focal( void ) {
  if ( glIsEnabled( Luz_Numero ) ) {
    glPointSize( 8.0f ); 
    glBegin( GL_POINTS  );
      glVertex3f( Position[0], Position[1], Position[2] );
    glEnd();
    glBegin( GL_LINES  );
      glVertex3f( Position[0],  Position[1],  Position[2] );
      glVertex3f( Position[0] + Direction[0], Position[1] + Direction[1], Position[2] + Direction[2] );
    glEnd();
  }
}
