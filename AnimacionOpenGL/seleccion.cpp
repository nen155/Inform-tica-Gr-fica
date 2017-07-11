//-----------------------------------------------------// 
//
//  Codigo del curso de programacion de videojuegos                        
// 
// 	Capitulo 14-a   
//
//  UnSitioWeb.com para programar juegos 
//  (cc) Reconocimiento-NoComercial-SinObrasDerivadas   
//                                                
//  Codigo probado en:
//   VC++ 2005 Express con SDK Microsoft instalado  
//
//-----------------------------------------------------//

// Para cargar el icono del programa 
// como recurso.
#define IDI_USW		107

// Tengo que definir esta variable a mas de 0x0400 
// para que pueda usar WM_MOUSEWHEEL y TrackMouseEvent,
// si no, no la reconoce (ver Winuser.h)
#ifndef _WIN32_WINNT 
#define _WIN32_WINNT   0x0500      
#endif 

// Estas directivas del preprocesador le dicen a el enlazador (linker o 
// linkador o ...) que incluya estas librerias. Esto se puede hacer en
// la configuracion del proyecto en el Visual C++ pero esto me parece mas claro.
#pragma comment( lib, "openGL32.lib" )  // Biblioteca de OpenGL
#pragma comment( lib, "glu32.lib" )	    // Biblioteca Glu32 (funciones GLU)
#pragma comment( lib, "corona.lib" )    // Biblioteca Corona para graficos
// Biblioteca GLAUX de funciones auxiliares. La usaremos solo
// para poder usar objetos que vienen predefinidos, antes de que 
// podamos usar los que cargamos nosotros mismos.
#pragma comment( lib, "glaux.lib" )	    

#include <stdio.h>		// Libreria standard de entrada y salida.
#include <windows.h>    // Fichero de inclusion para programas Windows.
#include <time.h>		// Fichero de inclusion para funciones de hora.
#include <stdarg.h>	    // Fichero de inclusion para argumetos tipo "prinf".

#include <GL/gl.h>		// Los ficheros de inclusion de OpenGL.
#include <GL/glu.h>		// Estrictamente solo es necesario el primero.
						// El segundo es de funciones utiles que se podrian
						// hacer de otra manera. No nos vamos a complicar la 
						// vida y la usaremos.
// Incluimos la cabecera de la libreria GLAUX.
#include <GL/glaux.h>

// Incluiremos las funciones de la libreria Corona.
#include "corona/include/corona.h"  

HINSTANCE IdAplicacion; // Para guardar el identificador del programa
HWND IdVentana;  // Para guardar el identificador de la ventana

HDC	DevContex;	// Device Context para conectar la ventana con OpenGL.
HGLRC OGLrc;	// Render Context para la ventana OpenGL.

int AnchoVentana = 600;	  // Lo que dice el nombre de la variable
int AltoVentana = 400;	  // Lo que dice el nombre de la variable

// Definimos un rectangulo (programacion windows)
// lo usaremos para almacenar el area cliente de nuestra ventana
RECT rect; 

GLuint tex1, tex2;  // Para los id's de texturas que genera OpenGL. 

// Matrices con los datos de iluminacion que vamos a usar.
// Se podria definir localmente, en las funciones que se usan
// pero yo lo pongo aqui.

