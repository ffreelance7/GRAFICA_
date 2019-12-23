#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "time.h"
#include <fstream>
#include <string>
#include <vector>
#include "TextureManager.h"

#define KEY_ESC 27
#define KEY_UP 119
#define KEY_DOWN 115
#define KEY_LEFT 97
#define KEY_RIGHT 100
#define KEY_IN 116
#define KEY_OUT 103

#define PI 3.14159265358979323846

using namespace std;

float a = 0;
float x = -8;
bool Signo = true;
int mx = 0;
int my = 0;
float ax = 0.0;
float ay = 0.0;
float speed = 0.1;
bool r = false;
long hours = 0;
float lx = 0;
float ly = 0;
float lz = 0;
void full(GLfloat*& source, GLfloat*& destino, int size_) {
	for (unsigned int i = 0; i < size_; i++) {
		*(destino + i) = *(source + i);
	}
}
class Material_ {
public:
	Material_(GLfloat ambient_p[], GLfloat difuse_p[], GLfloat specular_p[], GLfloat shininess) {
		GLfloat* ambient_s = ambient;
		GLfloat* difuses_s = difuse;
		GLfloat* specular_s = specular;

		full(ambient_p, ambient_s, 4);
		full(difuse_p, difuses_s, 4);
		full(specular_p, specular_s, 4);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	GLfloat ambient[4];
	GLfloat difuse[4];
	GLfloat specular[4];
};
class Iluminacion_ {
public:
	Iluminacion_(GLfloat light0ambient_p[], GLfloat light0difuso_p[],GLfloat light0specular_p[], GLfloat light0position_p[] ) {

		
		GLfloat* light0ambient_s = light0ambient;
		GLfloat* light0difuso_s = light0difuso;
		GLfloat* light0specular_s = light0specular;		
		GLfloat* light0position_s = light0position;

		full(light0ambient_p, light0ambient_s, 4);		
		full(light0difuso_p, light0difuso_s, 4);
		full(light0specular_p, light0specular_s, 4);		
		full(light0position_p, light0position_s, 4);
		
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0difuso);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light0specular);
		glLightfv(GL_LIGHT0, GL_POSITION, light0position);
	}
	GLfloat light0position[4];
	GLfloat light0ambient[4];
	GLfloat light0difuso[4];
	GLfloat light0specular[4];
};
typedef struct terreo { 		
	GLfloat s,t;
	GLfloat nz,nx,ny;	
	//GLfloat x,z,y; 
	GLfloat z,x,y;

} PuntoTerreno;
typedef struct terreott {
	GLfloat s, t; // Coordenadas de texturas 
	//GLfloat nx, ny, nz; // Coordenadas de la normal 
} CoordeTextTerreno;
typedef struct terreotts{
	GLfloat nx, ny, nz; // Coordenadas de la normal 
} CoordeNormals;
 class Terreno { 
 public: 
	 Terreno(); 
	 bool load(const char *) ; // carga de un archivo de modelo digital de terreno
	 void display(); // Visualizacion del terreno 
	 void computeTexCoord();
	 void computeNormals(); // calcula las normales de cada vertice 
	 void load_texture(const char* Path_texture) {
		 Texture = TextureManager::Inst()->LoadTexture(Path_texture, GL_BGR_EXT, GL_RGB);
	 };
 private: 
	 //int nb_pt_x, nb_pt_z; // quantidad de punto en X y Z
	//float dist_x, dist_z; // distancia entre dos puntos segun X y Z 
	 PuntoTerreno *lista_puntos; // Tabla que contiene los puntos del terreno
	 CoordeTextTerreno* lista_texture; // Tabla que contiene los puntos del terreno
	 PuntoTerreno* lista_normals; // Tabla que contiene los puntos del terreno
	 CoordeNormals* lista_normals_vertex; // Tabla que contiene los puntos del terreno
	int *lista_indices ; // Tabla que contiene los indicess
	PuntoTerreno Matrix_Terrain[20000];
	PuntoTerreno Matrix_NormalTriangle[60000];
	CoordeTextTerreno Matrix_TextureCoor[60000];
	CoordeNormals Matrix_NormalVertex[60000];
	int Matrix_indices[200000];
	GLint Texture;
 }; 
 Terreno::Terreno() {
	 lista_puntos = Matrix_Terrain;
	 lista_indices = Matrix_indices;
	 lista_texture = Matrix_TextureCoor;
	 lista_normals = Matrix_NormalTriangle;
	 lista_normals_vertex = Matrix_NormalVertex;
 }
 void Terreno::computeNormals() {
	 PuntoTerreno P1;
	 PuntoTerreno P2;
	 PuntoTerreno P3;

	 PuntoTerreno U;
	 PuntoTerreno V;
	 GLfloat Nx;
	 GLfloat Ny;
	 GLfloat Nz;
	 PuntoTerreno N;
	 GLfloat Magnitud;
	 int pos = 0;
	 for (unsigned int i = 0; i < 60000; i += 3) {
		 P1 = Matrix_Terrain[Matrix_indices[i]];
		 P2 = Matrix_Terrain[Matrix_indices[i + 1]];
		 P3 = Matrix_Terrain[Matrix_indices[i + 2]];

		 U.x = P1.x - P2.x;
		 U.y = P1.y - P2.y;
		 U.z = P1.z - P2.z;

		 V.x = P3.x - P2.x;
		 V.y = P3.y - P2.y;
		 V.z = P3.z - P2.z;

		 Nx = (U.y * V.z)-(U.z*V.y);
		 Ny = (U.z * V.x) - (U.x * V.z);
		 Nz = (U.x * V.y) - (U.y * V.x);

		 Magnitud = sqrt(pow(Nx,2)+pow(Ny,2)+pow(Nz,2));

		 N.x = Nx/Magnitud;
		 N.y = Ny / Magnitud;
		 N.z = Nz / Magnitud;

		 Matrix_NormalTriangle[pos++]=N;		
	 }
	 pos = 0;
	 CoordeNormals M;
	 GLfloat sum_x = 0;
	 GLfloat sum_y = 0;
	 GLfloat sum_z = 0;
	 int fila = 100;
	 int last = 10200;
	/* 
	Matrix_Terrain[0].nx =0;
	 Matrix_Terrain[0].ny = 0;
	 Matrix_Terrain[0].nz = 0;

	 Matrix_Terrain[fila].nx = 0;
	 Matrix_Terrain[fila].ny = 0;
	 Matrix_Terrain[fila].nz = 0;

	 Matrix_Terrain[last-fila].nx = 0;
	 Matrix_Terrain[last - fila].ny = 0;
	 Matrix_Terrain[last - fila].nz = 0;

	 Matrix_Terrain[last].nx = 0;
	 Matrix_Terrain[last].ny = 0;
	 Matrix_Terrain[last].nz = 0;*/
	 Matrix_Terrain[pos].nx = 0;
	 Matrix_Terrain[pos].ny = 0;
	 Matrix_Terrain[pos++].nz = 0;
	 for (unsigned int i = 1; i < fila; i++) {
		 for (unsigned int j = 1; j < fila; j++) {			
					 sum_x = 0;
					 sum_y = 0;
					 sum_z = 0;

						 sum_x += Matrix_NormalTriangle[((j*2) - 1) + (2*fila * (i-1))].x;
						 sum_y += Matrix_NormalTriangle[((j*2) - 1) + (2*fila * (i - 1))].y;
						 sum_z += Matrix_NormalTriangle[((j*2) - 1) + (2*fila * (i - 1))].z;

						 sum_x += Matrix_NormalTriangle[(j*2)+(2*fila * (i - 1))].x;
						 sum_y += Matrix_NormalTriangle[(j*2)+(2*fila * (i - 1))].y;
						 sum_z += Matrix_NormalTriangle[(j*2)+(2*fila * (i - 1))].z;

						 sum_x += Matrix_NormalTriangle[((j*2) + 1) + (2*fila * (i - 1))].x;
						 sum_y += Matrix_NormalTriangle[((j*2) + 1) + (2*fila * (i - 1))].y;
						 sum_z += Matrix_NormalTriangle[((j*2) + 1) + (2*fila * (i - 1))].z;


						 sum_x += Matrix_NormalTriangle[((j * 2) - 2) + (2*fila * i)].x;
						 sum_y += Matrix_NormalTriangle[((j * 2 )- 2) + (2*fila * i)].y;
						 sum_z += Matrix_NormalTriangle[((j * 2 )- 2) + (2*fila * i)].z;
					 
						 sum_x += Matrix_NormalTriangle[((j * 2) -1)+(2*fila * i)].x;
						 sum_y += Matrix_NormalTriangle[((j * 2) -1)+(2*fila * i)].y;
						 sum_z += Matrix_NormalTriangle[((j * 2 )-1)+(2*fila * i)].z;

						 sum_x += Matrix_NormalTriangle[(j * 2)+(2*fila * i)].x;
						 sum_y += Matrix_NormalTriangle[(j * 2)+(2*fila * i)].y;
						 sum_z += Matrix_NormalTriangle[(j * 2)+(2*fila * i)].z;

					 
						 M.nx = sum_x / 6;
						 M.ny = sum_y / 6;
						 M.nz = sum_z / 6;

						 Matrix_Terrain[(i * (fila + 1)) + j].nx = M.nx;
						 Matrix_Terrain[(i * (fila + 1)) + j].ny = M.ny;
						 Matrix_Terrain[(i * (fila + 1)) + j].nz = M.nz;
						 //Matrix_NormalVertex[pos++] = M;				 
			 }

	}
	 int ini = 0;
	 int fin = fila-1;
	 for (unsigned int j = 1; j < fila; j++) {
		 sum_x = 0;
		 sum_y = 0;
		 sum_z = 0;

		 sum_x += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * ini)].x;
		 sum_y += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * ini)].y;
		 sum_z += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * ini)].z;

		 sum_x += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * ini)].x;
		 sum_y += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * ini)].y;
		 sum_z += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * ini)].z;

		 sum_x += Matrix_NormalTriangle[(j * 2) + (2 * fila * ini)].x;
		 sum_y += Matrix_NormalTriangle[(j * 2) + (2 * fila * ini)].y;
		 sum_z += Matrix_NormalTriangle[(j * 2) + (2 * fila * ini)].z;


		 M.nx = sum_x / 6;
		 M.ny = sum_y / 6;
		 M.nz = sum_z / 6;

		 //Matrix_NormalVertex[pos++] = M;
		 Matrix_Terrain[j].nx = M.nx;
		 Matrix_Terrain[j].ny = M.ny;
		 Matrix_Terrain[j].nz = M.nz;

		 sum_x = 0;
		 sum_y = 0;
		 sum_z = 0;

		 sum_x += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * fin)].x;
		 sum_y += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * fin)].y;
		 sum_z += Matrix_NormalTriangle[((j * 2) - 2) + (2 * fila * fin)].z;

		 sum_x += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * fin)].x;
		 sum_y += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * fin)].y;
		 sum_z += Matrix_NormalTriangle[((j * 2) - 1) + (2 * fila * fin)].z;

		 sum_x += Matrix_NormalTriangle[(j * 2) + (2 * fila * fin)].x;
		 sum_y += Matrix_NormalTriangle[(j * 2) + (2 * fila * fin)].y;
		 sum_z += Matrix_NormalTriangle[(j * 2) + (2 * fila * fin)].z;


		 M.nx = sum_x / 6;
		 M.ny = sum_y / 6;
		 M.nz = sum_z / 6;

		 Matrix_Terrain[100*(fila + 1) +j].nx = M.nx;
		 Matrix_Terrain[100*(fila + 1) +j].ny = M.ny;
		 Matrix_Terrain[100*(fila + 1) +j].nz = M.nz;
		 
		 //Matrix_NormalVertex[pos++] = M;
	 }
	 for (unsigned int i = 1; i < fila; i++) {
		 sum_x = 0;
		 sum_y = 0;
		 sum_z = 0;

		 sum_x += Matrix_NormalTriangle[((ini * 2) - 2) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[((ini * 2) - 2) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[((ini * 2) - 2) + (2 * fila * i)].z;

		 sum_x += Matrix_NormalTriangle[((ini * 2) - 1) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[((ini * 2) - 1) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[((ini * 2) - 1) + (2 * fila * i)].z;

		 sum_x += Matrix_NormalTriangle[(ini * 2) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[(ini * 2) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[(ini * 2) + (2 * fila * i)].z;


		 M.nx = sum_x / 6;
		 M.ny = sum_y / 6;
		 M.nz = sum_z / 6;

		 //Matrix_NormalVertex[pos++] = M;
		 Matrix_Terrain[i * (fila + 1)].nx = M.nx;
		 Matrix_Terrain[i * (fila + 1)].ny = M.ny;
		 Matrix_Terrain[i * (fila + 1)].nz = M.nz;

		 sum_x = 0;
		 sum_y = 0;
		 sum_z = 0;


		 sum_x += Matrix_NormalTriangle[((fin * 2) - 2) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[((fin * 2) - 2) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[((fin * 2) - 2) + (2 * fila * i)].z;

		 sum_x += Matrix_NormalTriangle[((fin * 2) - 1) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[((fin * 2) - 1) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[((fin * 2) - 1) + (2 * fila * i)].z;

		 sum_x += Matrix_NormalTriangle[(fin * 2) + (2 * fila * i)].x;
		 sum_y += Matrix_NormalTriangle[(fin * 2) + (2 * fila * i)].y;
		 sum_z += Matrix_NormalTriangle[(fin * 2) + (2 * fila * i)].z;


		 M.nx = sum_x / 6;
		 M.ny = sum_y / 6;
		 M.nz = sum_z / 6;

		// Matrix_NormalVertex[pos++] = M;
		 Matrix_Terrain[i * (fila+1) + 100].nx = M.nx;
		 Matrix_Terrain[i * (fila+1) + 100].ny = M.ny;
		 Matrix_Terrain[i * (fila+1) + 100].nz = M.nz;
	 }
	 //cout << "orsfsd" << endl;
 }
 void Terreno::computeTexCoord() {
	 GLfloat CantidadX = 101;
	 GLfloat CantidadY = 101;
	 
	 GLfloat Pass = 1/CantidadX; 
	 int index = 0;
	 for (unsigned int i = CantidadY-1; i >0 ;i--) {
		 for (unsigned int j = 0; j < CantidadX;j++) {			
			 Matrix_Terrain[index].s= (j* Pass);
			 Matrix_Terrain[index++].t = (i * Pass);
		 }
	 }
	 cout << "dd" << endl;
 }
 void Terreno::display() { 
//	 glBindTexture(GL_TEXTURE_2D, Texture);
	 
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, Matrix_Terrain);
	/*glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, Matrix_NormalVertex);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, Matrix_TextureCoor);*/
	
	 //glEnableClientState(GL_sVERTEX_ARRAY);
	// glVertexPointer(3, GL_FLOAT, sizeof(PuntoTerreno), lista_puntos);
	// cout <<"TEXTURE" <<Texture << endl;
	  //glDrawElements(GL_TRIANGLES, 20500, GL_UNSIGNED_INT, (void*)lista_indices);
	 glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(PuntoTerreno), &lista_puntos[0]);	
	 glDrawElements(GL_TRIANGLES, 59999, GL_UNSIGNED_INT, Matrix_indices);
		  
 }
 bool Terreno::load(const char* filename){
	 
	 vector<string>altura;	
	 fstream myfile;
	 GLfloat DistanceX=50;
	 GLfloat DistanceZ = 50;
	 GLfloat CantidadX = 101;
	 GLfloat CantidadY = 101;
	 GLfloat IniX = -2525;
	 GLfloat IniZ = -2525;
	 myfile.open(filename);
	 string line;
	 if (myfile.is_open()){
		 while (getline(myfile, line)) {
			 altura.push_back(line);
		 }		
	 }
	 PuntoTerreno Point;
	 altura.erase(altura.begin(), altura.begin()+1);
	 myfile.close();
	 int posini;
	 int posfin;
	 int N = 0;
	 int index =0;
	 int iter= 0;
	 for (unsigned int i = 0; i < CantidadY;i++) {
		 posfin = 0;
		 posini = 0;		
		 for (unsigned int j = 0; j < CantidadX;j++) {			
			 Point.x = IniX + (DistanceX * j);
			 Point.y = stof(altura[i].substr(posini, (posfin = altura[i].find(" ", posini))- posini));
			 posini = posfin+1;
			 Point.z = IniZ+(DistanceZ * i);
			 Matrix_Terrain[N++] = Point;
			 if ((i)!= CantidadY-1 && (j)!=CantidadX-1) {
				 Matrix_indices[iter++] = index;
				 Matrix_indices[iter++] = CantidadX + index++;
				 Matrix_indices[iter++] = index;
				 ///////////////////////////////////
				 Matrix_indices[iter++] = Matrix_indices[iter - 1];
				 Matrix_indices[iter++] = Matrix_indices[iter - 3];
				 Matrix_indices[iter++] = CantidadX + index;
			 } else {
				 index++;
			 }
			
		 }
		 //index = index - 5;		
	 }
	 computeTexCoord();
	 computeNormals();
	 cout << "CANTIDAD :"<<N <<"ITER"<<iter<< endl;
	 return true;
 }
 Terreno Terrain;
GLvoid callback_motion(int x, int y) {
	int dx = mx - x;
	int dy = my - y;
	mx = x;
	my = y;
	ax += dx * speed;
	ay += dy * speed;
}
GLvoid callback_mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mx = x;
		my = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		r = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		r = false;
	}
}

GLvoid callback_keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case KEY_UP:
		lz += 0.5;
		break;
	case KEY_DOWN:
		lz -= 0.5;
		break;
	case KEY_LEFT:
		lx += 0.5;
		break;
	case KEY_RIGHT:
		lx -= 0.5;
		break;
	}
	cout << "X :" << lx << endl;
	cout << "Z :" << lz << endl;
}
//dibuja un simple gizmo
void displayGizmo() {
	glBegin(GL_LINES);
	glColor3d(255, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(10000, 0, 0);
	glColor3d(0, 255, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 10000, 0);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 10000);
	glEnd();
}
void idle() {
	glutPostRedisplay();
}
void display(void) {
	float R = 10;

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //(R, G, B, transparencia) en este caso un fondo negro
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(45, 1, 0, 100);
	gluLookAt(3500, 1000, 4000, 0.0, 0.0, 0.0, 0.0, 1.0, 30.0);
	glRotatef(ax, 0, 1, 0);
	glRotatef(ay, 1, 0, 0);

	Terrain.display();
	displayGizmo();	
	glutSwapBuffers();
}
void init(void) {
	//Color del fondo de la escena
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f); //(R, G, B, transparencia) en este caso un fondo negro
	glMatrixMode(GL_PROJECTION);
	/*SET UP*/
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH); // modelo de shading try GL_FLATddd
	glEnable(GL_CULL_FACE); //no trata las caras escondidas
	glEnable(GL_DEPTH_TEST); // Activa el Z-Buffer
	glDepthFunc(GL_LEQUAL); //Modo de funcionamiento del Z-Buffer
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Activa la corrección de perspectiva
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* ILUMINATION */
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);	
	//glFrontFace(GL_CW);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("Terrain"); //titulo de la ventana
	init(); //funcion de inicializacion de OpenGL
	glutDisplayFunc(display);
	glutReshapeFunc(&window_redraw);
	
	
	GLfloat ambient[] = { 0.0f,0.0f,0.0f,1.0f };
	GLfloat diffuse[] = { 0.8f,0.8f,0.8f,1.0f };
	GLfloat specular[] = { 1.0f,1.0f,1.0f,1.0f };

	GLfloat shininess = 32.0f;

	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat light_position[] = { -5.0f, -10.0f, -10.0f, 0.0f };
	GLfloat light_position[] = { -20.0f, -10.0f, -15.0f, 0.0f };

	Material_ material(ambient, diffuse, specular, shininess);
	Iluminacion_ iluminacion(light_ambient, light_diffuse, light_specular, light_position);
	
	Terrain.load("fontvieille.txt");
	Terrain.load_texture("fontvieille.tga");
	// Callback del teclado
	glutKeyboardFunc(&callback_keyboard);
	glutMouseFunc(&callback_mouse);
	glutMotionFunc(&callback_motion);

	glutIdleFunc(&idle);
	glutMainLoop(); //bucle de rendering
	return 0;
}