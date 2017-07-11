//**************************************************************************
// Práctica 2 usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hxx"

using namespace std;

const float AXIS_SIZE=5000;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void 	draw_solido_flat();
void	draw_solido_smooth();
void	draw_solido_normales_ply();
void 	revolucionar_perfil(int secciones,bool tapa_sup, bool tapa_inf);
void	calculo_normales_plano();
void	calculo_normales_vertices();
float 	distancia_total_vertices();
void 	draw_texturizado();
void	draw_texturizado_suave();
void	draw_texturizado_flat();



vector<_vertex3i> caras;
vector<_vertex3f> normales;
vector<_vertex3f> normales_vertices;
vector<_vertex2f> texturas;
};


//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
	void	draw_cubo_texturizado();

};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase esfera
//*************************************************************************

class _esfera: public _triangulos3D
{
public:

	_esfera(GLfloat radio,int secciones,int numverticesAltura);
};


//*************************************************************************
// función dibuja circulos
//*************************************************************************

void Circle (GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo);

//*************************************************************************
// clase cubo
//*************************************************************************

class _suelo: public _triangulos3D
{
public:

	_suelo(float tam=10);
	void draw_suelo_texturizado();
};


//*************************************************************************
// clase carga archivo
//*************************************************************************

class _objeto_archivo: public _triangulos3D
{
public:

	_objeto_archivo();
	void leer_archivo(const char * nombreArchivo);
};
