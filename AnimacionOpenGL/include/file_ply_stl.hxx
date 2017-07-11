//**********************************************************************
// class to read ply files of triangles
//
// Domingo Martín y Carlos Ureña
// Gnu Public Licence
//**********************************************************************



#include "file_ply_stl.h"


//**********************************************************************

void error( const char *msgError )
{
   using namespace std ;
   cout  << endl
         << "error: " << msgError << endl 
         << "programa terminado." << endl
         << flush ; 
        
   exit(-1);
}

// buffer de lectura usado para leer una linea completa del ply

const streamsize 
   bufferCapacity = streamsize(10)*streamsize(1024) ;
char 
   buffer[(unsigned long)(bufferCapacity)];


//**********************************************************************
//
// read: lee un archivo ply y lo carga en 'vertices' y 'faces'
//
// * 'nombreArchivo' no debe incluir la extensión .ply (se le añade antes de abrirlo)
// * si hay un error, aborta
// * elimina cualquier contenido previo en los vectores 'vertices' y 'faces'
// * lee el archivo .ply y lo carga en 'vertices' y 'faces'
// * solo admite plys con triángulos, 
// * no lee colores, coordenadas de textura, ni normales.


void _file_ply::read( const char * nombreArchivo, vector<_vertex3f> &vertices, vector<_vertex3i> &faces,vector<_vertex3f> &normales)
{

   // modelos ply en:
   // http://graphics.im.ntu.edu.tw/~robin/courses/cg03/model/
   // http://people.sc.fsu.edu/~jburkardt/data/ply/ply.html
   
   using namespace std ;
   char *str1;
   strcpy(str1,nombreArchivo);
   const char * sfn = strcat(str1,".ply") ;
   bool carga_normales=false;
   
   char token[2000] ;
   unsigned long long int 
      nVertexs = 0, 
      nFaces   = 0 ;
   unsigned
      state    = 0; // 0 antes de leer 'element vertex', 1 antes de leer 'element face', 2 después
   bool 
      inHeader = true ;
      cout<<sfn<<endl;
   ifstream src( sfn ) ; // open the file
      
   if ( ! src.is_open() ) 
   {
	   char str0[200];
	   strcat(str0,"no puedo abrir archivo '");
	   strcat(str0,sfn);
	   strcat(str0,"' para lectura."); 
      error(str0);
   }
   
   src >> token ;

   if ( strcmp(token, "ply")!=0 )
      error("el archivo de entrada no comienza con 'ply'.");

   src.getline(buffer,bufferCapacity);

   cout << "leyendo malla de triángulos de '" << sfn << "' " << flush ;

   // leer cabecera:
   
   while( inHeader )
   {
      if ( src.eof() ) 
         error("fin de archivo no esperado en la cabecera");

     src >> token ;
     
     if ( strcmp(token, "end_header")==0  )
     {  if ( state != 2 )
           error("no se encuentran elementos 'element vertex' ni 'element face' en la cabecera");
        src.getline(buffer,bufferCapacity);
        inHeader = false ;
     }
     else if ( strcmp(token, "comment")==0 )
     {  src.getline(buffer,bufferCapacity);
        //cout << "  comment: " << buffer << endl ;
     }
     else if ( strcmp(token, "format")==0 )
     {  src >> token ;
        if ( strcmp(token,"ascii")!=0 )
        {  
				
		   char str0[300];
		   
		   strcat(str0,"no se puede leer archivo en formato no 'ascii', el formato es: '");
		   strcat(str0,token);
		   strcat(str0,"'"); 
		   error(str0);
        }
        src.getline(buffer,bufferCapacity);
     }
     else if ( strcmp(token ,"element")==0 )
     {  src >> token ;
        if ( strcmp(token, "vertex")==0 )
        {  if ( state != 0 )
              error("'element vertex' va después de 'element face'");
           src >> nVertexs ;
           //cout << "  number of vertexes == " << nVertexs << endl ;
           state = 1 ;
        }
        else if ( strcmp(token, "face")==0 )
        {  if ( state != 1 )
              error("warning: 'element vertex' va después de 'element face'");
           src >> nFaces ;
           //cout << "  number of faces == " << nFaces << endl ;
           state = 2 ;
        }
        else
        {  cout << "  elemento '" << token << "' ignorado." << endl ;
        }
        src.getline(buffer,bufferCapacity);
     }
     else if ( strcmp(token, "property")==0 )
     {  
		 char floa[200];
		 char norma[10];
		 if(strcmp(norma, "nx")==0)
		 {
			 carga_normales=true;
		 }
		 src.getline(buffer,bufferCapacity); // ignore properties, so far ...
     }
   } // end of while( inHeader )

   if ( nVertexs == 0 || nFaces == 0 )
      error("hay cero vértices o cero caras en el archivo");
      
   cout << "  (" << nVertexs << " vértices, " << nFaces << " caras)" << endl << flush ;
   
      
   // leer vértices:
   normales.resize(nVertexs);
   vertices.resize( nVertexs );

   //cout << "  reading " << nVertexs << " vertexes ...." << endl << flush ;
     
   for( unsigned long long iv = 0 ; iv < nVertexs ; iv++ )
   {
      if ( src.eof() ) 
         error("fin de archivo no esperado en la lista de vértices");

      double x,y,z ;
	  double nx,ny,nz;
      src >> x >> y >> z ;
      //cout << "vertex #" << iv << " readed: (" << x << "," << y << "," << z << ")" << endl ;
	  if(carga_normales==true)
		src >> nx >> ny >> nz;
 
      
      src.getline(buffer,bufferCapacity); // ignore more properties, so far ...
      
      // add new vertex
      unsigned long long base = iv*3 ;
      vertices[iv].x = x ;
      vertices[iv].y = y ;
      vertices[iv].z = z ;
      if(carga_normales==true){
		  normales[iv].x = nx ;
		  normales[iv].y = ny ;
		  normales[iv].z = nz ;
		} 
   }

   cout << "  end vertex list" << endl << flush ;
   //cout << "  reading " << nFaces << " faces ...." << endl << flush ;
   
   // leer caras
   
   faces.resize( nFaces );
   
   for( unsigned long long ifa = 0 ; ifa < nFaces ; ifa++ )
   {
      if ( src.eof() ) 
         error("fin de archivo no esperado en la lista de caras");

      unsigned nv ;
      src >> nv ;
      //cout << "reading face #" << ifa << " with " << nv << " vertexes: " ;
      
      if ( nv != 3 )
         error("se ha encontrado una cara con un número de vértices distinto de 3.");
         
      unsigned long long iv0, iv1, iv2 ;

      src >> iv0 >> iv1 >> iv2 ;
      //cout << " " << iv0 << ", " << iv1 << ", " << iv2 << endl ;

      src.getline(buffer,bufferCapacity); // ignore more properties, so far ...

      unsigned long long base = ifa*3 ; 
      faces[ifa]._0 = iv0 ;
      faces[ifa]._1 = iv1 ;
      faces[ifa]._2 = iv2 ;
   }
	
   cout << "  end face list." << endl ;
   cout << "ply file readed" << endl << flush ;
}
