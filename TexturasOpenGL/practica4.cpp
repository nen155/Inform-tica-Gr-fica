#include "stdlib.h"
#include "stdio.h"
#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include "include/objetos.hxx"
#include "include/texto.hxx"
#include "include/materiales.hxx"
#include "include/luces.hxx"


// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=500,UI_window_height=500;

//**************************************************************************
//
//***************************************************************************
//variable que determina si se muestra puntos, lineas,solido,ajedrez,smooth
int drawforma=1;
float posicion_luz_focal[]= {-5.0f,  10.0f, 10.0f};
float posicion_luz_puntual[]={0.0f,20.0f,0.0f};
int direction[3];

///Rotaciones y posiciones de la nave
float posicion_avion[]= {-20.0f,  0.0f, 0.0f};
float rotacion_avion[]={0.0f,0.0f,0.0f};
bool abrir_cabina_nave =false;
bool abrir_alas_nave=false;
bool rotar_disparadores=false;
const int maximo_rotacion_ala_s=15;
const int maximo_rotacion_ala_i=-15;
int rotacion_ala_s=0;
int rotacion_ala_i=0;

///END Rotaciones y posiciones de la nave
//MODO NORMALES POR DEFECTO SMOOTH PARA LA NAVE
bool flat=true;
//Intervalo de tiempo para ejecutar la funcion
double interv = 100;
bool flag = true; //bandera para terminar de redibujar la rotacion de las alas

_cubo cubo(3); //CUBO A MOSTRAR
_cubo cielo(400);
_piramide piramide(3,3); //PIRAMIDE A MOSTRAR
_suelo suelo(50);

float rgb[3]; //array de colores para el texto
//Archivo cargado
_objeto_archivo obj_cargado;
_objeto_archivo lata;
_objeto_archivo lata_s;
_objeto_archivo lata_i;

///PEON
_objeto_archivo peon;

///CARGAR XWING
_objeto_archivo nave;
_objeto_archivo alais;
_objeto_archivo	alads;
_objeto_archivo	alaii;
_objeto_archivo aladi;
_objeto_archivo cilindrodi;
_objeto_archivo cilindrods;
_objeto_archivo cilindroii;
_objeto_archivo cilindrois;
_objeto_archivo cabina;

///CARGAR XWING


// -- CREAMOS LAS LUCES ------------------------------------------------------
luces *luz = new luces[2];
// -- CREAMOS EL TEXTO -------------------------------------------------------
texto textos;
// -- CREAMOS EL MATERIAL ----------------------------------------------------
materiales *mat = new materiales[2];
// -- ACTIVA LUCES -----------------------------------------------------------
bool act1=false,act2=false;
	// Data read from the header of the BMP file

 unsigned int width_suelo, height_suelo,width_cuadrado, height_cuadrado,width_lata, height_lata,width_estrella, height_estrella,width_peon, height_peon;

 // Actual RGB data
 unsigned char * estrellas;
 unsigned char * madera;
 unsigned char * madera_peon;
 unsigned char * textura_lata;
  unsigned char * estrella_muerte;
 


unsigned char * loadBMP_custom(const char * imagepath, unsigned int & width,  unsigned int & height){
	 unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	 unsigned int dataPos;     // Position in the file where the actual data begins
	 unsigned int imageSize;   // = width*height*3
	 unsigned char * data;
	 // Open the file
	 FILE * file = fopen(imagepath,"rb");
	 if (!file){printf("Image could not be opened\n"); exit(0);}
	  if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
		 printf("Not a correct BMP file\n");
		 exit(0);
	 }
	  if ( header[0]!='B' || header[1]!='M' ){
		 printf("Not a correct BMP file\n");
		 exit(0);
	 }
	 if(*(int*)&(header[0x1E])!=0){printf("Not a correct BMP file\n"); return 0;}
	 if(*(int*)&(header[0x1C])!=24){printf("Not a correct BMP file\n"); return 0;}
	 dataPos    = *(int*)&(header[0x0A]);
	 imageSize  = *(int*)&(header[0x22]);
	 width      = *(int*)&(header[0x12]);
	 height     = *(int*)&(header[0x16]);
	 // Some BMP files are misformatted, guess missing information
	 if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	 if (dataPos==0)      dataPos=54; // The BMP header is done that way
	// Create a buffer
	 data = new unsigned char [imageSize];
	 
	 // Read the actual data from the file into the buffer
	 fread(data,1,imageSize,file);
	 
	 //Everything is in memory now, the file can be closed
	 fclose(file);
	 return data;
}

