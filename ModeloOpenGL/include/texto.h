#ifndef _TEXTO_
#define _TEXTO_

#include <iostream>
#include <GL/gl.h>										
#include <GL/glu.h>
using namespace std;	

class texto{
	public:
	
	texto();
	//**************************************************************************
	//Dibujar texto en pantalla
	//***************************************************************************

	void dibujar_texto(float x, float y, void *font,const char *string,const float * rgb,int w,int h);

	//**************************************************************************
	//Carga la proyeccion ortografica
	//***************************************************************************

	void carga_proyeccionOrto(int w,int h);

	//**************************************************************************
	// Funcion que devuelve la matriz de proyeccion y de modelado
	//***************************************************************************

	void quita_proyeccionOrto();

	//**************************************************************************
	// Funcion que dibuja texto en una posicion y con un color y fuente determinada
	//***************************************************************************

	void draw_text(float x, float y, void *font,const char *string,const float * rgb);
	private:

};
#endif