// Datos de las componentes ambiente, difusa y especular
// de nuestra fuente de luz.
GLfloat ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat difusa[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat especular[] = { 0.8, 0.8, 0.8, 1.0 };
// Datos de las componentes ambiente, difusa y especular
// de el primer material. Todo 1.0 en difusa y especular sera blanco,
// la luz ambiente se reflejara poco y la escena sera oscurilla. 
GLfloat mat1_ambiente[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat mat1_difusa[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat1_especular[] = { 1.0, 1.0, 1.0, 1.0 };
// Datos de las componentes ambiente, difusa y especular
// de el segundo material. 
GLfloat mat2_ambiente[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat mat2_difusa[] = { 2.0, 2.0, 1.0, 1.0 };
GLfloat mat2_especular[] = { 1.0, 1.0, 1.0, 1.0 };

// Variable de tipo unsigned int para guardar 
// el identificativo de la "display list"
GLuint Cubo;

// Variable de tipo unsigned int para guardar 
// el identificativo base de las "display list"
// que generara para almacenar las letras.
// A partir de este, los siguientes, en orden,
// seran usados en estricto orden.
GLuint	ListaFuente=0;

// Variable para guardar nuestro calculo
// de FPS reales
double FPS_reales;
// Variable donde ponemos los FPS a los que
// queremos que vaya nuestro programa.
int FPS_que_queremos=30;

// Defino un contador para girar los cubos.
int ang=0;
// Contator para alejarnos de la escena.
GLfloat dist=0;
// Dos contadores para girar la camara, una
// para cada eje a rotar.
GLfloat ang_camaraX = 0;
GLfloat ang_camaraY = 0;
// Matriz que almacena el estado del teclado.
bool Teclas[256];
// Variable para almacenar el ultimo caracter pulsado.
unsigned char Letra;
// Indicador de si son visibles los FPS en pantalla. 
bool pintaFPS=false;
//Indicadores de si hemos levantado las teclas.
// Tecla F
bool LevantaTeclaF=true;
// Tecla U
bool LevantaTeclaU=true;
// Tecla P
bool LevantaTeclaP=true;
// Tecla 8
bool LevantaTecla8=true;
// Tecla L
bool LevantaTeclaL=true;
// Tecla Raton Izquierda
bool LevantaTeclaRI=true;

// Estructura para los datos del raton que vamos a usar.
// Observad como guardamos la coordenada del raton actual
// y la del frame anterior.
struct raton
{
	int prev_x;
	int prev_y;
	int rueda;
	int x;
	int	y;
	bool izquierdo;
	bool derecho;
} Raton;

// Estructura necesaria para controlar si el raton sale de
// nuestra ventana con la funcion TrackMouseEvent.
TRACKMOUSEEVENT tme;

// Indicador para saber si estamos 
// a pantalla completa o no.
bool PantallaCompleta = false;
// Indicador de si presentamos 
// lista de resoluciones en pantalla
// o no.
bool imprime_res=false;
// Indicador de si hemos terminado
// la lista de resoluciones o no.
bool fin_res=false;
// Indice de las resoluciones de
// la tarjeta grafica.
int indice_res=0;
// Indice auxiliar de resoluciones.
int indice_res2=0;
// Texto a presentar con pantalla 
// completa. Luego lo cambiaremos
// segun el programa.
char TextoPC[200]="";

// Variable para guardar el nonbre (identificador)
// del objeto seleccionado.
int ObjetoSel = 0;
// Definimos el maximo numero de objetos que se
// pueden seleccionar.
#define MAX_SEL 64

// Identificador de si se ve la leyenda o no.
bool ImprimeLeyenda=false;

//--------------------------------------------------------------
// Funcion que crea las "display lists" necesarias
// para cada letra de la fuente.
void GeneraFuente()
{
 // Variable para guardar el identificativo
	// de una fuente de Windows
	HFONT	fuente=NULL;	
	// Otro para guardar el que esta activo ahora
	// y al terminar volver a dejar el que estaba.
	HFONT	fuente2=NULL;					
 
	// Creo la fuente que voy a convertir en listas de OpenGL.
	fuente = CreateFont(-14, // Altura del caracter				
						0,	// Ancho del caracter. 0 es proporcionado a la altura.
						0,	// Angulo de escape
						0,	// Angulo respecto a X
						FW_MEDIUM,	// Grosor del caracter. 0 es por defecto. Valor de 0 a 1000.
						FALSE,		// Italica
						FALSE,		// Subrallada
						FALSE,		// Tachada
						ANSI_CHARSET,	// Conjunto de caracteres
						OUT_TT_PRECIS,	// Tipo de fuente preferida
						CLIP_DEFAULT_PRECIS,	// Precision de recorte
						ANTIALIASED_QUALITY,	// Calidad de salida
						FF_DONTCARE|DEFAULT_PITCH,	// Familia y �pitch?
						//L"Comic Sans MS");	// Fuente del sistema a usar	
						L"Times New Roman");	// Fuente del sistema a usar	

	  
	// Selecciono la fuente que acabo de crear como la actual
	// en el contexto de dispositivo de nuestra ventana.
	fuente2 = (HFONT)SelectObject(DevContex, fuente);	 
   
	// Creo 223 "display lists" para guardar las letras.
	// Todos los caracteres ASCII desde el espacio en 
	// blanco (codigo 32).
	ListaFuente = glGenLists(256-32);		

	// Usamos esta funcion de OpenGL especifica de Windows
	// para crear 223 listas a partir del identificativo de ListaFuente
	// y a partir del 32 con la fuente del contexto de dispositivo.
	wglUseFontBitmaps(DevContex, 32, 256-32, ListaFuente);	
	
	// Una vez creadas las listas restablezco la fuente anterior.
	SelectObject(DevContex, fuente2);		
	// Borro la fuente que cree.
	DeleteObject(fuente);					
}
//--------------------------------------------------------------
// Funcion para escribir texto en pantalla.
// Se hace ejecutando las "display lists" 
// creadas para cada letra en la funcion
// GeneraFuente().
// Los dos primeros parametros son la posicion
// y el resto es igual que la funcion prinf.
void Escribe(int x, int y, char *cadena,...)
{
	// Puntero para guardar	texto.
	char texto[256];	
	// Tipo de datos para argumentos variables.
	va_list	ap;
	// Si no mando nada a escribir me voy.
	if (cadena == NULL)	return;				
	
	// Con las siguientes 3 lineas proceso la
	// entrada tipo "printf" de mi funcion,
	// los argumentos variables y su encaje en 
	// el texto general en base a las "%" que encuentre. 
	va_start(ap, cadena);									
	vsprintf(texto, cadena, ap);						
	va_end(ap);					

	// Compruebo si esta habilitada la iluminacion,
	// texturas y profundidad para dejarlas luego igual.
	bool luz = glIsEnabled(GL_LIGHTING);
	bool tex = glIsEnabled(GL_TEXTURE_2D);
	bool pro = glIsEnabled(GL_DEPTH_TEST);
	// Ahora deshabilito la iluminacion.
	glDisable(GL_LIGHTING);
	// Deshabilito Texturas.
	glDisable(GL_TEXTURE_2D); 
	// Deshabilito profundidad.
	glDisable(GL_DEPTH_TEST);

	// Este bloque es para guardar las matrices de 
	// OpenGL y dejarlas igual al final.
	// Selecciono matriz de projeccion.
	glMatrixMode(GL_PROJECTION);
	// La guardo en la pila.
	glPushMatrix();
	// Cargo la matriz identidad.
	glLoadIdentity();
	// Pongo proyeccion ortogonal.
	glOrtho(0,rect.right,rect.bottom,0,-1,1);
	// Selecciono matriz de modelo.
	glMatrixMode(GL_MODELVIEW);
	// La guardo en la pila.
	glPushMatrix();
	// Cargo la matriz identidad.
	glLoadIdentity();

	// El color con el que voy a pintar.
	glColor3f(1,1,1);

	// Le indico donde va a empezar a pintar de la
	// pantalla en base a los parametros de nuestra
	// funcion.
	glRasterPos2i(x,y);

	// Esta funcion guarda los valores de lista. 
	glPushAttrib(GL_LIST_BIT);				
	// Establecemos el identificativo base de las listas
	// a usar (-32 porque el caracter de la fuente con valor
	// ASCII mas bajo es el espacio cuyo valor es 32, y de ahi 
	// para arriba.
	glListBase(ListaFuente-32);		
	// Ejecuto tantas listas como la longitud del texto a 
	// escribir, los indices son del tipo unsigned byte y 
	// los indices de las listas estan en el puntero que se 
	// pasa como tercer parametro(los codigos ASCII del texto).
	// A estos indices se les suma la base establecida en la  
	// funcion anterior.
	glCallLists(strlen(texto), GL_UNSIGNED_BYTE, texto);	
	// Restablezco los valores de lista.
	glPopAttrib();
	
	// Restablezco como color el blanco, nuestro color base.
	glColor3f(1,1,1);
	
	// Restablezco la matrix de modelo anterior.
	glPopMatrix();
	// Cambio a la matriz de projeccion.
	glMatrixMode(GL_PROJECTION);
	// Restablezco la matrix de proyeccion anterior.
	glPopMatrix();
	// Dejo como activa la matriz de modelo.
	glMatrixMode(GL_MODELVIEW);

	// Si antes estaba habilidata la iluminacion,
	// Texturas o profundidad las dejo igual.
	if(luz) glEnable(GL_LIGHTING);
	if(tex) glEnable(GL_TEXTURE_2D);
	if(pro) glEnable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
// Funcion para cargar y crear texturas
void CargaTextura(GLuint *textura, const char *fichero)
{
	// Objeto de Corona para cargar imagenes 
	corona::Image* imagen=NULL; 
	// Usamos de nuevo el objeto imagen para cargar otra imagen de disco.
	imagen = corona::OpenImage(fichero, corona::PF_R8G8B8A8);
	// Si no retorna un puntero valido, normalmente por que no encuentra
	// la imagen en disco o el fichero esta mal, termino la funcion.
	// Si ocurre esto, luego no pintara nada.
	if (!imagen)    return;

	// Genero una textura y cargo su identificativo en tex2
	glGenTextures(1, textura);
	// Le indico a OpenGL que voy a trabajar con la textura tex2
	glBindTexture(GL_TEXTURE_2D, *textura);

	// Cargo la textura normal sin generar mipmaps en OpenGL desde
	// la imagen de corona. 
	// Parametros: 1- Es de 2D; 2- Es para mipmaps manuales y no lo
	// usamos; 3- Tipo de imagen (de nuevo RBGA); 4- Ancho de la imagen;
	// 5- Alto de la imagen; 6- Borde. Nosotros 0; 7- El formato de nuevo
	// 8- Tipo en que almacena la textura; 9- Puntero a los pixels de
	// la imagen.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagen->getWidth(), imagen->getHeight(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, imagen->getPixels());

	// Indico como va a visualizarse los pixels en caso de encajar una imagen grande
	// en un cuadrado peque�o. GL_LINEAR interpola y se ve bien.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	// Indico como va a visualizarse los pixels en caso de encajar una imagen peque�a
	// en un cuadrado grande. GL_LINEAR interpola y se ve bien.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Das dos funciones siguientes son por si quiero repetir la imagen 
	// dentro del cuadrado. Tendria que cambiar las coordenadas de 
	// textura. En este ejemplo no seran necesarias.
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

	
	// El modo en que se aplica la textura sobre el color del poligono.
	// DECAL y REPLACE simplemente de pone el dibujo. MODULATE y BLEND 
	// se podrian usar con iluminacion una y para transparencia otra.
	glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_DECAL);

	// Como ya he cargado la imagen en la textura con glTexImage2D
	// en la memoria grafica, puedo liberar la memoria. 
	if(imagen!=NULL) delete imagen;
} // Fin CargaTextura


//--------------------------------------------------------------
// Funcion que genera una lista para pintar
// cubos.
void GeneraLista()
{

	// Genero una (1) lista y guardo el 
	// identificativo en "Cubo"
	Cubo = glGenLists(1);
	// Esta funcion es la que marca el inicio 
	// de la definicion de la lista.
	// Todas las funciones OpenGL entre esta 
	// funcion y glEndList() se guardaran en la lista.
	glNewList(Cubo, GL_COMPILE);

	// Le digo a OpenGL que voy a pintar y con cuadrados:
	glBegin(GL_QUADS); 
	// Cara de arriba
	// Quito (comento) los colores que puse en ejercicios anteriores
	// ya que uso texturas.
	//glColor3f(1,0,0); // rojo.
		 
	// Coordenadas de textura para el primer vertice.
	// Le indico que punto de la textura se va a pintar en este
	// vertice.
	glTexCoord2d(1.0,0.0);
	// Por cada vertice o por cada cara, definimos el vector normal
	// a la superficie
	glNormal3f(0,1,0);
	// Ahora la coordenada del vertice.
	glVertex3f( 1.0f, 1.0f,-1.0f);
	// Repetimos operacion. Para cada vertice una funcion
	// glTexCoor, una glNormal y una glVertex ...
	glTexCoord2d(0.0,0.0);
	glNormal3f(0,1,0);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glTexCoord2d(0.0,1.0);
	glNormal3f(0,1,0);
	glVertex3f(-1.0f, 1.0f, 1.0f);	
	glTexCoord2d(1.0,1.0);
	glNormal3f(0,1,0);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	// Cara de abajo
	//glColor3f(1,0,0); // rojo
	glTexCoord2d(1.0,0.0);
	glNormal3f(0,-1,0);
	glVertex3f( 1.0f,-1.0f, 1.0f);	
	glTexCoord2d(0.0,0.0);
	glNormal3f(0,-1,0);
	glVertex3f(-1.0f,-1.0f, 1.0f);	
	glTexCoord2d(0.0,1.0);
	glNormal3f(0,-1,0);
	glVertex3f(-1.0f,-1.0f,-1.0f);	
	glTexCoord2d(1.0,1.0);
	glNormal3f(0,-1,0);
	glVertex3f( 1.0f,-1.0f,-1.0f);			 
	// Cara frontal
	//glColor3f(0,0,1); // azul
	glTexCoord2d(1.0,0.0);
	// En esta definimos solo la primera normal para probar.
	// El resto de vertices usaran la misma.
	glNormal3f(0,0,1);
	glVertex3f( 1.0f, 1.0f, 1.0f);	
	glTexCoord2d(0.0,0.0);
	//glNormal3f(0,0,1);
	glVertex3f(-1.0f, 1.0f, 1.0f);		
	glTexCoord2d(0.0,1.0);
	//glNormal3f(0,0,1);
	glVertex3f(-1.0f,-1.0f, 1.0f);	
	glTexCoord2d(1.0,1.0);
	//glNormal3f(0,0,1);
	glVertex3f( 1.0f,-1.0f, 1.0f);	
	// Cara trasera
	//glColor3f(0,0,1); // azul
	glTexCoord2d(1.0,0.0);
	glNormal3f(0,0,-1);
	glVertex3f( 1.0f,-1.0f,-1.0f);		
	glTexCoord2d(0.0,0.0);
	glNormal3f(0,0,-1);
	glVertex3f(-1.0f,-1.0f,-1.0f);		
	glTexCoord2d(0.0,1.0);
	glNormal3f(0,0,-1);
	glVertex3f(-1.0f, 1.0f,-1.0f);	
	glTexCoord2d(1.0,1.0);
	glNormal3f(0,0,-1);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	// Cara izquierda
	//glColor3f(0,1,0); // verde
	glTexCoord2d(1.0,0.0);
	glNormal3f(-1,0,0);
	glVertex3f(-1.0f, 1.0f, 1.0f);		
	glTexCoord2d(0.0,0.0);
	//glNormal3f(-1,0,0);
	glVertex3f(-1.0f, 1.0f,-1.0f);	
	glTexCoord2d(0.0,1.0);
	//glNormal3f(-1,0,0);
	glVertex3f(-1.0f,-1.0f,-1.0f);	
	glTexCoord2d(1.0,1.0);
	//glNormal3f(-1,0,0);
	glVertex3f(-1.0f,-1.0f, 1.0f);	
	// Cara derecha
	//glColor3f(0,1,0); // verde
	glTexCoord2d(1.0,0.0);
	glNormal3f(1,0,0);
	glVertex3f( 1.0f, 1.0f,-1.0f);	
	glTexCoord2d(0.0,0.0);
	glNormal3f(1,0,0);
	glVertex3f( 1.0f, 1.0f, 1.0f);	
	glTexCoord2d(0.0,1.0);
	glNormal3f(1,0,0);
	glVertex3f( 1.0f,-1.0f, 1.0f);		
	glTexCoord2d(1.0,1.0);
	glNormal3f(1,0,0);
	glVertex3f( 1.0f,-1.0f,-1.0f);	
    glEnd();

	// Fin de la creacion de la lista.
	glEndList();

}

//------------------------------------------------------------------	
// Funcion que consulta e imprime la lista de 
// resoluciones en pantalla a partir del valor 
// del indice de resoluciones (indice_res) hasta
// donde quepa en pantalla.
void ImprimeResoluciones()
{
	// Variable del tipo DEVMODE. Es una
	// estructura con los datos necesarios
	// de un modo grafico. 
	DEVMODE dv;
	// Defino una variable para ir escribiendo
	// las resoluciones que encontremos en pantalla
	// de abajo a arriba. La inicializo con la 
	// altura de la ventana menos 10 pixels.
	int pos_y = rect.bottom-10;

	// Funcion que pregunta a la tarjeta grafica sus datos.
	// Con el parametro ENUM_CURRENT_SETTINGS devuelve los
	// datos del modo grafico activo (la resolucion que 
	// tenemos puesta ahora). Deja los datos en una estructura
	// del tipo DEVMODE (&dv).
	EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dv);

	// Pongo en pantalla (arriba) la resolucion actual.
	Escribe(10, 15,"Resolucion actual: %i x %i, %i bpp, %i hercios",
		dv.dmPelsWidth, dv.dmPelsHeight, dv.dmBitsPerPel, dv.dmDisplayFrequency);

	// Pongo el valor del indice de resoluciones (indice_res)
	// en el indice auxiliar (indice_res2) para no modificar
	// el valor original.
	indice_res2=indice_res;
	// Se ejecutara mientras la coordenada Y en la que voy 
	// a escribir sea mayor de 65 pixels (dejamos espacio
	// arriba para escribir la resolucion actual).
	while( pos_y > 65 )
	{
		// La funcion "EnumDisplaySettings", si
		// el segundo parametro es un numero, empezando
		// por 0; lo toma como un indice de las resoluciones
		// que es capaz de soportar. Si la llamas con 0 como
		// segundo parametro retornara la resolucion numero 0
		// que es capaz de soportar, con un 1 la numero 1, y 
		// asi sucesivamente. Nosotros la llamaremos con 
		// indice_res2 como parametro muchas veces sucesivas
		// e incrementando indice_res2, y asi obtendremos en 
		// dv, en cada pasada los datos de resolucion para 
		// todos los indices y formaramos la lista a imprimir.
		// Si retorna false es que ha fallado, normalmente
		// porque la lista ha terminado. 
		if(EnumDisplaySettings(NULL,indice_res2,&dv))
		{
			// Pongo en pantalla los datos de la resolucion
			// del indice en curso.
			Escribe(10, pos_y,"%i: %i x %i, %i bpp, %i hercios",
				indice_res2,
				dv.dmPelsWidth, dv.dmPelsHeight, dv.dmBitsPerPel, 
				dv.dmDisplayFrequency);
			// Muevo la posicion Y donde escribire la
			// proxima pasada.
			pos_y-=15;
			// Incremento el indice de resoluciones para
			// que apunte a la proxima resolucion de la tarjeta.
			indice_res2++;
		}
		// Si falla EnumDisplaySettings es porque
		// ha terminado la lista.
		else { 
			// Asi que pongo el indicador de fin de 
			// lista a true...
			fin_res=true;
			// ... y salgo del bucle.
			break; }
	}
	


} // Fin ImprimeResoluciones

//------------------------------------------------------------------
// Funcion que cambia de programa en ventana a pantalla completa 
// y al reves. Con el parametro res a true, cuando cambia a 
// pantalla completa lo hace, ademas, ambiando la resolucion
// a 800 x 600 x 16. Sin parametro, res coje el valor por defecto,
// que es false.
void CambiaPantalla(bool res=false)
{
	// Variable del tipo DEVMODE. Es una
	// estructura con los datos necesarios
	// de un modo grafico. 
	DEVMODE dv;
	// Si este indicador es false, es que antes no 
	// estaba a pantalla completa y por tanto hay
	// que poner pantalla completa.
	if(!PantallaCompleta)
	{
		// Uso esta funcion para recuperar los datos 
		// de la resolucion actual de pantalla en dv.
		EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dv);
		// Modifico el estilo de la ventana para que no tenga
		// borde (el parametro style de la funcion "CreateWindowEx"
		SetWindowLong(IdVentana,GWL_STYLE,WS_POPUP);
		// Cambio la posicion y el tama�o de la ventana para
		// que se ajuste a la pantalla. La esquina superior 
		// izquierda en las coordenadas "0,0". La esquina inferior
		// derecha en "dv.dmPelsWidth,dv.dmPelsHeight" (tama�o  
		// X e Y de la resolucion actual de pantalla, en la 
		// estructura dv de tipo DEVMODE).
		// HWND_TOP inica que ponga la ventana encima de las demas.
		// SWP_FRAMECHANGED indica que el borde ha cambiado.
		SetWindowPos(IdVentana,HWND_TOP, 0,0,dv.dmPelsWidth, dv.dmPelsHeight, SWP_FRAMECHANGED);
		// Ponemos el mensaje de no haber cambiado 
		// la resolucion en TextoPC (luego se escribira
		// en pantalla.
		sprintf(TextoPC,"Resoluci�n sin cambiar");
		
		// En el caso de que res (el parametro
		// de esta funcion) se true es que
		// queremos, ademas, cambiar la resolucion
		// de pantalla.
		if(res)
		{
			// Pongo en los parametros correspondientes
			// de la estructura dv (tipo DEVMODE) el 
			// ancho y alto que queremos.
			dv.dmPelsWidth=800;
			dv.dmPelsHeight=600;
			// Pongo los bit por pixel
			// Con menos de 16, OpenGL no va.
			dv.dmBitsPerPel=16;
			// Podria cambiar la frecuencia tambien
			// pero �para que?.
			// Indico en la estructura los parametros que 
			// quiero modificar...
			dv.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
			// ... y cambio la resolucion y a pantalla completa.
			if(DISP_CHANGE_SUCCESSFUL==ChangeDisplaySettings(&dv,CDS_FULLSCREEN))
			{
				// Si el cambio va bien...
				// Pongo el mensaje que escribire en pantalla.
				sprintf(TextoPC,"A 800x600x16");
				// Indico el tama�o que debe tener la ventana 
				// para adecuarse a la pantalla con la nueva 
				// resolucion.
				SetWindowPos(IdVentana,HWND_TOP, 0,0,800,600,SWP_FRAMECHANGED);
			}
			// Si el cambio fallase, el programa 
			// se habria quedado a pantalla completa
			// pero sin cambiar resolucion.
			// Pongo el mensaje para saber que ha fallado.
			else sprintf(TextoPC,"Falla cambio");
		}
	}
	// En el caso de que haya que pasar de 
	// pantalla completa a verse en ventana.
	else
	{
		// Cambio en estilo de la ventana al
		// original de cuando creamos la ventana.
		SetWindowLong(IdVentana,GWL_STYLE,WS_POPUPWINDOW|WS_CAPTION|WS_MINIMIZEBOX);
		// Pongo la ventana donde al principio y 
		// con el tama�o original. 
		SetWindowPos(IdVentana,HWND_TOP, 100,100,AnchoVentana, AltoVentana,SWP_FRAMECHANGED);
		// Dejo la resolucion original, por si estaba cambiada.
		ChangeDisplaySettings(NULL,0);
		// Redibujo toda la pantalla, desde el tapiz 
		// de windows hasta la ultima ventana.
		// Con esto evito que el resto (las ventanas que 
		// no son de nuestro programa) se queden en negro.
		RedrawWindow(NULL,NULL,NULL,RDW_ALLCHILDREN|RDW_INVALIDATE );
		// Quito el mensaje de pantalla completa.
		// En realidad no haria falta pero por si acaso.
		sprintf(TextoPC,"");
	}
	// Cambio el indicador de pantalla completa al
	// contrario de como estaba.
	PantallaCompleta=!PantallaCompleta;
	
	
	// Tras esto debemos recalcular el area cliente de 
	// la ventana como al empezar y ajustar el viewport 
	// de OpenGL y poner la prespectiva otra vez en base
	// a las nuevas dimensiones.

	// Extraemos el area cliente de nuestra ventana en rect.
	GetClientRect(IdVentana, &rect); 
	// Le digo a OpenGL que voy a cambiar la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	// Usamos rect (el area cliente o espacio util dentro de los bordes de nuestra ventana)
	// para definir los limites del espacio que va a usar OpenGL para dibujar (viewport).
	// Asi ajustamos nuestra ventana y OpenGL.
	glViewport(rect.left,rect.top,rect.right,rect.bottom);  
	// Le digo a OpenGL que use proyeccion perspectiva.
	// Uso el ancho y alto de mi viewport para calcular el segundo parametro
	gluPerspective(50.0f, (float)rect.right/(float)rect.bottom, 0.5f, 50.0f);
	// Dejo la matriz de modelo de nuevo.
	glMatrixMode(GL_MODELVIEW);
	
	// Nos aseguramos de ver nuestra ventana.
	ShowWindow(IdVentana, SW_SHOW); 
	// La actualizo para que muestre lo que tenga que mostrar.
	UpdateWindow(IdVentana);  
} // Fin CambiaPanalla.


//--------------------------------------------------------------
// Funcion para pintar algo y que se repite continuamente. Se suele 
// llamar bucle de juego o principal.
void Pinta()
{
	// Borro el buffer de atras (BackBuffer), donde voy a pintar.
	// Tambien el buffer de profundidad para que no pinte lo que 
	// esta detras de otra cosa.
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	 
	// Seleccionamos la matriz de proyeccion.
	glMatrixMode(GL_PROJECTION);
	// Cargo la identidad.
	glLoadIdentity();
	// Pongo la perspectiva.
	gluPerspective(50.0f, (float)rect.right/(float)rect.bottom, 0.5f, 50.0f);
	
	// Podria hallar los datos y usar la funcion gluLookAt pero no lo voy
	// a hacer.
	//gluLookAt(0,0,3, 0,0,0, 0,1,0);
	
	// Si el boton derecho del raton esta presionado
	if(Raton.derecho)
	{
		// Si he movido el raton a la derecha
		if( Raton.prev_x > Raton.x ) ang_camaraX--; 
		// Si he movido el raton a la izquierda
		if( Raton.prev_x < Raton.x ) ang_camaraX++; 
		// Con estas dos lineas controlo que el angulo x este entre 0 y 360.
		// No es necesario pero si me dedico a dar vueltas, el programa 
		// podria hacer cosas raras.
		if(ang_camaraX < 0) ang_camaraX+=360;
		if(ang_camaraX > 360) ang_camaraX-=360;
		
		// Si he movido el raton hacia abajo
		if( Raton.prev_y > Raton.y ) ang_camaraY--;  
		// Si he movido el raton hacia arriba
		if( Raton.prev_y < Raton.y ) ang_camaraY++; 
		// Con estas dos lineas controlo que el angulo y este entre 0 y 360.
		// No es necesario pero si me dedico a dar vueltas, el programa 
		// podria hacer cosas raras.
		if(ang_camaraY < 0) ang_camaraY+=360;
		if(ang_camaraY > 360) ang_camaraY-=360;

		// Si giro la rueda del raton hacia delante
		if(Raton.rueda>0) 
		{
			// Aumento la distancia en 0.5
			dist+=0.5; 
			// Controlo que no sea mayor de 20,
			// no sea que lo pierda de vista
			if(dist>20) dist=20;
		}
		// Si giro la rueda del raton hacia atras
		if(Raton.rueda<0) 
		{
			// Reduzco la distancia en 0.5
			dist-=0.5;
			// Controlo que no sea menor de 0, asi 
			// mantengo la escena a la vista.
			if(dist<0) dist=0;
		}
	} // fin if(Raton.derecho)
    
	// Aplico los cambios en las variables a la matriz de
	// proyeccion y asi modifico la camara. Cuidado con
	// el orden de las transformaciones.
	
	// Giro a izquierda o derecha 
	glRotatef(ang_camaraX,0,1,0);
	// Giro arriba o abajo 
	glRotatef(ang_camaraY,1,0,0);
	// Me alejo o acerco de la escena.
	glTranslatef(0,0,0-dist);
	
	
	// Le digo a OpenGL que voy a cambiar la matriz de modelo
	glMatrixMode(GL_MODELVIEW);
	// Cargo la matriz identidad en la modelview, 
	// con eso me aseguro de que ningun cambio anterior modifica
	// lo que voy ha hacer despues con esta matriz. Empiezo de cero.
	// (borramos posibles traslaciones, rotacions, ...)
	glLoadIdentity();


	
	// Muevo para atras el objeto. El punto de vista esta 
	// en la posicion 0,0,0 porque no lo he cambiado, asi que 
	// alejo el objeto para poder verlo.
	glTranslatef(1,0,-4.0f);

	// Giro el objeto 30 grados en el eje x, luego otros
	// 30 en el eje y. Es para que quede bonito.
	glRotatef(30,1,0,0);

	// Controlo ahora lo que hago con teclado
	// Si presiono la tecla de la flecha abajo me alejo,
	// igual que hice con la rueda del raton. El 
	// cambio se aplicara en la roxima pasada de la
	// funcion Pinta().
	if(Teclas[VK_DOWN]) 
	{
		dist+=0.2; 
		if(dist>20) dist=20;
	}
	// Controlo tambien la tecla de la flecha arriba para
	// acercarme igual que hice con la rueda del raton 
	if(Teclas[VK_UP]) 
	{
		dist-=0.2; 
		if(dist<0) dist=0;
	}

	// Si presiono la tecla "a" o "A"
	if(Teclas['A']) 
	{
		// Aumento los FPS 
		FPS_que_queremos+=5; 
		// Controlo que no pase de 500 (por que me ha dado la gana)
		if(FPS_que_queremos>500) FPS_que_queremos=500;
	}
	// Si presiono la tecla "z" o "Z"
	if(Teclas['Z']) 
	{
		// Reduzco los FPS
		FPS_que_queremos-=5; 
		// Controlo que no baje de 25
		if(FPS_que_queremos<25) FPS_que_queremos=25;
	}

	// Si presiono la tecla "q" o "Q" 
	if(Teclas['Q']) 
	{	// Aumento el angulo de giro de los cubos 
		ang+=2; 
		// Si pasa de 360 lo hago 0
		if(ang>360) ang=0;
	}
	// Si presiono la tecla "w" o "W"
	if(Teclas['W']) 
	{	// Reduzco el angulo de giro de los cubos
		ang-=2; 
		// Si baja de 0 lo hago 360
		if(ang<0) ang=360;
	}
	// Aplico el giro a la matriz de modelo para que modifique
	// los cubos que dibujare ahora.
	glRotatef(30+ang,0,1,0);

	// En caso de presionar el boton izquierdo del raton 
	// comprobaremos si el raton esta sobre algun objeto. 
	// Usaremos el metodo habitual con un indicador auxiliar
	// para que solo se haga una vez por pulsacion.
	if(LevantaTeclaRI && Raton.izquierdo)
	{  
		// Reservamos la suficiente memoria para almacenar
		// los datos que nos retornara el sistema de seleccion
		// de OpenGL. Retornara 4 unsigned int por objeto 
		// seleccionado. En nuestro codigo hemos elegido un 
		// numero maximo de objetos (MAX_SEL) que multiplicamos 
		// por los 4 int de cada objeto seleccionado.
		GLuint BufferSeleccion[4*MAX_SEL] = {0}; 
		// Un array de 4 int's para guardar las dimensiones
		// de nuestro viewport para luego usarlo de 
		// parametro.
		GLint	viewport[4] = {0};
		// Decimos a OpenGL que use el buffer que hemos
		// definido y su tama�o. Aqui dejara OpenGL los
		// objetos seleccionados.
		glSelectBuffer(4*MAX_SEL, BufferSeleccion);
		// Cargamos en el array que hemos definido el 
		// viewport con la funcion de OpenGL para ello.
		glGetIntegerv(GL_VIEWPORT, viewport);
		// Ponemos matriz proyeccion.
		glMatrixMode(GL_PROJECTION);
		// Guardamos en la pila la matriz proyeccion.
		glPushMatrix();	
		// Cargamos la matriz identidad. 
		glLoadIdentity();
		// Usamos la funcion de la libreria glu para
		// crear una matriz especial para seleccion.
		// Le indicamos las coordenadas del raton (la Y
		// la cambiamos para que comienzen abajo) y el tama�o 
		// de una ventana, con centro en las coordenadas
		// anteriores, de 2 x 2 pixels(solo la punta del raton)
		// dentro de las cuales, si en modo seleccion pinta 
		// algo, guardara su identificativo (glPushName o 
		// glLoadName) en el array de seleccion como objeto
		// seleccionado.
		gluPickMatrix(Raton.x, viewport[3]-Raton.y, 2, 2, viewport);
		// Pongo la perspectiva actual...
		gluPerspective(50.0f, (float)rect.right/(float)rect.bottom, 0.1f, 50.0f);
		// ... y coloco la camara en el mismo sitio donde esta.
		// Mismos giros que antes.
		glRotatef(ang_camaraX,0,1,0); 
		glRotatef(ang_camaraY,1,0,0);
		// Misma traslacion de antes.
		glTranslatef(0,0,0-dist);
		// No necesito luz ni texturas
		// para seleccionar.
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		// Pongo matriz de modelo.
		glMatrixMode(GL_MODELVIEW);
		// Guardo la matriz de modelo.
		glPushMatrix();
		
		// Pongo a OpenGL en modo seleccion.
		// En este modo OpenGL no pinta pero 
		// lo que pintaria en el cuadrado definido
	    // por gluPickMatrix sera incluido en el 
		// buffer de seleccion como objeto seleccionado.
		glRenderMode(GL_SELECT); 
		// Inicializa, limpia, la pila de nombres
		// de seleccion.
		glInitNames();	
		// Guarda un primer numero en la pila de
		// nombres.
		glPushName(0);	
		
		// Cambia el 0 que metimos antes
		// en la pila por un 1 (como hacer
		// glPopName y glPushName(1) ).
		// El 1 sera el identificativo (nombre)
		// de todo o que pinto despues si es 
		// seleccionado.
		glLoadName(1);
		// Pito ahora el cubo con nombre 1.
		glCallList(Cubo);
		// Muevo el siguiente cubo.
		glTranslatef(0,0,-2.0f);
		// Cambio el nombre del objeto,
		// ahora sera 2.
		glLoadName(2);
		// Pinto el cubo que tendra el nombre 2.
		glCallList(Cubo);
		// Muevo el siguiente cubo.
		glTranslatef(2,0,2.0f);
		// Cambio el nombre del objeto,
		// ahora sera 3.
		glLoadName(3);
		// Pinto el cubo que tendra el nombre 3.
		glCallList(Cubo);
		// Muevo el siguiente cubo.
		glTranslatef(2,0,-4.0f);
		// Cambio el nombre del objeto,
		// ahora sera 4.
		glLoadName(4);
		// Pinto el cubo que tendra el nombre 4.
		glCallList(Cubo);
		// Cargo matriz identidad para pintar
		// donde esta la esfera.
		glLoadIdentity();
		// La pongo donde esta.
		glTranslatef(-2.0,0,-4.0f);
		// Cambio el nombre del objeto,
		// ahora sera 5.
		glLoadName(5);
		// Pinto la esfera, que sera el objeto
		// con nombre 5
		auxSolidSphere(1);
		// Dejo vacia la pila de nombres 
		// al terminar.
		glPopName();
		// Habilito iluminacion.
		glEnable(GL_LIGHTING);
		// Restablezco la matriz modelo.
		glPopMatrix();
		// Selecciono la matriz proyeccion
		glMatrixMode(GL_PROJECTION);
		// La restablezco.
		glPopMatrix();	
		// Dejo la de matriz modelo.
		glMatrixMode(GL_MODELVIEW);
		
		// Ahora salgo del modo seleccion y 
		// vuelvo al modo normal (render).
		int NumSel = glRenderMode(GL_RENDER);
		
		// Si el numero de objetos seleccionados supera
		// el que permite nuestro buffer de seleccion,
		// OpenGL retornara -1.
		// Si es asi hago que el numero de objetos 
		// seleccionados sea el maximo.
		if(NumSel==-1) NumSel=MAX_SEL;
		// Si ha habido algun objeto seleccionado.
		if(NumSel > 0) 
		{
			// Seleccionare el que esta mas cerca en caso 
			// de haber mas de 1.
			// Una variable para guardar dato de profundidad.
			GLuint z1;
			// Guardo el nombre del primer objeto seleccionado.
			ObjetoSel=BufferSeleccion[(0*4)+3];
			// Guardo en z el dato de profundidad del 
			// primer objeto seleccionado.
			GLfloat z=(float)BufferSeleccion[(0*4)+1];
			// Un bucle para recorrer todos los objetos
			// seleccionados e ir comparando sus profundidades.
			for(GLuint i=0; i<NumSel; i++)
			{
				// Obtengo la profundidad del objeto en curso...
				z1=BufferSeleccion[(i*4)+1];
				// ... y si es menos que z ...
				if(z1 < z)
				{	// ... la pongo en z...
					z=z1;
					// ... y el nombre del objeto en la variable para eso.
					ObjetoSel=BufferSeleccion[(i*4)+3];
				}
			}
		}
		// Si no hay ninguno seleccionado pongo 0 en la variable
		// con el nombre del objeto seleccionado.
		else ObjetoSel=0;

	
		// El resto del codigo para control de la
		// tecla Raton Izquierdo
		LevantaTeclaRI=false; 
	}
	else if( !Raton.izquierdo) LevantaTeclaRI=true;

	// Definimos el material del objeto modificando la forma en 
	// como refleja cada componente de la luz que le llega.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat1_ambiente);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_difusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_especular);
	// El brillo especular peque�o (de 0 a 255).
	glMaterialf(GL_FRONT, GL_SHININESS, 20);

	
    // y pinto el objeto con coordenadas genericas alrededor
	// del eje de coordenadas. Estas coordenadas que pongo
	// ahora son modificadas por las modificaciones que
	// hemos hecho en la matriz modelview (glTranslate, glRotate).

	// Le indico a OpenGL que voy a usar texturas para pintar el 
	// objeto y que van a ser de 2 dimensiones (un dibujo normal).
	glEnable(GL_TEXTURE_2D);
	// Indico la textura que voy a usar (tex1) y que es de 2D.
	glBindTexture(GL_TEXTURE_2D, tex1);
											 
	// En vez de pintar el cubo con todas las funciones 
	// glVertex, glNormal y glTexCoord, solo tengo
	// que ejecutar la lista "Cubo".
	// Si el objeto seleccionado es este (el 1)
	// lo pintamos en wireframe (solo las lineas),
	// sin textura y deshabilitando la iluminacion.
	if(ObjetoSel==1) 
	{
		glDisable(GL_LIGHTING); 
		glDisable(GL_TEXTURE_2D); 
		glPolygonMode(GL_FRONT, GL_LINE); 
	}
	// Pinto el objeto, normal o en wireframe.
	glCallList(Cubo);
	// Si lo he pintado en lineas regreso al modo
	// normal.
	if(ObjetoSel==1) 
	{ 
		glEnable(GL_LIGHTING); 
		glEnable(GL_TEXTURE_2D); 
		glPolygonMode(GL_FRONT, GL_FILL); 
	}
			
	// Pinto otro cubo un poco mas lejos
	glTranslatef(0,0,-2.0f);
	// Si el objeto seleccionado es este (el 2)
	// lo pintamos en wireframe (solo las lineas),
	// sin textura y deshabilitando la iluminacion.
	if(ObjetoSel==2) {glDisable(GL_LIGHTING); glDisable(GL_TEXTURE_2D); glPolygonMode(GL_FRONT, GL_LINE); }
	// Pinto el objeto, normal o en wireframe.
	glCallList(Cubo);
	// Si lo he pintado en lineas regreso al modo
	// normal.
	if(ObjetoSel==2) { glEnable(GL_LIGHTING); glEnable(GL_TEXTURE_2D); glPolygonMode(GL_FRONT, GL_FILL); }

	// Desabilito pintar con texturas para pintar
	// otros dos cubos blancos
	glDisable(GL_TEXTURE_2D);

	// Pinto otro cubo, ya sin textura a un lado.
	glTranslatef(2,0,2.0f);
	// Si el objeto seleccionado es este (el 3)
	// lo pintamos en wireframe (solo las lineas)
	// y deshabilitando la iluminacion.
	if(ObjetoSel==3) {glDisable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_LINE); }
	// Pinto el objeto, normal o en wireframe.
	glCallList(Cubo);
	// Si lo he pintado en lineas regreso al modo
	// normal.
	if(ObjetoSel==3) { glEnable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_FILL); }

	// Pinto otro cubo mas lejano.
	glTranslatef(2,0,-4.0f);
	// Si el objeto seleccionado es este (el 4)
	// lo pintamos en wireframe (solo las lineas)
	// y deshabilitando la iluminacion.
	if(ObjetoSel==4) {glDisable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_LINE); }
	// Pinto el objeto, normal o en wireframe.
	glCallList(Cubo);
	// Si lo he pintado en lineas regreso al modo
	// normal.
	if(ObjetoSel==4) { glEnable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_FILL); }

	// Desabilito la aplicacion de texturas 2D.
	// Ya las habia desabilitado para pintar dos 
	// cubos blancos asi que comento esta linea.
	//glDisable(GL_TEXTURE_2D);

	// Definimos el material del objeto modificando la forma en 
	// como refleja cada componente de la luz que le llega.
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat2_ambiente);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2_difusa);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat2_especular);
	// El brillo especular peque�o (de 0 a 255).
	glMaterialf(GL_FRONT, GL_SHININESS, 20);
 
	// Cargo la matriz identidad en la matriz de modelo.
	// para colocar desde 0 el proximo objeto.
	glLoadIdentity();
	// Traslado lo proximo que voy a dibujar 
	// -2 en al eje X (2 a la izquierda) y -4 en
	// el eje Z	( 4 al fondo ).
	glTranslatef(-2.0,0,-4.0f);
	
	// y dibujo una esfera solida de 1 de radio
	// usando la libreria glaux, que para eso la
	// hemos cargado.
	// Si el objeto seleccionado es este (el 5)
	// lo pintamos en wireframe (solo las lineas)
	// y deshabilitando la iluminacion.
	if(ObjetoSel==5) {glDisable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_LINE); }
	// Pinto el objeto, normal o en wireframe.
	auxSolidSphere(1);
	// Si lo he pintado en lineas regreso al modo
	// normal.
	if(ObjetoSel==5) { glEnable(GL_LIGHTING); glPolygonMode(GL_FRONT, GL_FILL); }

	// Ahora vamos a pintar un cartel en pantalla tal y como
	// vimos en capitulos anteriores, ortogonalmente

	// Primero desabilito la iluminacion, para que 
	// OpenGL no haga el calculo de luz al pintarlo.
	// Queremos simplemente que pinte en pantalla el
	// dibujo que hemos cargado de disco, tal cual.
	glDisable(GL_LIGHTING);
 
	// Le digo a OpenGL que voy a trabajar con la matriz
	// de proyeccion.
	glMatrixMode(GL_PROJECTION);
	// Guardo la actual matriz de proyeccion en la pila de matrices
	// para volver a la proyeccion actual cuando termine.
	glPushMatrix();
	// Cargo la matriz identidad (inicializo). 
	glLoadIdentity();
	// Defino la proyeccion ortogonal, tal y como vimos en capitulos
	// anteriores
	glOrtho(0,rect.right,rect.bottom,0,-1,1);
	// Una vez puesta la proyeccion regreso a la matriz de modelo,
	// con la que pinto los objetos.
	glMatrixMode(GL_MODELVIEW);
	// Inicializo la matriz de modelo con la matriz identidad.
	glLoadIdentity();
		 
	// Habilito de nuevo la aplicacion de texturas.
	glEnable(GL_TEXTURE_2D);
	// Le indico la textura que voy a usar (tex2).
	glBindTexture(GL_TEXTURE_2D, tex2);
	// Habilito el blending para OpenGL (esta imagen sera un
	// .PNG con el fondo trasparente y quiero que se vea asi en mi
	// programa.
	glEnable(GL_BLEND);
	// Le indico como tiene que hacer el blending (para un .PNG
	// Con fondo transparente esta funcion va bien)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Muevo el cartel 10 pixels para abajo y 10 pixels a la 
	// derecha del borde de la pantalla para que no este pegado.
 	glTranslatef(10,10,0);

	// Y dibujo un cuadrado con sus correspondientes vertices 
	// y sus coordenadas de textura.
	glBegin(GL_QUADS);
	glTexCoord2d(0.0,0.0);
	glVertex3i( 0, 0,0);	
	glTexCoord2d(0.0,1.0);
	glVertex3i( 0, 75,0);	
	glTexCoord2d(1.0,1.0);
	glVertex3i( 300,75,0);		
	glTexCoord2d(1.0,0.0);
	glVertex3i( 300,0,0);	
    glEnd();	 
 
	// Desabilito el blending para que lo que haga despues no
	// me haga raros y me vuelva loco para saber porque.
	glDisable(GL_BLEND);
	// Cambio a la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	// Recupero de la pila, la matriz de proyeccion original
	// asi no tengo que volver a indicar a OpenGL como quiero la 
	// proyeccion. Sencillamente la restablezco.
	glPopMatrix();
	// Cambio de nuevo a la matriz de modelo.
	glMatrixMode(GL_MODELVIEW);
 
	// Habilito la iluminacion que habia desabilitado
	// para pintar el cartel, no sea que lo siguiente 
	// no se pinte como quiero.
	// Lo importante es no perder el control con los 
	// sucesivos cambios y que en todo momento controlemos
	// como se pinta nuestra escena.
	glEnable(GL_LIGHTING);

	
	// Este trozo de codigo es para visualizar o no en 
	// pantalla los FPS presionando la tecla "f" o "F".
	// Usamos un indicador para ver si se ha levantado 
	// la tecla. De otra forma cambiaria el estado de 
	// pintaFPS muchas veces en un solo pulsado de tecla
	// y el resultado seria imprevisible.
	if(LevantaTeclaF && Teclas['F'])
		{ pintaFPS=!pintaFPS; LevantaTeclaF=false; }
	else if( !Teclas['F']) LevantaTeclaF=true;

	// Escribo los FPS reales con 4 digitos decimales si 
	// pintaFPS es true.
	if(pintaFPS) Escribe(480,40,"FPS: %.4f",FPS_reales );



	// De la misma manera que antes, controlo que
	// solo se ejecuta una vez lo que hace la tecla U.
	if(LevantaTeclaU && Teclas['U'])
	{  
		// Llamo a la funcion de cambiar pantalla.
		// Si pone pantalla completa, lo hara sin
		// cambiar resolucion (sin parametros la funcion).
		// Si esta a pantalla completa, volvera a 
		// verse en una ventana mas peque�a.
		CambiaPantalla();
	
		// El resto del codigo para control de la
		// tecla U
		LevantaTeclaU=false; 
	}
	else if( !Teclas['U']) LevantaTeclaU=true;

	// De la misma manera que antes, controlo que
	// solo se ejecuta una vez lo que hace la tecla 8.
	if(LevantaTecla8 && Teclas['8'])
	{  
		// Llamo a la funcion de cambiar pantalla.
		// Si pone pantalla completa, lo hara cambiando
		// la resolucion (con parametro true).
		// Si esta a pantalla completa, volvera a 
		// verse en una ventana mas peque�a.
		CambiaPantalla(true);
	
		// El resto del codigo para control de la
		// tecla 8
		LevantaTecla8=false; 
	}
	else if( !Teclas['8']) LevantaTecla8=true;

	// Si esta a pantalla completa escribe TextoPC en 
	// su lugar. 
	if(PantallaCompleta) Escribe(rect.right-200,40,TextoPC);
	
	// De la misma manera que antes, controlo que
	// solo se ejecuta una vez lo que hace la tecla U.
	if(LevantaTeclaP && Teclas['P'])
	{	
		// Cambio el indicador para que imprima 
		// o no imprima en pantalla la lista de
		// resoluciones.
		imprime_res=!imprime_res; 
		// Si tiene que imprimir, actualizo 
		// el indice de resoluciones igualandolo
		// con el auxiliar. Asi lo dejo en el lugar
		// donde termino de imprimir anteriormente
		// y por lo tanto imprimira los siguientes
		// que le quepan en pantalla.
		if(imprime_res) indice_res=indice_res2;
		// Si hemos llegado al final de la lista
		// de resoluciones.
		if(fin_res) 
		{ 
			// Preparo el indicador para empezar
			// la lista la proxima vez que se presione
			// la tecla P.
			fin_res=false;
			// Inicializo el indice de resoluciones y
			// el indice auxiliar para empezar la
			// lista.
			indice_res=0;
			indice_res2=0; 
		}
	
		// El resto del codigo para control de la
		// tecla p
		LevantaTeclaP=false; 
	}
	else if( !Teclas['P']) LevantaTeclaP=true;

	// Si toca imprimir la lista de resoluciones
	// llamo a la funcion que lo hara, en base al
	// indice de resoluciones.
	if(imprime_res) ImprimeResoluciones();
	
	// De la misma manera que antes, controlo que
	// solo se ejecuta una vez lo que hace la tecla L.
	// Cambia el estado del indicador de si se ve o no la
	// leyenda.
	if(LevantaTeclaL && Teclas['L'])
		{ ImprimeLeyenda=!ImprimeLeyenda; LevantaTeclaL=false; }
	else if( !Teclas['L']) LevantaTeclaL=true;

	// En base al indicador InprimeLeyenda pintara o no 
	// la leyenda.
	if(ImprimeLeyenda)
	{
		// Escribo la leyenda de lo que se puede hacer en nuestro 
		// programa. La situo segun el tama�o de la ventana, sea
		// cual sea.
		Escribe(rect.right-300,rect.bottom-180,"8: Cambiar a Pantalla completa y de resoluci�n");
		Escribe(rect.right-300,rect.bottom-160,"U: Cambiar entre Pantalla completa y normal");
		Escribe(rect.right-300,rect.bottom-140,"P: Ver resoluciones de la tarjeta");
		// Ahora para mover el con el boton derecho
		// del raton
		Escribe(rect.right-300,rect.bottom-120,"Click derecho presionado para mover");
		Escribe(rect.right-300,rect.bottom-100,"con el raton (rueda para distancia)");
		Escribe(rect.right-300,rect.bottom-80,"F - Ver FPS");
		Escribe(rect.right-300,rect.bottom-60,"A Z - Modificar FPS");
		Escribe(rect.right-300,rect.bottom-40,"Q W - Rotar cubos");
		Escribe(rect.right-300,rect.bottom-20,"Arriba Abajo - Acercar y alejar");
	}
	// Si no pinta la Leyenda escribe como hacer que aparezca.
	else 
		Escribe(rect.right-100,rect.bottom-20,"L: Leyenda");

	// Escribo las coordenadas del raton haya donde este el raton.
	//Escribe(Raton.x,Raton.y,"Raton: %i - %i",Raton.x,Raton.y);

	// Cambio los bufferes de modo que presento lo que he dibujado en pantalla:
	SwapBuffers(DevContex);

} // Fin Pinta.
//--------------------------------------------------------------
// Funcion para inicializar OpenGL.
void IniciaGL()
{
	
	// Definimos una estructura de tipo PIXELFORMATDESCRIPTOR para definir
	// las caracteristicas graficas que queremos usar (dentro de las que nos
	// permite el OpenGL de nuestra tarjeta de video) 
 	static PIXELFORMATDESCRIPTOR pfd =
	{ 				
        sizeof(PIXELFORMATDESCRIPTOR),	// Tama�o de este descriptor
        1,								// N�mero de versi�n
        PFD_DRAW_TO_WINDOW |			// El formato debe soportar ventana
        PFD_SUPPORT_OPENGL |			// El formato debe soportar OpenGL
        PFD_DOUBLEBUFFER |				// Debe soportar Doble Buffer
        PFD_TYPE_RGBA,					// Tambi�n debe soportar RGBA
        32,								// Bits por pixels seleccionados
        0,0,0,0,0,0,					// Bits de color ignorados
        0,								// Sin buffer alpha
        0,								// Shift bit ignorado
        0,								// Buffer de acumulaci�n ignorado
        0,0,0,0,						// Bits de acumulaci�n ignorados
        32,								// Z-Buffer de 32 bits
        0,								// Sin buffer de pincel (Stencil)
        0,								// Sin buffer auxiliar
        PFD_MAIN_PLANE,					// Layer de dibujo principal
        0,								// Reservado
        0,0,0,							// Layers de m�scara ignorados
	}; 	

	// Estas funciones son las que hacen que la ventana acepte el modo grafico que 
	// queremos dado el descriptor de pixel anterior. (en el que uno de los parametros
	// es que funcione con OpenGL).

	DevContex=GetDC(IdVentana);	  // Obtengo el "device context" de la ventana.
	int PixF=ChoosePixelFormat(DevContex,&pfd);	// Busco un indice de una conbinacion
												// que coincida con mis especificaciones
	SetPixelFormat(DevContex,PixF,&pfd);	// Pongo la ventana en el formato que quiero.
	// Dos funciones de OpenGL por fin (aunque sean especificas de windows).
	OGLrc=wglCreateContext(DevContex);	// Indico a OpenGL que la ventana esta disponible para el.
	wglMakeCurrent(DevContex,OGLrc);	// Le digo a OpenGL que nuestra ventana es donde tiene 
										// que dibujar a partir de ahora.
	// A partir de ahora podemos considerar OpenGL inicializado. 
	// Aun nos queda decirle los parametros del modo en que dibujar, aunque eso se puede,
	// y de hecho se hace continuamente, cambiar durante el programa.
	// Ahora pondremos algunos parametro puramente de OpenGL de inicio.
	glClearDepth(1.0f);			// Profundidad del buffer de profundidad. Hace que lo que esta 
								// mas cerca se vea (dibuje) encima de lo que esta mas lejos.
	glDepthFunc(GL_LEQUAL);		// Comparacion del buffer de profundidad.  
	glEnable(GL_DEPTH_TEST);	// Habilita test de profundidad. A partir de ahora, lo que 
								// esta mas cerca se pinta encima.
	glClearColor(0,0,0,1.0f);	// Color del fondo. Color con el que se borra la pantalla, o la
								// ventana donde pintamos. Cuando usemos la funcion glClear.
	glShadeModel(GL_SMOOTH);    // Renderizado suave. Cuanta mejor calidad mas lento. Nosotros
								// tenemos un ordenador reciente(+ o -) y queremos que se vea bien.
	                            // En realidad las opciones son ver facetados los objetos o verlos
								// con superficie suave, y podemos elegir una u otra opcion.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);   // Calidad buena de perspectiva.
	
	 
	// Extraemos el area cliente de nuestra ventana en rect.
	GetClientRect(IdVentana, &rect); 
	// Usamos rect (el area cliente o espacio util dentro de los bordes de nuestra ventana)
	// para definir los limites del espacio que va a usar OpenGL para dibujar (viewport).
	// asi ajustamos nuestra ventana y OpenGL.
	glViewport(rect.left,rect.top,rect.right,rect.bottom);  
							// Le dice a OpenGL el tama�o en el que va a pintar en pixels.
							// Coincidira, en principio, con el tama�o de nuestra ventana y
							// empezando por 0,0 (la esquina de nuestra ventana).


	// Le digo a OpenGL que voy a cambiar la matriz de proyeccion
	glMatrixMode(GL_PROJECTION);
	// Le digo a OpenGL que use proyeccion perspectiva.
	// Uso al ancho y alto de mi viewport para calcular el segundo parametro
	gluPerspective(50.0f, (float)rect.right/(float)rect.bottom, 0.5f, 50.0f);

	// Coloco la camara y pongo hacia donde mira con la funcion gluLookAt
	gluLookAt(0,0,3, 0,0,0, 0,1,0);

	// Estas dos funciones indican a OpenGL
	// que no pinte las caras traseras de los 
	// objetos (vertices en orden de las manillas 
	// del reloj, desde donde miramos).
	// Ahorran tiempo de computo.
	glCullFace(GL_BACK)	 ;
	glEnable(GL_CULL_FACE);

	// Hago que OpenGL pinte lineas 
	// un poco mas anchas.
	glLineWidth(2);

	// Definimos una cadena de caracteres para poner en el titulo de nuestra ventana
	// el tama�o del area cliente (a mi me gusta saber el tama�o de mi ventana de dibujo)
	char cad[50]; // 50 seran suficientes caracteres.
	// Relleno mi cadena con lo que voy a poner en la ventana.
	sprintf(cad,"UnSitioWeb %i x %i",rect.right,rect.bottom);
	// Modifico el titulo de mi ventana.
	SetWindowTextA(IdVentana,cad);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	// Borramos la ventana con el color antes establecido
														// (en realidad solo el backbuffer (la pantalla oculta
														// donde en realidad estamos pintando)
														// y borramos tambien el buffer de profundidad
	SwapBuffers(DevContex);		// Le digo a nuestro espacio de dibujo que cambie "muy rapido" el buffer de 
								// dibujo por el de visualizacion y viceversa (suichear en Espanglish)

	// Genero la fuente con la que voy a escribir.
	GeneraFuente();
 
	// Cargo texturas con la funcion de 
	// una forma mas comoda.
	CargaTextura(&tex1,"dibus//mad.png");
	CargaTextura(&tex2,"dibus//usw.png");

	// Modificamos la posicion de la luz 0 de OpenGL. 
	// El cuarto parametro es un 1.0 para que sea una luz puntual.
	GLfloat posicion[] = { 0.5, 4.0, -3.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicion);

	// Habilitamos el calculo de la iluminacion.
	glEnable(GL_LIGHTING);
	// Encendemos la luz 0.
	glEnable(GL_LIGHT0);

	// Definimos cada una de las componentes de la luz 
	// que emite nuestra bombilla (la luz 0).
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, difusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, especular);
	

	// Creamos la lista para pintar despues cubos.
	GeneraLista();

} // Fin IniciaGL

