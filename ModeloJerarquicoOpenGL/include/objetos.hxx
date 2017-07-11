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
void _triangulos3D::draw_solido_flat()
{
	normales.resize(caras.size());
	calculo_normales_plano();
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
	for(i=0;i<caras.size();++i){
		glNormal3fv((GLfloat *) &normales[i]);
		glColor3f(1.0f, 0.0f, 0.0f);  // activamos el color rojo
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glColor3f(0.0f,1.0f, 0.0f);  // activamos el color verde
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glColor3f(0.0f, 0.0f, 1.0f);  // activamos el color azul
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	glEnd();
}
void _triangulos3D::draw_solido_normales_ply(){
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glBegin(GL_TRIANGLES);
	for(i=0;i<caras.size();++i){
		glNormal3fv((GLfloat *) &normales[caras[i]._0]);
		glColor3f(1.0f, 0.0f, 0.0f);  // activamos el color rojo
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glNormal3fv((GLfloat *) &normales[caras[i]._1]);
		glColor3f(0.0f,1.0f, 0.0f);  // activamos el color verde
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glNormal3fv((GLfloat *) &normales[caras[i]._2]);
		glColor3f(0.0f, 0.0f, 1.0f);  // activamos el color azul
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	glEnd();

}
void _triangulos3D::calculo_normales_plano(){
  GLfloat modulo;
  GLfloat nx, ny, nz;//componentes de la normal al plano
  GLfloat a[3],b[3];//componentes de los vectores para calcular la normal
  for( int i = 0; i < caras.size(); i++ ) {
	// el producto vectorial es a= p2 -p0 y b= p1-p0 donde
	// el resultado de p2 -p0 es
	a[0] = vertices[caras[i]._2].x - vertices[caras[i]._0].x; 
    a[1] = vertices[caras[i]._2].y - vertices[caras[i]._0].y;
    a[2] = vertices[caras[i]._2].z - vertices[caras[i]._0].z;
	//  el resultado de p1 -p0 es
    b[0] = vertices[caras[i]._1].x - vertices[caras[i]._0].x;
    b[1] = vertices[caras[i]._1].y - vertices[caras[i]._0].y;
    b[2] = vertices[caras[i]._1].z - vertices[caras[i]._0].z;
    
    //el producto vectorial es
    nx = ( a[1] * b[2] ) - ( a[2] * b[1] );
    ny = ( a[2] * b[0] ) - ( a[0] * b[2] );
    nz = ( a[0] * b[1] ) - ( a[1] * b[0] );

    modulo = (GLfloat)sqrt( (nx*nx) + (ny*ny) + (nz*nz) );
    if( modulo == 0.0f ) 
		modulo = 1.0f;
    nx = nx/modulo; 
    ny = ny/modulo; 
    nz = nz/modulo;  
  
    normales[i].x = nx; 
    normales[i].y = ny; 
    normales[i].z = nz;
    
  } 

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
//**************************************************************************
// Superficies por revolucion
//***************************************************************************
void _triangulos3D::revolucionar_perfil(int secciones,bool tapa_sup,bool tapa_inf){
	
	
	vector<_vertex3f> finales;
	int tam =vertices.size();
	bool vertice_sup=false,vertice_inf=false;
	int tamfin=(tam*secciones);
	if(tapa_sup==true)
		if(vertices[0].x!=0){
			tamfin+=1;
			vertice_inf=true;
		}
	if(tapa_inf==true)
		if(vertices[vertices.size()-1].x!=0){
			tamfin+=1;// +2 para los vertices de la tapa superior e inferior
			vertice_sup=true;
		}

	
	finales.resize(tamfin);
	float alpha=0;
	
	
	
	for (int i = 0; i < tam; ++i){
			
		for (int j = 0; j<secciones; j++){
			alpha+=(2*M_PI)/secciones;
			finales[(i*secciones)+j].x = vertices[i].x*cos(alpha);
			finales[(i*secciones)+j].y = vertices[i].y;
			finales[(i*secciones)+j].z= vertices[i].x*sin(alpha);
			
		}
		
	}
	
	vertices.resize(tamfin);
	vertices =finales;
	if(tapa_sup==true)
		if( vertice_sup==true){
			//vertice central superior el ultimo de los vertices del array
			vertices[vertices.size() - 1].x = 0; //El ultimo elemento relleno es vertices -3 y el opuesto es 3+secciones-1.  El punto medio entre vertices opuestos
			vertices[vertices.size() - 1].y = vertices[vertices.size() - 3].y;
			vertices[vertices.size() - 1].z = 0;
		}
	if(tapa_inf==true)	
		if( vertice_inf==true){
			//vertice central inferior el penúltimo de los vertices del array
			vertices[vertices.size() - 2].x = 0; // El punto medio entre vertices opuestos
			vertices[vertices.size() - 2].y = vertices[0].y;
			vertices[vertices.size() - 2].z = 0;
		}
	
	
	caras.resize(2*secciones*(tam-1)); // Hay tantas caras como vertices iniciales=tam-1 expectuando a los vertices de la tapa superior e inferior y para quitar los que conectan a ellos secciones*2
	
	int i,t=0;
	for (i = 0,t=0; i < caras.size() && t+secciones<vertices.size(); i+=2,t++){
		if (t%secciones==(secciones-1) && t!=0) // Si está en el ultimo vertice de una sección tiene que conectarse con el primer vertice de esa seccion
		{

			//CARA A
			caras[i]._0 = t;
			caras[i]._1 = t - (secciones-1); // El vertice incial
			caras[i]._2 = t + secciones; //El vertice superior al que me encuentro

			//CARA B
			if(i+1<caras.size() && t+secciones<vertices.size()-1){	
				caras[i + 1]._0 = t - (secciones - 1); // El vertice incial
				caras[i + 1]._1 = t+1;  // El vertice derecho superior al que voy
				caras[i + 1]._2 = t + secciones; // el vertice superior al que voy

			}
			
		}//Todos los vertices menos el ultimo que conecta con el primero de la seccion
		else{
			//CARA A
			caras[i]._0 = t;
			caras[i]._1 = t + 1; //conecto con el vertice siguiente
			caras[i]._2 = t + secciones; // y con el vertice superior al vertice inicial

			//CARA B
			if(i+1<caras.size() && !(t%secciones==(secciones-1)) && t+secciones<vertices.size()){
				caras[i + 1]._0 = t + 1;// el vertice siguiente lo conecto con
				caras[i + 1]._1 = t + secciones + 1;// el vertice superior a el
				caras[i + 1]._2 = t + secciones;// y el vertice anterior al superior a el

			}
			
		}

	}
	caras.resize(caras.size()+secciones*2);
	
	int j=i,k;
	if(tapa_sup==true)
		if(vertice_sup==true){
			/////TAPA DE ARRIBA
			for ( j= i, k=0; k<secciones; ++j,++k){
				if (k%secciones==(secciones-1)) // Si está en el ultimo vertice de una sección tiene que conectarse con el primer vertice de esa seccion
				{
					caras[j]._0 = vertices.size() - 3; // El ultimo vertice
					caras[j]._1 = vertices.size() - (3 + k); // El vertice inicial
					caras[j]._2 = vertices.size() - 1; // El penultimo vertice que es el de la tapa de abajo
					
				}else{//Todos los vertices menos el ultimo que conecta con el primero de la seccion
					caras[j]._0 = (vertices.size() - (3 + k+1));// El anterior al 3+k
					caras[j]._1 = vertices.size() - (3 + k); //Empiezo en el ultimo vertice de la tapa excluyendo los dos finales por eso -3, y +k para retroceder
					caras[j]._2 = vertices.size() - 1; // la punta de la tapa
					cout<<" "<<caras[j]._0<<" "<<caras[j]._1<<" "<<caras[j]._2<<endl;
				}
			}
		}
	if(tapa_inf==true)
		if(vertice_inf==true){
			/////TAPA DE ABAJO
			for (int l = j,k=0; k < secciones; ++l,++k){
				if (k%secciones==(secciones-1))// Si está en el ultimo vertice de una sección tiene que conectarse con el primer vertice de esa seccion
				{
					caras[l]._0 = k; // El vertice actual que será secciones-1
					caras[l]._1 = k-(secciones-1); // El vertice inicial
					caras[l]._2 = vertices.size() - 2; // El penultimo vertice que es el de la tapa de abajo
				}
				else //Todos los vertices menos el ultimo que conecta con el primero de la seccion
				{
					caras[l]._0 = k; //Empiezo en el ultimo vertice de la tapa excluyendo los dos finales por eso -3, y +k para retroceder
					caras[l]._1 = k+1;//El vertice siguiente
					caras[l]._2 = vertices.size() - 2;// El penultimo vertice que es el de la tapa de abajo
					
				}
			}
		}
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

_esfera::_esfera(GLfloat radio,int secciones,int numverticesAltura){
		/*vertices.resize(numverticesAltura/2);
		for (int i=0;i<numverticesAltura/2;i++)
		{
			vertices[i].x=0;
			vertices[i].y=radio*sin(2.0*M_PI*i/n);
			vertices[i].z=radio*cos(2.0*M_PI*i/n);
		}*/
}


//**************************************************************************
// Funcion que dibuja semicirculos
//***************************************************************************

void semiCircle (GLfloat radio, GLint n, GLenum modo)
{
	int i;
	if (modo==GL_LINE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (modo==GL_FILL) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	glBegin(GL_POLYGON );
	for (i=0;i<n/2;i++)
		glVertex3f( 0, radio*sin(2.0*M_PI*i/n),radio*cos(2.0*M_PI*i/n));
	glEnd();
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
//**************************************************************************
// Carga archivos ply
//***************************************************************************

_objeto_archivo::_objeto_archivo(){
	
}
void _objeto_archivo::leer_archivo(const char * nombreArchivo){
	_file_ply::read(nombreArchivo,vertices,caras,normales);
}
