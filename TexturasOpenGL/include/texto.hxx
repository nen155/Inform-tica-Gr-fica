#include "texto.h"


//**************************************************************************
//Constructor vacio
//***************************************************************************
texto::texto(){

}

//**************************************************************************
//Dibujar texto en pantalla
//***************************************************************************

void texto::dibujar_texto(float x, float y, void *font,const char *string,const float * rgb,int w,int h){
	carga_proyeccionOrto(w,h);
	// dibujo el texto en la pantalla
	draw_text(x,y, font, string, rgb);
	quita_proyeccionOrto();
}

//**************************************************************************
//Carga la proyeccion ortografica
//***************************************************************************

void texto::carga_proyeccionOrto(int w,int h){
	glMatrixMode(GL_PROJECTION); //Aplica operaciones con matrices posteriores a la pila matriz PROYECCIÓN.
    glPushMatrix(); // Guardamos la matriz de proyeccion
    glLoadIdentity(); // Cargamos la matriz identidad
    gluOrtho2D(0.0, w, 0.0, h); //definir una matriz de proyección ortografica 2D  
    glMatrixMode(GL_MODELVIEW); //Aplica operaciones con matrices posteriores a la pila matriz MODELVIEW.
    glPushMatrix(); //pone en la pila la matriz actual 
    glLoadIdentity(); //resetea la matriz a su estado por defecto
}

//**************************************************************************
// Funcion que devuelve la matriz de proyeccion y de modelado
//***************************************************************************

void texto::quita_proyeccionOrto(){
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
}

//**************************************************************************
// Funcion que dibuja texto en una posicion y con un color y fuente determinada
//***************************************************************************

void texto::draw_text(float x, float y, void *font,const char *string,const float * rgb){
    const char *c;
    glColor3f(rgb[0], rgb[1], rgb[2]); 
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 