//------------------------------------------------------------------									  
// Funcion para controlar lo que ocurre en la ventana segun los eventos
// que vienen de Windows. Mas tarde se asocia a la ventana que crearemos.
// De momento solo interceptamos la tecla ESC para poder salir del 
// programa cuando queramos y el evento de destruccion de ventana con el
// que terminamos la aplicacion
LRESULT FAR PASCAL ProcesaMensajes(HWND Ventana, UINT Mensaje,
     						WPARAM wParam, LPARAM lParam)
{
	switch(Mensaje) // Seg�n el mensaje recibido
	{
	// En caso de presionar el boton izquierdo del raton
	case WM_LBUTTONDOWN:
		// Pongo true en la variable para el boton
		// izquierdo de nuestra estructura.
		Raton.izquierdo = true;
		break;
	// En caso de presionar el boton derecho del raton
	case WM_RBUTTONDOWN:
		// Pongo true en la variable para el boton
		// derecho de nuestra estructura.
		Raton.derecho = true;
		break;
	// En caso de soltar el boton izquierdo del raton
	case WM_LBUTTONUP:
		// Pongo false en la variable para el boton
		// izquierdo de nuestra estructura.
		Raton.izquierdo = false;
		break;
	// En caso de soltar el boton derecho del raton
	case WM_RBUTTONUP:
		// Pongo false en la variable para el boton
		// derecho de nuestra estructura.
		Raton.derecho = false;
		break;
	// En caso de hacer doble click con el boton izquierdo del raton
	case WM_LBUTTONDBLCLK:
		
		break;
	// En caso de hacer doble click con el boton derecho del raton
	case WM_RBUTTONDBLCLK:
		
		break;
	// En caso de que el raton salga de nuestra ventana
	case WM_MOUSELEAVE:
		// Pongo false en la variable para el boton
		// izquierdo de nuestra estructura.
		// Si el raton sale de la ventana es como
		// si soltara es boton.
		Raton.izquierdo = false;
		break;
	// En caso de que se mueva el raton
	case WM_MOUSEMOVE:
		// Si se mueve fuera de la ventana genero el mensaje
		// WM_MOUSELEAVE con esta funcion
		TrackMouseEvent(&tme);
		// Guardo las nuevas coordenadas del raton que me
		// llegan en el parametro lParam en las variables
		// x e y de nuestra estructura.
		Raton.x=LOWORD(lParam);
		Raton.y=HIWORD(lParam);
		break;
	// En caso de mover la rueda del raton.
	case WM_MOUSEWHEEL:
		// Pongo el valor en la variable para la rueda
		// de nuestra estructura.
		// Hacia delante sera positivo y hacia atras 
		// sera negativo.
		Raton.rueda=(int)wParam;
		break;
	// Caso de mensaje de pulsacion de una tecla
	case WM_KEYDOWN: 
		// Pongo a true el estado de la tecla en el array que representa
		// el teclado. wParam contiene el Virtual Key Code de la tecla.
		Teclas[(int)wParam]=true;	
		
		// El caso de ESC para salir lo trato especialmente.
		switch(wParam) // y segun el contenido de la variable wParam 
		{
		case VK_ESCAPE: // ESC causa la salida del programa
			// Funcion para enviar mensaje de cerrado a la ventana
			// y despues de la aplicacion
			PostMessage(Ventana, WM_CLOSE, 0, 0); 
			break;
		} 
		break;
	// En caso de soltar una tecla
	case WM_KEYUP:
		// Actualizo el estado de la tecla en el array a false.
		Teclas[(int)wParam]=false;
		// Pongo a 0 la variable para guardar el codigo ASCII
		// de la tecla pulsada.
		Letra=0;
		break;
	// En caso de que se pulse una tecla con codigo ASCII.
    // Este evento nos servira para hacernos una especie de 
	// getchar o gets e introducir texto. 
	case WM_CHAR: 
		// Guardo el codigo ASCII generado
		Letra=(unsigned char)wParam;
		break;

	case WM_DESTROY:	// Mensaje de destrucci�n de la ventana (provocado por
						// nosotros al pulsar ESC o cerrar la ventana.
		
		// Borro las texturas de memoria. No hay que dejar basura.
		glDeleteTextures( 1, &tex1 );
		glDeleteTextures( 1, &tex2 );
		// Borro las listas.
		glDeleteLists(ListaFuente, 256-32);

		// Destruccion de el epacio de 
		// dibujo de OpenGL.
		// Si hay un Contexto de Render de
		// OpenGL es que hay que destruirlo.
		if(OGLrc) 
		{ 
			// Hago que el OGLrc no sea ya 
			// el que esta en uso.
			wglMakeCurrent(NULL,NULL);
			// Lo borro
			wglDeleteContext(OGLrc);
			// Lo hago NULL (por si acaso)
			OGLrc=NULL; 
		}
		// Libero el Dispositivo de Contexto 
		// de la Ventana.
		ReleaseDC(IdVentana,DevContex);
		// Dejo la resolucion como esta
		// en el registro de Windows, como
		// estaba antes.
		ChangeDisplaySettings(NULL,0);



		PostQuitMessage(0); // Funcion para salir del programa
		break;
	} 
	// Efectuar el proceso por defecto del mensaje (si viene cualquier mensaje
	// que no hemos usado, querremos que haga lo que suele hacer)
	return DefWindowProc(Ventana, Mensaje, wParam, lParam);
} // fin ProcesaMansajes


