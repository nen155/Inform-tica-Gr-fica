#include "objetos.h"
_puntos3D::_puntos3D(){
	
}
void _puntos3D::draw_puntos(float r, float g, float b, int grosor){
	int i;
	glPointSize(grosor);
	glColor3f(r,g,b); 
	glBegin(GL_POINTS);
	for (i=0;i<vertices.size();i++) 
		glVertex3fv((GLfloat *) &vertices[i]);
	glEnd();
}

_triangulos3D::_triangulos3D(){

}
void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
	int j;
	glColor3f(r,g,b); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(grosor);
	glBegin (GL_POLYGON);
	
	for (j=0;j<caras.size();++j) {
		glVertex3fv((GLfloat *) &vertices[caras[j]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[j]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[j]._2]);
	}
	glEnd();
}
void _triangulos3D::draw_solido(float r, float g, float b)
{
	int i;
	glColor3f(r,g,b); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for(i=0;i<caras.size();++i){
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	glEnd();
}
void _triangulos3D::draw_solido_smooth()
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLES);
	for(i=0;i<caras.size();++i){
		glNormal3fv((GLfloat *) &vertices[caras[i]._0]);
		glColor3f(1.0f, 0.0f, 0.0f);  // activamos el color rojo
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glNormal3fv((GLfloat *) &vertices[caras[i]._1]);
		glColor3f(0.0f,1.0f, 0.0f);  // activamos el color verde
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glNormal3fv((GLfloat *) &vertices[caras[i]._2]);
		glColor3f(0.0f, 0.0f, 1.0f);  // activamos el color azul
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	glEnd();
}
void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2){
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for(i=0;i<caras.size();++i){
		if(i%2==0)
			glColor3f(r1,g1,b1);
		else
			glColor3f(r2,g2,b2);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	glEnd();
}

_suelo::_suelo(float tam){
	vertices.resize(4);

	vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
	vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
	vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
	vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
	caras.resize(2);
	caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
	caras[1]._0=1;caras[1]._1=2;caras[1]._2=3;

}

_cubo::_cubo(float tam){

	vertices.resize(8);
	//PRIMERA PARTE DEL CUBO
	vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
	vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
	vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
	vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
	//SEGUNDA PARTE DEL CUBO
	vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
	vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
	vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
	vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;
	caras.resize(12);
	//cara FRONTAL
	caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
	caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
	//cara LATERAL IZQ
	caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
	caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
	//cara LATERAL DER
	caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
	caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
	//cara POSTERIOR
	caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
	caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
	//cara INFERIOR
	caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
	caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
	//cara SUPERIOR
	caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
	caras[11]._0=1;caras[11]._1=5;caras[11]._2=4;
}
_piramide::_piramide(float tam, float al){
	
	vertices.resize(5);
	//BASE PIRAMIDE
	vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
	vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
	vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
	vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
	//PUNTA DE LA PIRAMIDE
	vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;
	
	caras.resize(6);
	//cara frontal
	caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
	//cara lateral der
	caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
	//cara atras
	caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
	//cara lateral izq
	caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
		//caras de la base
	caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
	caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;

}
//**************************************************************************
// Funcion que dibuja circulos
//***************************************************************************

void Circle (GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo)
{
	int i;
	if (modo==GL_LINE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (modo==GL_FILL) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glBegin(GL_POLYGON );
	for (i=0;i<n;i++)
		glVertex3f( cx+radio*cos(2.0*M_PI*i/n), cy+radio*sin(2.0*M_PI*i/n),1.0);
	glEnd();
}