void clear_window()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glEnable(GL_DEPTH_TEST);
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
	//  Front_plane>0  Back_plane>PlanoDelantero)
	glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-Observer_distance);
	glRotatef(Observer_angle_x,1,0,0);
	glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	glBegin(GL_LINES);
	// eje X, color rojo
	glColor3f(1,0,0);
	glVertex3f(-AXIS_SIZE,0,0);
	glVertex3f(AXIS_SIZE,0,0);
	// eje Y, color verde
	glColor3f(0,1,0);
	glVertex3f(0,-AXIS_SIZE,0);
	glVertex3f(0,AXIS_SIZE,0);
	// eje Z, color azul
	glColor3f(0,0,1);
	glVertex3f(0,0,-AXIS_SIZE);
	glVertex3f(0,0,AXIS_SIZE);
	glEnd();
}

//**************************************************************************
//Funcion que carga la imagen de la textura
//***************************************************************************

void init_textura (unsigned char * data, unsigned int width,  unsigned int height)
{
	glEnable(GL_DEPTH_TEST);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
}
void end_textura(void){
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

//**************************************************************************
//Funcion que carga los materiales
//***************************************************************************

void carga_materiales(int i,float r, float g, float b){
	mat[i].ambient(r,g,b);// Definición del color del material
	//mat[i].emissive(0.3f,0.3f,0.3f);// Emisión de luz del propio objeto
	mat[i].specular(1.0f,1.0f,1.0f);// Definición del brillo del objeto
	mat[i].diffuse(  1.0f, 1.0f, 1.0f );//color cuando esta iluminados
	mat[i].phongsize(120.0f);
	mat[i].shininess(120.0f);
	mat[i].activa();
}
//**************************************************************************
//Funcion que carga la luz
//***************************************************************************

void carga_luz(float xf, float yf,float zf,float xp, float yp,float zp,int* direction){
	GLfloat luz1 []= {6.0,50.0,5.0,1};
	GLfloat luz2 []= {-10.0,3.0,5.0,1};
	GLfloat direccion2[3];
	direccion2[0]=xf;
	direccion2[1]=yf;
	direccion2[2]=zf;
	GLfloat direccion1[3];
	direccion1[0]=-5.0f;
	direccion1[1]=5.0f;
	direccion1[2]=10.0f;
	
	glLightfv( GL_LIGHT1, GL_POSITION, direccion1	  );
	glLightfv( GL_LIGHT0, GL_POSITION, direccion2 );
	
	GLfloat ambiente2 []= {1.0,0.5,0.25,1};
	GLfloat ambiente1 []= {1.0,1.0,1.0,1.0};
	
	GLfloat difusa2 []= {1.0,0.5,0.25,1};
	GLfloat difusa1 []= {0.3,0.3,0.3,1.0};
	
	GLfloat especular2 []= {1.0,0.5,0.25,1};
	GLfloat especular1 []= {1.0,1.0,1.0,1.0};
	
  glLightfv( GL_LIGHT1, GL_DIFFUSE,  difusa1  );
  glLightfv( GL_LIGHT1, GL_AMBIENT,  ambiente1  );
  glLightfv( GL_LIGHT1, GL_SPECULAR, especular1 );

	luz[0].position(  xf,  yf, zf );
	luz[0].diffuse(   0.2f,  0.2f, 0.2f );
	luz[0].ambient(   1.0f,  1.0f, 1.0f );
	luz[0].specular(1.0,1.0,1.0);
	luz[0].direction(direction[0],direction[1],direction[2]);
	luz[0].focal( GL_LIGHT0 );
	
}


//**************************************************************************
//Funcion que ejecuta una en un intervalo de tiempo
//***************************************************************************

void recorrido_alas(){
	clock_t startTime = clock(); //Start timer
	double secondsPassed;
	double secondsToDelay = interv;
	 while(flag)
	  {
	   secondsPassed = (clock() - startTime) / (CLOCKS_PER_SEC/1000);
	   if(secondsPassed == secondsToDelay)
		{
			if(abrir_alas_nave){
						if(rotacion_ala_i> maximo_rotacion_ala_i)
							rotacion_ala_i--;
						if(rotacion_ala_s<maximo_rotacion_ala_s)
							rotacion_ala_s++;
							
			}else
			{
						if(rotacion_ala_i< 0)
							rotacion_ala_i++;
						if(rotacion_ala_s>0)
							rotacion_ala_s--;
			}	
			flag = false;
		}
	  }
	  flag=true;
}
//**************************************************************************
//Funcion que ejecuta continuamente con el redisplay
//***************************************************************************
void idle(){
			if(abrir_alas_nave){
						if(rotacion_ala_i> maximo_rotacion_ala_i)
							rotacion_ala_i--;
						if(rotacion_ala_s<maximo_rotacion_ala_s)
							rotacion_ala_s++;
							
			}else
			{
						if(rotacion_ala_i< 0)
							rotacion_ala_i++;
						if(rotacion_ala_s>0)
							rotacion_ala_s--;
			}	
}
//**************************************************************************
//Funcion que dibuja el texto
//***************************************************************************

void dibujar_texto(){
		//pongo el color del texto
	rgb[0]=1.0f;
	rgb[1]=0.0f;
	rgb[2]=0.0f;
	textos.dibujar_texto(5,UI_window_width-24,GLUT_BITMAP_TIMES_ROMAN_24,(char *)"F1 para modo puntos",rgb,UI_window_width,UI_window_height);
	textos.dibujar_texto(5,UI_window_width-52,GLUT_BITMAP_TIMES_ROMAN_24,(char *)"F2 para modo lineas",rgb,UI_window_width,UI_window_height);
	textos.dibujar_texto(5,UI_window_width-80,GLUT_BITMAP_TIMES_ROMAN_24,(char *)"F3 para modo solido",rgb,UI_window_width,UI_window_height);
	textos.dibujar_texto(5,UI_window_width-108,GLUT_BITMAP_TIMES_ROMAN_24,(char *)"F4 para modo ajedrez",rgb,UI_window_width,UI_window_height);
	textos.dibujar_texto(5,UI_window_width-136,GLUT_BITMAP_TIMES_ROMAN_24,(char *)"F5 para nave",rgb,UI_window_width,UI_window_height);
	
}

//**************************************************************************
//Funcion que dibuja la nave jerarquica
//***************************************************************************

void dibujar_nave_completa(){	
		glPushMatrix();
			glRotatef(rotacion_avion[0],1,0,0);
			glRotatef(rotacion_avion[1],0,1,0);
			glRotatef(rotacion_avion[2],0,0,1);
			
			carga_materiales(1,0.9f,0.9f,0.9f); //color GRIS
			if(flat==true)
				nave.draw_solido_flat();
			else
				nave.draw_solido_smooth();
			//nave.draw_solido_normales_ply();
			glPushMatrix();
				glRotatef(rotacion_ala_s,0,0,1);
				carga_materiales(1,0.9f,0.9f,0.9f); //color GRIS MAS CLARO
				if(flat==true){
					alais.draw_solido_flat();
					aladi.draw_solido_flat();
				}
				else{
					alais.draw_solido_smooth();
					aladi.draw_solido_smooth();
				}
				//alais.draw_solido_normales_ply();
				//aladi.draw_solido_normales_ply();
				glPushMatrix();
					if(rotar_disparadores==true){
						glTranslatef(0,7.5f,60);
						glRotatef(10,1,0,0);
						glTranslatef(0,-7.5f,-60);
					}
					carga_materiales(1,0.6f,0.6f,0.6f); //color GRIS
					if(flat==true)
						cilindrodi.draw_solido_flat();
					else
						cilindrodi.draw_solido_smooth();
					//cilindrodi.draw_solido_normales_ply();
				glPopMatrix();
				
				glPushMatrix();
					if(rotar_disparadores==true){
						glTranslatef(0,7.5f,55);
						glRotatef(10,1,0,0);
						glTranslatef(0,-7.5f,-55);
					}
					carga_materiales(1,0.6f,0.6f,0.6f); //color GRIS
					if(flat==true)
						cilindrois.draw_solido_flat();
					else
						cilindrois.draw_solido_smooth();
					//cilindrois.draw_solido_normales_ply();
				glPopMatrix();
				
			glPopMatrix();	
				
			glPushMatrix();
				glRotatef(rotacion_ala_i,0,0,1);
				carga_materiales(1,0.9f,0.9f,0.9f); //color GRIS MAS CLARO
				if(flat==true){
					alaii.draw_solido_flat();
					alads.draw_solido_flat();
				}else{
					alaii.draw_solido_smooth();
					alads.draw_solido_smooth();
				}
				//alaii.draw_solido_normales_ply();
				//alads.draw_solido_normales_ply();
				glPushMatrix();
				
					glPushMatrix();
					if(rotar_disparadores==true){
						glTranslatef(0,7.5f,65);
						glRotatef(10,1,0,0);
						glTranslatef(0,-7.5f,-65);
					}
					carga_materiales(1,0.6f,0.6f,0.6f); //color GRIS
					if(flat==true)
						cilindroii.draw_solido_flat();
					else
						cilindroii.draw_solido_smooth();	
					//cilindroii.draw_solido_normales_ply();
					glPopMatrix();
					
					glPushMatrix();
						if(rotar_disparadores==true){
							glTranslatef(0,7.5f,55);
							glRotatef(10,1,0,0);
							glTranslatef(0,-7.5f,-55);
						}
						if(flat==true)
							cilindrods.draw_solido_flat();
						else
							cilindrods.draw_solido_smooth();
						//cilindrods.draw_solido_normales_ply();
					glPopMatrix();
					
				glPopMatrix();
			glPopMatrix();	
			glPushMatrix();
				
				if(abrir_cabina_nave==true){
					glTranslatef(-20,7.5f,47);
					glRotatef(65,1,0,0);
					glTranslatef(20,-7.5f,-47);
				}
				carga_materiales(1,0.0f,0.0f,1.0f); //color azul
				if(flat==true)
					cabina.draw_solido_flat();
				else
					cabina.draw_solido_smooth();
				//cabina.draw_solido_normales_ply();
			glPopMatrix();	
		glPopMatrix();
}

// -----------------------------------------------------------------------------
// -- Hace una máscara para limitar el dibujo de las Sombras -------------------
// -----------------------------------------------------------------------------
const void Hacer_Mascara_Sombra( void ) {
  glClear( GL_STENCIL_BUFFER_BIT );
  glDisable( GL_DEPTH_TEST );
  glEnable( GL_STENCIL_TEST );
  glStencilFunc( GL_ALWAYS, 1, 1 );
  glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
}

const void Terminar_Mascara_Sombra( void ) {
  glStencilFunc( GL_EQUAL, 1, 1 );
  glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
}

//**************************************************************************
// Funcion que dibuja los reflejos

//***************************************************************************
void reflejos(){
	 if ( glIsEnabled( GL_LIGHT0 ) || glIsEnabled( GL_LIGHT1 )) {
		 direction[0]=1;
		 direction[1]=1; //hacia arriba
		 direction[2]=1;
	glPushMatrix();
	carga_luz( posicion_luz_focal[0],  -(posicion_luz_focal[1]+10), posicion_luz_focal[2],posicion_luz_puntual[0],posicion_luz_puntual[1],posicion_luz_puntual[2],direction);
	glPopMatrix();
	carga_luz( posicion_luz_focal[0],   -(posicion_luz_focal[1]+10), posicion_luz_focal[2],posicion_luz_puntual[0],posicion_luz_puntual[1],posicion_luz_puntual[2],direction);
	
	glPushMatrix();
	glTranslatef(posicion_avion[0],posicion_avion[1]-8,posicion_avion[2]);
        glRotatef( 180, 1.0f, 0.0f, 0.0f );
        glRotatef( 180, 0.0f, 1.0f, 0.0f );
          mat[1].alpha( 0.4f );
          mat[1].activa();
			dibujar_nave_completa();
		mat[1].desactiva();   // -- OJO, IMPORTANTE DESACTIVAR MATERIAL --
        mat[1].alpha( 1.0f );
	glPopMatrix();
	
  }
	
}
void suelo_reflejos(){
	    glEnable( GL_STENCIL_TEST );
		glStencilFunc( GL_ALWAYS, 1, 0xFFFFFFFF );
		glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			carga_materiales(0,1.0f,1.0f,1.0f); //suelo color blanco
			glTranslatef(0.0,-7,0.0);
			init_textura(madera,width_suelo,height_suelo);
			suelo.draw_suelo_texturizado();
			end_textura();
			glTranslatef(0.0,7,0.0);
			
			glDisable(GL_CULL_FACE);
		glStencilFunc( GL_EQUAL, 1, 0xFFFFFFFF  );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		glDisable(GL_DEPTH_TEST);
}



//**************************************************************************
// Funcion que dibuja los objetos
//***************************************************************************

void draw_objects()
{
	
	switch(drawforma){
		case 1:
			cubo.draw_puntos(1,0,0,3);
			
			glPushMatrix();
			obj_cargado.draw_puntos(1,0,0,3);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(-3,0,0.0);
			glRotatef(90,0,0,1);
			piramide.draw_puntos(1,0,0,3);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(3,0,0.0);
			glRotatef(-90,0,0,1);
			piramide.draw_puntos(1,0,0,3);
			glPopMatrix();
		break;
		case 2:
			  
			cubo.draw_aristas(1,0,0,2);
			
			glPushMatrix();
			glTranslatef(0,5,0.0);
			obj_cargado.draw_aristas(1,0,0,3);
			glPopMatrix();
			   
			glPushMatrix();
			glTranslatef(-3,0,0.0);
			glRotatef(90,0,0,1);
			piramide.draw_aristas(1,0,0,2);
			glPopMatrix();
		
			glPushMatrix();
			glTranslatef(3,0,0.0);
			glRotatef(-90,0,0,1);
			piramide.draw_aristas(1,0,0,2);
			glPopMatrix();
		break;
		case 3:
			cubo.draw_solido(1,1,0);
			
			glPushMatrix();
			glTranslatef(0,5,0.0);
			obj_cargado.draw_solido(0,0,1);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(3,0,0.0);
			glRotatef(-90,0,0,1);
			piramide.draw_solido(1,0,0);
			glPopMatrix();
		
			glPushMatrix();
			glTranslatef(-3,0,0.0);
			glRotatef(90,0,0,1);
			piramide.draw_solido(1,0,0);
			glPopMatrix();
		break;
		case 4:
			cubo.draw_solido_ajedrez(1,1,0,0,1,1);
			
			glPushMatrix();
			glTranslatef(0,5,0.0);
			obj_cargado.draw_solido_ajedrez(1,1,0,0,1,1);
			glPopMatrix();
		
			glPushMatrix();
			glTranslatef(3,0,0.0);
			glRotatef(-90,0,0,1);
			piramide.draw_solido_ajedrez(1,1,0,0,1,1);
			glPopMatrix();
		
			glPushMatrix();
			glTranslatef(-3,0,0.0);
			glRotatef(90,0,0,1);
			piramide.draw_solido_ajedrez(1,1,0,0,1,1);
			glPopMatrix();
		break;
		case 5:
			
			direction[0]=1;
			direction[1]=-1;
			direction[2]=1;
			carga_luz( posicion_luz_focal[0],  posicion_luz_focal[1], posicion_luz_focal[2],posicion_luz_puntual[0],posicion_luz_puntual[1],posicion_luz_puntual[2],direction);
			
			glPushMatrix();
			carga_materiales(0,0.0f,0.0f,0.0f); //suelo color blanco
			obj_cargado.draw_solido_flat();
			
			glPopMatrix();
			


			
			glPushMatrix();
				glTranslatef(posicion_avion[0],posicion_avion[1],posicion_avion[2]);
				dibujar_nave_completa();
			glPopMatrix();
			
			/*glPushMatrix();
				lata_s.draw_texturizado_suave();
			glPopMatrix();
			
			
			glPushMatrix();
				lata_i.draw_texturizado_suave();
			glPopMatrix();*/
			
			glPushMatrix();
				carga_materiales(0,1.0f,1.0f,1.0f); //suelo color blanco
				static GLfloat plano_s2[4]={1.0, 0.0, 0.0, 0.0}, plano_t2[4]={0.0, 1.0, 0.0, 0.0};
				init_textura(madera_peon,width_peon,height_peon);
				glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexGenfv(GL_S,GL_OBJECT_PLANE,plano_s2);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGenfv(GL_T,GL_OBJECT_PLANE,plano_t2);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glTranslatef(10.0,45.0,50.0);
				glScalef(15,15,15);
				peon.draw_solido_smooth();
				end_textura();
			
			glPopMatrix();
			
			
			glPushMatrix();
				carga_materiales(0,1.0f,1.0f,1.0f); //lata color blanco
				glColor3f (1.0, 1.0, 1.0);
				glScalef(50,50,50);
				init_textura(textura_lata,width_lata,height_lata);
				if(flat==true)
					lata.draw_texturizado_flat();
				else
					lata.draw_texturizado_suave();
				end_textura();
			
			glPopMatrix();

			
			///TEXTURAS DE SUELO Y CIELO
			glPushMatrix();
				carga_materiales(0,1.0f,1.0f,1.0f); //suelo color blanco
				glTranslatef(0.0,-7,0.0);
				init_textura(madera,width_suelo,height_suelo);
				suelo.draw_suelo_texturizado();	
				end_textura();
			glPopMatrix();
			
			glPushMatrix();
				carga_materiales(0,1.0f,1.0f,1.0f);
				init_textura(estrellas,width_cuadrado,height_cuadrado);
				cielo.draw_cubo_texturizado();
				end_textura();
			glPopMatrix();
			
			///DIBUJAR ESTRELLA DE LA MUERTE
			glPushMatrix();
				static GLfloat plano_s[4]={1.0, 0.0, 0.0, 0.0}, plano_t[4]={0.0, 1.0, 0.0, 0.0};
				init_textura(estrella_muerte,width_estrella,height_estrella);
				//glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			    //glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
				glTexGenfv(GL_S,GL_OBJECT_PLANE,plano_s);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGenfv(GL_T,GL_OBJECT_PLANE,plano_t);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glTranslatef(100,50,-200);
				glutSolidSphere(100,100,100);
				end_textura();
			glPopMatrix();
			

		
		break;
	}
}



void draw_scene(void)
{
	clear_window();
	change_observer();
	draw_axis();
	draw_objects();
	dibujar_texto();
	glutSwapBuffers();
	glutPostRedisplay();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
	change_projection();
	glViewport(0,0,Ancho1,Alto1);
	glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{

if (toupper(Tecla1)=='Q') exit(0);
if(toupper(Tecla1)=='F'){
	flat=!flat;
}
if(toupper(Tecla1)=='1')
	if(act1==false){
		
		luz[0].activa(GL_LIGHT0);
		luz[0].ver_focal();	
		act1=true;
	}else
	{
		luz[0].desactiva(GL_LIGHT0);
		act1=false;
	}
if(toupper(Tecla1)=='2')
	if(act2==false){
		
		luz[1].activa(GL_LIGHT1);
		luz[1].ver_puntual();	
		act2=true;
	}else
	{
		luz[1].desactiva(GL_LIGHT1);
		act2=false;
	}
	
		if(toupper(Tecla1)=='A'){
			posicion_luz_focal[0]-=0.5;
			luz[0].ver_focal();	
		}
		if(toupper(Tecla1)=='D'){
			posicion_luz_focal[0]+=0.5;
			luz[0].ver_focal();	
		}
		if(toupper(Tecla1)=='S'){
			posicion_luz_focal[1]-=0.5;
			luz[0].ver_focal();	
		}
		if(toupper(Tecla1)=='W'){
			posicion_luz_focal[1]+=0.5;
			luz[0].ver_focal();	
		}
		
		if(toupper(Tecla1)=='Z'){
			posicion_luz_focal[2]+=0.5;
			luz[0].ver_focal();	
		}
		if(toupper(Tecla1)=='X'){
			posicion_luz_focal[2]-=0.5;
			luz[0].ver_focal();	
		}
		///Abrir las alas
		if(toupper(Tecla1)=='7'){
			abrir_alas_nave=!abrir_alas_nave;
		}
		if(toupper(Tecla1)=='9'){
			
			abrir_cabina_nave=!abrir_cabina_nave;
		}
		///END Abrir las alas
		
		///Posicion del avion
		if(toupper(Tecla1)=='I'){
			//subir y bajar
			if(rotacion_avion[0]<-2){
				if(rotacion_avion[0]>=-90){//bajar
					posicion_avion[1]-=	0.5;
				}else if(rotacion_avion[0]>-180)
				{
						posicion_avion[1]-=	1.0;
						posicion_avion[2]+=2; //retrocedo
				}else if(rotacion_avion[0]>=-360){
					posicion_avion[2]+=2; //retrocedo
					posicion_avion[1]+=1.0;//subir
				}
			}
			if(rotacion_avion[0]>2){
				if(rotacion_avion[0]<=90){//subir
					posicion_avion[1]+=	1.0;
				}else if(rotacion_avion[0]<180)
				{
						posicion_avion[1]+=	1.0;
						posicion_avion[2]+=2; //retrocedo
				}else if(rotacion_avion[0]<360){
					posicion_avion[2]+=2; //retrocedo
					posicion_avion[1]-=1.0;//bajar
				}
			}
				
			//girar izquierda y derecha
			if(rotacion_avion[2]<0){//derecha
				posicion_avion[0]+=1.0;
			}else if(rotacion_avion[2]>0){//izquierda
				posicion_avion[0]-=1.0;
			}
			//avanzar
			posicion_avion[2]-=1.0;
		}
		if(toupper(Tecla1)=='J')//atras
			posicion_avion[0]-=1.0;
		
		if(toupper(Tecla1)=='L')//izquierda
			posicion_avion[0]+=1.0;
			
		if(toupper(Tecla1)=='K')//derecha
			posicion_avion[2]+=1.0;
		///END Posicion del avion
		
		///rotacion del avion
		if(toupper(Tecla1)=='4'){//izquierda
			rotacion_avion[2] = ((int)(rotacion_avion[2]-1))%360;
		}
		if(toupper(Tecla1)=='8'){//abajo
			rotacion_avion[0]= ((int)(rotacion_avion[0]-1))%360;
		}
		if(toupper(Tecla1)=='5'){//arriba
			rotacion_avion[0] = ((int)(rotacion_avion[0]+1))%360;
		}
		if(toupper(Tecla1)=='6'){//derecha
			rotacion_avion[2] = ((int)(rotacion_avion[2]+1))%360;
		}	
		///END rotacion del avion
		///Rotacion cilindros
		if(toupper(Tecla1)=='0')
			rotar_disparadores=!rotar_disparadores;


	glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1,int x,int y)
{

switch (Tecla1){
	case GLUT_KEY_LEFT:Observer_angle_y--;break;
	case GLUT_KEY_RIGHT:Observer_angle_y++;break;
	case GLUT_KEY_UP:Observer_angle_x--;break;
	case GLUT_KEY_DOWN:Observer_angle_x++;break;
	case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
	case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
	case GLUT_KEY_F1:
		drawforma=1;
	break;
	case GLUT_KEY_F2:
		drawforma=2;
	break;
	case GLUT_KEY_F3:
		drawforma=3;
	break;
	case GLUT_KEY_F4:
		drawforma=4;
	break;
	case GLUT_KEY_F5:
		drawforma=5;
	break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(int argc, char**argv)
{
	bool tapa_sup=true,tapa_inf=true;
	int secciones=6;

			
	if(argc>1){///si se pasa un parametro para poner el nombre del .ply
		obj_cargado.leer_archivo(argv[1]); ///nombre del .py
		if(argc>2)
			if(strcmp(argv[2],"revolucionar")==0){ ///si quiere revolucionar el .ply en caso de que no se pueda fallará
					if(argc>2)///el numero de secciones que se quiere poner al .ply
						secciones=atoi(argv[3]);
					if(argc>3)///si se quiere dibujar la tapa de arriba o no
						if(argv[4][0]=='0')
							tapa_sup=false;
					if(argc>4)///si se quiere dibujar la tapa de abajo o no
						if(argv[5][0]=='0')
							tapa_inf=false;
				///revoluciono el perfil pasado
				obj_cargado.revolucionar_perfil(secciones,tapa_sup,tapa_inf);
			}
	}
	else{///sino se pasa parametro dibujo el perfil revolucionado con las tapas
		estrellas=loadBMP_custom("img/BrightStars2.bmp",width_cuadrado,height_cuadrado);
		madera=loadBMP_custom("img/panel.bmp",width_suelo,height_suelo);
		textura_lata=loadBMP_custom("img/text-lata.bmp",width_lata,height_lata);
		estrella_muerte=loadBMP_custom("img/estrella.bmp",width_estrella, height_estrella);
		madera_peon=loadBMP_custom("img/wood.bmp",width_peon,height_peon);
		lata.leer_archivo("lata-pcue");
		lata_i.leer_archivo("lata-psup");
		lata_s.leer_archivo("lata-pinf");
		nave.leer_archivo("xwing/nave");
		alaii.leer_archivo("xwing/alaii");
		aladi.leer_archivo("xwing/aladi");
		alais.leer_archivo("xwing/alais");
		alads.leer_archivo("xwing/alads");
		cilindrods.leer_archivo("xwing/cilindrods");
		cilindrois.leer_archivo("xwing/cilindrois");
		cilindroii.leer_archivo("xwing/cilindroii");
		cilindrodi.leer_archivo("xwing/cilindrodi");
		cabina.leer_archivo("xwing/cabina");
		
		lata.revolucionar_perfil(100,true,true);
		peon.leer_archivo("perfil");
		peon.revolucionar_perfil(100,true,true);
		
	}
	obj_cargado.calculo_normales_vertices();
	lata.calculo_normales_vertices();
	nave.calculo_normales_vertices();
	alais.calculo_normales_vertices();
	alads.calculo_normales_vertices();
	alaii.calculo_normales_vertices();
	aladi.calculo_normales_vertices();
	cilindrodi.calculo_normales_vertices();
	cilindrods.calculo_normales_vertices();
	cilindroii.calculo_normales_vertices();
	cilindrois.calculo_normales_vertices();
	cabina.calculo_normales_vertices();
	peon.calculo_normales_vertices();
	// se inicalizan la ventana y los planos de corte
	Window_width=5;
	Window_height=5;
	Front_plane=15;
	Back_plane=1000;

	// se inicia la posicion del observador, en el eje z
	Observer_distance=20*Front_plane;
	Observer_angle_x=20;
	Observer_angle_y=-50;

	// se indica cual sera el color para limpiar la ventana	(r,v,a,al)
	// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
	glClearColor(1,1,1,1);

	// se habilita el z-bufer
	glEnable(GL_DEPTH_TEST);
	//
	change_projection();
	//
	glViewport(0,0,UI_window_width,UI_window_height);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char **argv)
{
	// se llama a la inicialización de glut
	glutInit(&argc, argv);
	
	// se indica las caracteristicas que se desean para la visualización con OpenGL
	// Las posibilidades son:
	// GLUT_SIMPLE -> memoria de imagen simple
	// GLUT_DOUBLE -> memoria de imagen doble
	// GLUT_INDEX -> memoria de imagen con color indizado
	// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
	// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
	// GLUT_DEPTH -> memoria de profundidad o z-bufer
	// GLUT_STENCIL -> memoria de estarcido
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	// posicion de la esquina inferior izquierdad de la ventana
	glutInitWindowPosition(UI_window_pos_x,UI_window_pos_y);
	
	// tamaño de la ventana (ancho y alto)
	glutInitWindowSize(UI_window_width,UI_window_height);
	
	// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
	// al bucle de eventos)
	glutCreateWindow("Práctica 1");
	
	// asignación de la funcion llamada "dibujar" al evento de dibujo
	glutDisplayFunc(draw_scene);
	glutIdleFunc(idle);
	// asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
	glutReshapeFunc(change_window_size);
	// asignación de la funcion llamada "tecla_normal" al evento correspondiente
	glutKeyboardFunc(normal_keys);
	// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
	glutSpecialFunc(special_keys);

	// funcion de inicialización
	initialize(argc,argv);
	// inicio del bucle de eventos
	glutMainLoop();
	return 0;
}