//------------------------------------------------------------------
// Funcion para crear la ventana de nuestro programa, que asociaremos
// al OpenGL para pintar en ella
void CreaVentana()
{
	WNDCLASS ClaseVentana; // Declaramos un tipo de ventana, el nuestro.

	//Definimos nuestro tipo de ventana...
	ClaseVentana.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Se redibujara si 
		// cambia el tama�o de la ventana horizontal y verticalmente, y un solo
		// DC (device context) para cada ventana (para cuando empezemos con OpenGL)
	ClaseVentana.lpfnWndProc = ProcesaMensajes;	 //La definimos antes para contolar
		// los mansajes como los del teclado 
	ClaseVentana.cbClsExtra = 0; //Sin memoria extra para la clase
	ClaseVentana.cbWndExtra = 0; //Sin memoria extra para la ventana
	ClaseVentana.hInstance = IdAplicacion; // Identificador del programa para asociar
		// esta ventana con este programa.
	// Carga icono de un fichero de recursos. Sin icono se pone "= NULL"
	ClaseVentana.hIcon = LoadIcon(IdAplicacion,MAKEINTRESOURCE(IDI_USW)); 
	ClaseVentana.hCursor = LoadCursor(NULL, IDC_ARROW); // Flecha normal de raton
	ClaseVentana.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // Por ahora lo ponemos blanco
	ClaseVentana.lpszMenuName = NULL; // Pasamos de menu
	ClaseVentana.lpszClassName = L"UnSitioWeb"; // Nombre de la clase (la "L" antes de la cadena es
												// por que solo admite cadenas de caracteres 
												// unicode -LPCWSTR o TCHAR- y con la "L" se convierte).

	RegisterClass(&ClaseVentana); // Registramos nuestro tipo de ventana

	IdVentana = CreateWindowEx( // Funcion que crea la ventana. Guardamos el identificativo.
			WS_EX_APPWINDOW,	// estilo extendido de ventana
			L"UnSitioWeb",		// Nombre de la clase puesto antes (la "L" antes para convertir a UNICODE)
			L"Un Sitio Web",    // Titulo que aparecera en la ventana.
			WS_POPUPWINDOW|WS_CAPTION|WS_MINIMIZEBOX,	// Parametros de como sera la ventana
														// se pueden combinar varios. 
			100,       // Posicion Horizontal. 
			100,       // Posicion Vertical. 
			AnchoVentana,       // Ancho de la ventana. 
			AltoVentana,		// Alto de la ventana. 
			(HWND) NULL,		// No depende de otra ventana.
			(HMENU) NULL,       // No le damos un menu diferente al de la clase (que es ninguno).
			IdAplicacion,       // Identificador del programa al que pertenece la ventana. Al
								// empezar lo guardamos en una variable para usarlo despues.
			(LPVOID) NULL  );   // Puntero a "no se que" datos (pasamos del tema, es para 
								// aplicaciones MIDI).


	ShowWindow(IdVentana, SW_SHOW); // Mostramos nuestra ventana.
	UpdateWindow(IdVentana);  // La actualizo para que muestre lo que tenga que mostrar (ahora nada).

	// Relleno los datos de la estructura TRACKMOUSEEVENT
	// para poder controlar cuando sale el raton de la ventana
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = IdVentana;
	tme.dwHoverTime = 1;



} // Fin CreaVentana


//------------------------------------------------------------------
// Funcion principal de un programa Windows (como main en C normal, aqui WinMain)
// el prorama empieza a ejecutarse a partir de esta funcion.
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpszCmdLine, int nCmdShow) 
{ 
	UNREFERENCED_PARAMETER(lpszCmdLine);  // Para que no moleste al compilar, no lo usamos.
	IdAplicacion = hInstance;   // Guardo el identificador del programa, luego lo usamos.

	CreaVentana(); // Funcion que crea la ventana. Definida mas arriba.
	IniciaGL(); // Funcion para inicializar OpenGL. Definida mas arriba.
  
	
 	// Este es el bucle habitual de windows que se esta ejecutando continuamente hasta que recibe 
	// el mensaje de acabar (ya lo hemos preparado en la funcion "ProcesaMensajes" asociada a 
	// la ventana, tambien asociada a este programa atraves de su identificativo).
	MSG Mensaje; // Varible para contener los mensajes que van llegando.

	// Definimos tres variables de 64 bits que usaremos para 
	// obtener el dato de tiempo en cada pasada del bucle
	LARGE_INTEGER t1,t2,frecuencia;
	// Esta variable de tipo coma flotante de doble precision
	// para guardar el tiempo en segundos que tarda cada pasada
	// del programa.
	double segundos=0;

	// Obtenemos la primera toma de tiempo en t1.
	QueryPerformanceCounter(&t1);
	// Obtenemos la frecuencia del procesador para
	// calcular, despues, en segundos lo que tarda 
	// nuestro bucle en base a dos tomas de tiempo 
	// con "QueryPerformanceCounter". 
	QueryPerformanceFrequency(&frecuencia);
	
	// Entramos en el bucle del programa
	while(TRUE) // Se ejecuta continuamente.
	{	
		
		// Tratamos los mensajes que haya de la forma habitual
		if(PeekMessage(&Mensaje, NULL, 0, 0, PM_NOREMOVE))	// Exploramos la cola de mensajes.
		{// proces�ndolos adecuadamente
			if(!GetMessage(&Mensaje, NULL, 0, 0)) return (int)Mensaje.wParam; // En este caso terminamos.
			TranslateMessage(&Mensaje);
			DispatchMessage(&Mensaje);
		}
		// Eliminamos la espera. Ahora queremos ejecutar continuamente
		// aunque no haya mensajes.
		//else WaitMessage();	// en caso contrario esperamos un mensaje
		
		
		// Tomo el segundo tiempo en t2
		QueryPerformanceCounter(&t2);
		// Calculo los segundos en base a los dos tiempos, t1 y t2.
		// t2, excepto en la primera pasada del bucle, sera siempre
		// mayor que t1 (segundos es un decir, sera muuucho menos 
		// de un segundo).
		segundos = (double)(t2.QuadPart-t1.QuadPart)/(double)frecuencia.QuadPart;
		
		// Si, en base a los segundos calculados y el frame rate que queremos,
		// toca ya pintar otra vez la escena, tomamos de nuevo tiempo en t1 y
		// pintamos de nuevo. Si no toca volvemos al principio del bucle, vemos
		// si hay mensajes y tomamos de nuevo el segundo tiempo y comprobamos
		// otra vez.
		// Aprovechamos para calcular el frame rate real (si la maquina fuera muy 
		// lenta o tuvieramos algun problema en el codigo, podria ser menor de lo 
		// que hemos fijado)
		if( (segundos) >= 1.0f/(double)FPS_que_queremos ) 
		{	
			// Tomamos el primer tiempo	en t1.
			QueryPerformanceCounter(&t1);
			// Calculamos frame rate real.
			// Sera el numero de veces que el trocito de segundo que tarda 
			// nuestro bucle, cabe en 1 segundo entero. 
			FPS_reales = 1.0f/segundos;	
			// Pintamos nuestra escena.
			Pinta();  

			// Al terminar el bucle pongo a 0 nuestro
			// valor para la rueda del raton
			Raton.rueda=0;
			// Guardo las coordenadas del raton como las anteriores 
			// ahora que las he usado como actuales.
			Raton.prev_x = Raton.x;
			Raton.prev_y = Raton.y;		
		}

	} // Fin while.
} // fin WinMain.
//------------------------------------------------------------------
