/*
Semestre 2019-2
Práctica 10: Iluminación 2 Spotlight: Se agrega código para crear spotlights.
Texturizado: se agrega código para transparencia y blending.
Skybox: Se agrega Skybox como textura ligada a la cámara. 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_texturizado.h"

#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

//Keyframes -------------------------------------------------------------------------------------------------------
void inputKeyframes(bool* keys);

float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
float posXovni = -70.0, posYovni = 33.0f, posZovni = 20.0f;
float	movOvni_x = -70.0f, movOvni_y = 33.0f, movOvni_z = 20.0f;
float giroOvni = 0;

#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movOvni_x;		//Variable para PosicionX
	float movOvni_y;		//Variable para PosicionY
	float movOvni_z;
	float movOvni_xInc;		//Variable para IncrementoX
	float movOvni_yInc;		//Variable para IncrementoY
	float movOvni_zInc;
	float giroOvni;
	float giroOvniInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movOvni_x = movOvni_x;
	KeyFrame[FrameIndex].movOvni_y = movOvni_y;
	KeyFrame[FrameIndex].movOvni_z = movOvni_z;
	KeyFrame[FrameIndex].giroOvni = giroOvni;

	FrameIndex++;
}

void resetElements(void)
{

	movOvni_x = KeyFrame[0].movOvni_x;
	movOvni_y = KeyFrame[0].movOvni_y;
	movOvni_z = KeyFrame[0].movOvni_z;
	giroOvni = KeyFrame[0].giroOvni;
}

void interpolation(void)
{
	KeyFrame[playIndex].movOvni_xInc = (KeyFrame[playIndex + 1].movOvni_x - KeyFrame[playIndex].movOvni_x) / i_max_steps;
	KeyFrame[playIndex].movOvni_yInc = (KeyFrame[playIndex + 1].movOvni_y - KeyFrame[playIndex].movOvni_y) / i_max_steps;
	KeyFrame[playIndex].movOvni_zInc = (KeyFrame[playIndex + 1].movOvni_z - KeyFrame[playIndex].movOvni_z) / i_max_steps;
	KeyFrame[playIndex].giroOvniInc = (KeyFrame[playIndex + 1].giroOvni - KeyFrame[playIndex].giroOvni) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movOvni_x += KeyFrame[playIndex].movOvni_xInc;
			movOvni_y += KeyFrame[playIndex].movOvni_yInc;
			movOvni_z += KeyFrame[playIndex].movOvni_zInc;
			giroOvni += KeyFrame[playIndex].giroOvniInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/

const float toRadians = 3.14159265f / 180.0f;
float movCoche;
float movOffset;
bool avanza;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture pastoTexture;

//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;

//PROYECTO FINAL
//VARS PROYECTO FINAL
float tazasVarEstructuraGiro = 0.0f;
float tazasVarTazaGiroPos = 0.0f;
float tazasVarTazaGiroNeg = 0.0f;

float sillasVarGiroPos = 0.0f;

//MODELOS PROYECTO FINAL
//Juego de las tazas:
Model Tazas_TazaRoja;
Model Tazas_TazaRosa;
Model Tazas_TazaDorada;
Model Tazas_TazaVerde;
Model Tazas_TazaAzul;
Model Tazas_TazaAzulGris;
Model Tazas_TazaMorada;
Model Tazas_Estructura;
Model Tazas_BaseGiratoria;

//Juego de las sillas:
Model Sillas_BaseMetalica;
Model Sillas_EstructuraGiro;

//Relleno
Model Pruebas;
Model Lampara;
Model LamparaDir;
Model Pasto;
Model Kilahuea;
Model Muro;
Model Excusado;
Texture wc;
Model Banca;
Model Carpa;
Model Tree;
Model Carrusel;
Model Ovni;
Model Globo;
Model Bath;

Model Stand1;

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//Variables camara
float lastYawEstado1, lastPitchEstado1, lastYawEstado2, lastPitchEstado2, lastYawEstado3 = -70.0f, lastPitchEstado3 = -26.0f, lastYawEstado4 = -120.0f, lastPitchEstado4 = -17.50f;
float lastYawEstado5, lastPitchEstado5;
float lastYawEstado6, lastPitchEstado6;
glm::vec3 lastPositionEstado1;
glm::vec3 lastPositionEstado2(-12.0f, 170.0f, -35.0f);
glm::vec3 lastPositionEstado3(-48.0f, 10.0f, -75.0f);
glm::vec3 lastPositionEstado4(45.0f, 15.0f, -71.0f);
glm::vec3 lastPositionEstado5;
glm::vec3 lastPositionEstado6;

//Variables globo
float velocidadMovimientoGlobos = 0.005f;
float posYglobos1 = 8.0f, posYglobos2 = 9.0f;
float posYmin = 8.0f, posYmax = 9.0f;
bool upGlobo = true;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

	Mesh* cuadro = new Mesh();
	cuadro->CreateMesh(cubo_vertices, cubo_indices, 32, 6);
	meshList.push_back(cuadro);

}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void GenerarCamino(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float rotate, Model &Textura, glm::mat4 &model, GLuint &uniformModel, GLuint &uniformSpecularIntensity, GLuint &uniformShininess) {
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
	model = glm::rotate(model, rotate * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Textura.RenderModel();
}

void GenerarObjeto(Model &Modelo, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float rotate, float ejeX, float ejeY, float ejeZ, glm::mat4& model, GLuint& uniformModel, GLuint& uniformSpecularIntensity, GLuint& uniformShininess) {
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posX, posY, posZ));
	model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ));
	model = glm::rotate(model, rotate * toRadians, glm::vec3(ejeX, ejeY, ejeZ));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Modelo.RenderModel();
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(-5.0f, 0.0f, 40.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	//CARGAR TEXTURAS
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/tierra.tga");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/plain.png");
	pisoTexture.LoadTextureA();
	pastoTexture = Texture("Textures/pasto.jpg");
	pastoTexture.LoadTexture();
	wc = Texture("Textures/wc.png");
	wc.LoadTexture();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//CARGA DE MODELOS
	//-----------------------------------------------------------------------------------------------------------------------------
	//MODELOS
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.3ds");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	Excusado = Model();
	Excusado.LoadModel("Models/toilet.obj");

	//JUEGOS MECANICOS
	//JUEGO DE LAS TAZAS
	//ESTRUCTURA
	Tazas_Estructura = Model();
	Tazas_Estructura.LoadModel("Models/Tazas-Estructura-obj.obj");
	//BASE GIRATORIA
	Tazas_BaseGiratoria = Model();
	Tazas_BaseGiratoria.LoadModel("Models/Tazas-BaseGiratoria-obj.obj");
	//TAZAS
	Tazas_TazaRoja = Model();
	Tazas_TazaRoja.LoadModel("Models/Tazas-TazaRoja-obj.obj");
	Tazas_TazaRosa = Model();
	Tazas_TazaRosa.LoadModel("Models/Tazas-TazaRosa-obj.obj");
	Tazas_TazaDorada = Model();
	Tazas_TazaDorada.LoadModel("Models/Tazas-TazaDorada-obj.obj");
	Tazas_TazaVerde = Model();
	Tazas_TazaVerde.LoadModel("Models/Tazas-TazaVerde-obj.obj");
	Tazas_TazaAzul = Model();
	Tazas_TazaAzul.LoadModel("Models/Tazas-TazaAzul-obj.obj");
	Tazas_TazaMorada = Model();
	Tazas_TazaMorada.LoadModel("Models/Tazas-TazaMorada-obj.obj");
	Tazas_TazaAzulGris = Model();
	Tazas_TazaAzulGris.LoadModel("Models/Tazas-TazaAzulGris-obj.obj");

	//JUEGO DE LAS SILLAS
	//BASE
	Sillas_BaseMetalica = Model();
	Sillas_BaseMetalica.LoadModel("Models/Sillas-BaseMetalica-obj.obj");
	Sillas_EstructuraGiro = Model();
	Sillas_EstructuraGiro.LoadModel("Models/Sillas-EstructuraGiratoria-obj.obj");

	//OBJETOS DE RELLENO
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara-obj.obj");
	LamparaDir = Model();
	LamparaDir.LoadModel("Models/Spotlight-obj.obj");
	Pruebas = Model();
	Pruebas.LoadModel("Models/carrusel.obj");
	Pasto = Model();
	Pasto.LoadModel("Models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj");
	Kilahuea = Model();
	Kilahuea.LoadModel("Models/DropTower.blend");
	Muro = Model();
	Muro.LoadModel("Models/wall.obj");
	Banca = Model();
	Banca.LoadModel("Models/banca-obj.obj");
	Carpa = Model();
	Carpa.LoadModel("Models/Tent.obj");
	Tree = Model();
	Tree.LoadModel("Models/Tree1.3ds");
	Carrusel = Model();
	Carrusel.LoadModel("Models/carrusel.obj");
	Stand1 = Model();
	Stand1.LoadModel("Models/Stand1-obj.obj");
	Ovni = Model();
	Ovni.LoadModel("Models/ovni-obj.obj");
	Globo.LoadModel("Models/balloon-obj.obj");
	Bath = Model();
	Bath.LoadModel("Models/Bath-obj.obj");

	//KEYFRAMES DECLARADOS INICIALES

	KeyFrame[0].movOvni_x = -70.0f;
	KeyFrame[0].movOvni_y = 33.0f;
	KeyFrame[0].movOvni_z = 20.0f;
	KeyFrame[0].giroOvni = 0;


	KeyFrame[1].movOvni_x = 70.0f;
	KeyFrame[1].movOvni_y = 33.0f;
	KeyFrame[1].movOvni_z = 20.0f;
	KeyFrame[1].giroOvni = 0;
	
	KeyFrame[2].movOvni_x = 70.0f;
	KeyFrame[2].movOvni_y = 33.0f;
	KeyFrame[2].movOvni_z = -121.0f;
	KeyFrame[2].giroOvni = 0;

	KeyFrame[3].movOvni_x = -70.0f;
	KeyFrame[3].movOvni_y = 33.0f;
	KeyFrame[3].movOvni_z = -121.0f;
	KeyFrame[3].giroOvni = 0;

	KeyFrame[4].movOvni_x = -70.0f;
	KeyFrame[4].movOvni_y = 33.0f;
	KeyFrame[4].movOvni_z = 20.0f;
	KeyFrame[4].giroOvni = 0;


	//---------------------------------------------------------------------------------------------------------------------------------------
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	
	unsigned int pointLightCount = 0;
	
	//LUCES PUNTUALES
	//---------------------------------------------------------------------------------------------------------------------------------------
	//Pasillo izquierda
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-13.0f, 8.0f, 25.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	//Pasillo derecha
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-13.0f, 8.0f, 5.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-13.0f, 8.0f, -15.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	pointLights[3] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-13.0f, 8.0f, -35.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	pointLights[4] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-13.0f, 8.0f, -55.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	//Pasillo derecha
	pointLights[5] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		3.0f, 8.0f, 25.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[6] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		3.0f, 8.0f, 5.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[7] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		3.0f, 8.0f, -15.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[8] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		3.0f, 8.0f, -35.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[9] = PointLight(1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		3.0f, 8.0f, -55.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	//--------------------------------------------------------------------------------------------------------------------------------------

	glm::vec3 posovni = glm::vec3(2.0f, 0.0f, 0.0f);

	unsigned int spotLightCount = 0;
	//LUCES DIRECCIONALES
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 0.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/nightsky_rt.tga");	//Derecha		EL ORDEN ES MUY IMPORTANTE
	skyboxFaces.push_back("Textures/Skybox/nightsky_lf.tga");	//Izquierda
	skyboxFaces.push_back("Textures/Skybox/nightsky_dn.tga");	//Abajo
	skyboxFaces.push_back("Textures/Skybox/nightsky_up.tga");	//Arriba
	skyboxFaces.push_back("Textures/Skybox/nightsky_bk.tga");	//Atras
	skyboxFaces.push_back("Textures/Skybox/nightsky_ft.tga");	//Adelante

	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");	//Derecha		EL ORDEN ES MUY IMPORTANTE
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");	//Izquierda
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");	//Abajo
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");	//Arriba
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");	//Atras
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");	//Adelante
	*/

	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");*/


	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 1.0f;
	avanza = 1;

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		//COMPORTAMIENTOS CAMARA
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//1:Libre
		if (mainWindow.getEstadoCamara() == 0) {
			if (mainWindow.getJustChangedCamara()) {
				//camera.setCameraFront(glm::vec3(0.0f, 0.0f, -1.0f));
				camera.yaw = lastYawEstado1;
				camera.pitch = lastPitchEstado1;
				camera.setCameraPosition(lastPositionEstado1);
				mainWindow.doneJustChangedCamera();
			}
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			lastYawEstado1 = camera.yaw;
			lastPitchEstado1 = camera.pitch;
			lastPositionEstado1 = camera.getCameraPosition();
		}
		//2:Camara Superior
		else if (mainWindow.getEstadoCamara() == 1) {
			if (mainWindow.getJustChangedCamara()) {
				//camera.setCameraFront(glm::vec3(0.0f, -1.0f, 0.0f));
				camera.yaw = lastYawEstado2;
				camera.pitch = lastPitchEstado2;
				camera.setCameraPosition(lastPositionEstado2);
				mainWindow.doneJustChangedCamera();
			}
			camera.yaw = -90.0f;
			camera.pitch = -90.0f;
			camera.mouseControl(0.0f, 0.0f);
			lastYawEstado2 = camera.yaw;
			lastPitchEstado2 = camera.pitch;
			lastPositionEstado2 = camera.getCameraPosition();
		}
		//3:Camara Tazas
		else if (mainWindow.getEstadoCamara() == 2) {
			if (mainWindow.getJustChangedCamara()) {
				camera.yaw = lastYawEstado3;
				camera.pitch = lastPitchEstado3;
				camera.setCameraPosition(lastPositionEstado3);
				mainWindow.doneJustChangedCamera();
			}
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			lastYawEstado3 = camera.yaw;
			lastPitchEstado3 = camera.pitch;
			lastPositionEstado3 = camera.getCameraPosition();
		}
		//4:Camara Sillas
		else if (mainWindow.getEstadoCamara() == 3) {
			if (mainWindow.getJustChangedCamara()) {
				camera.yaw = lastYawEstado4;
				camera.pitch = lastPitchEstado4;
				camera.setCameraPosition(lastPositionEstado4);
				mainWindow.doneJustChangedCamera();
			}
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			lastYawEstado4 = camera.yaw;
			lastPitchEstado4 = camera.pitch;
			lastPositionEstado4 = camera.getCameraPosition();
		}
		//5:Camara en Tazas
		else if (mainWindow.getEstadoCamara() == 4) {
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.setCameraPosition(lastPositionEstado5);
		}
		//6:Camara en Sillas
		else if (mainWindow.getEstadoCamara() == 5) {
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			camera.setCameraPosition(lastPositionEstado6);
		}

		//---------------------------------------------------------------------------------------------------------------------------------

		//TRIGGERS
		camera.checkTriggers();

		//---------------------------------------------------------------------------------------------------------------------------------

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();



		//Datos camara
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;

		//USO DE LUCES
		//----------------------------------------------------------------------------------------------------------------------------------------------------
		//Direccionales
		if (mainWindow.getEstadoLucesDir() || camera.getTriggerLucesDir()) {
			//Izquierda
			spotLights[0].SetFlash(glm::vec3(-66.0f, 3.0f, -42.0f), glm::vec3(1.0f, 0.0f, -1.0f));

			//Derecha
			spotLights[1].SetFlash(glm::vec3(66.0f, 3.0f, -42.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
		}
		else if (!mainWindow.getEstadoLucesDir() || !camera.getTriggerLucesDir()) {
			//Izquierda
			spotLights[0].SetFlash(glm::vec3(-66.0f, 3.0f, -42.0f), glm::vec3(0.0f, 0.0f, 0.0f));

			//Derecha
			spotLights[1].SetFlash(glm::vec3(66.0f, 3.0f, -42.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		}

		//Puntuales
		if (mainWindow.getEstadoLucesPuntuales() || camera.getTriggerLucesPoint()) {
			for (int i = 0; i < pointLightCount; i++) {
				pointLights[i].PrenderLuz();
			}
		}
		else if (!mainWindow.getEstadoLucesPuntuales() || !camera.getTriggerLucesPoint()) {
			for (int i = 0; i < pointLightCount; i++) {
				pointLights[i].ApagarLuz();
			}
		}

		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);


		//----------------------------------------------------------------------------------------------------------------------------------------------------

		//Pasto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(8.0f, 1.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		pastoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		/*
		//pruebas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 3.0f, 20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pruebas.RenderModel();
		*/



		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, -1.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();
		spotLights[2].SetPos(glm::vec3(movCoche, -1.5f, 0.0f));

		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(2.0f - movCoche, 2.0f, 0.0f);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, -90* toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		spotLights[3].SetPos(posblackhawk);*/

		/*model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.53f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino_M.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();*/

		//JUEGO DE LAS TAZAS
		//Matriz temporal:
		glm::mat4 modelTempTazas(1.0);
		glm::mat4 modelTempTazas2(1.0);
		//ESTRUCTURA:
		model = glm::mat4(1.0);
		modelTempTazas = model = glm::translate(model, glm::vec3(-41.0f, -2.0f, -95.0f));
		modelTempTazas = model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_Estructura.RenderModel();

		//BASE GIRATORIA:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(0.0f, 0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, (tazasVarEstructuraGiro)* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTempTazas = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_BaseGiratoria.RenderModel();
		//Giro base:
		/*
		tazasVarEstructuraGiro += 60.0f*deltaTime;
		if (tazasVarEstructuraGiro == 360.0f) {
			tazasVarEstructuraGiro = 0.0f;
		}
		*/

		//TAZA ROJA:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(0.35f, 0.08f, 0.4f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, tazasVarTazaGiroNeg * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaRoja.RenderModel();
		// ANIMACION TAZAS
		if (mainWindow.mueveTazas() || camera.getTriggerTazas())
		{
			tazasVarTazaGiroNeg -= 90.0f * deltaTime;
			if (tazasVarTazaGiroNeg == -360.0f) {
				tazasVarTazaGiroNeg = 0.0f;
			}
			tazasVarEstructuraGiro += 60.0f * deltaTime;
			if (tazasVarEstructuraGiro == 360.0f) {
				tazasVarEstructuraGiro = 0.0f;
			}
		}

		/*
		tazasVarTazaGiroNeg -= 90.0f * deltaTime;
		if (tazasVarTazaGiroNeg == -360.0f) {
			tazasVarTazaGiroNeg = 0.0f;
		}
		*/
		//TAZA ROSA:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(2.2f, 0.08f, 2.83f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, tazasVarTazaGiroNeg * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelTempTazas2 = model;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaRosa.RenderModel();


		lastPositionEstado5 = glm::vec3(modelTempTazas2[3].x, modelTempTazas2[3].y+2.0f, modelTempTazas2[3].z);

		//TAZA DORADA:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(-2.0f, 0.08f, 2.83f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, tazasVarTazaGiroNeg * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaDorada.RenderModel();

		//TAZA VERDE:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(-3.15f, 0.08f, -0.78f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (-45 + tazasVarTazaGiroNeg) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaVerde.RenderModel();

		//TAZA AZUL:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(+3.5f, 0.08f, 0.2f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (45 + tazasVarTazaGiroNeg) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaAzul.RenderModel();

		//TAZA MORADA:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(2.15f, 0.08f, -2.23f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (-225 + tazasVarTazaGiroNeg) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaMorada.RenderModel();

		//TAZA GRIS:
		//model = glm::mat4(1.0);
		model = glm::translate(modelTempTazas, glm::vec3(-0.8f, 0.08f, -2.9f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, (-180 + tazasVarTazaGiroNeg) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tazas_TazaAzulGris.RenderModel();

		//JUEGO DE LAS SILLAS
		glm::mat4 modelTempSillas(1.0);
		glm::mat4 modelTempSillas2(1.0);
		//BASE GIRATORIA:
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(31.0f, -2.0f, -95.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelTempSillas = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Sillas_BaseMetalica.RenderModel();


		//ESTRUCTURA GIRATORIA:
		model = glm::translate(modelTempSillas, glm::vec3(0.0f, 0.4f, -1.83f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		modelTempSillas = model = glm::rotate(model, sillasVarGiroPos * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Sillas_EstructuraGiro.RenderModel();


		model = glm::translate(modelTempSillas, glm::vec3(3.25f, 3.0f, -1.2f));
		modelTempSillas2 = model;

		lastPositionEstado6 = glm::vec3(modelTempSillas2[3].x, modelTempSillas2[3].y,modelTempSillas2[3].z);

		//---------------------------------------------------------------------------------------------

		//Relleno
		//LAMPARAS
		//PUNTUALES
		//Izquierda
		GenerarObjeto(Lampara, -13.0f, -1.8f, 25.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, -13.0f, -1.8f, 5.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, -13.0f, -1.8f, -15.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, -13.0f, -1.8f, -35.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, -13.0f, -1.8f, -55.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Derecha
		GenerarObjeto(Lampara, 3.0f, -1.8f, 25.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, 3.0f, -1.8f, 5.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, 3.0f, -1.8f, -15.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, 3.0f, -1.8f, -35.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Lampara, 3.0f, -1.8f, -55.0f, 1.25f, 1.5f, 1.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//GLOBOS ADORNOS
		//animacion globos
		if (upGlobo) {
			posYglobos1 += velocidadMovimientoGlobos;
			posYglobos2 -= velocidadMovimientoGlobos;
			if (posYglobos1 >= posYmax)
				upGlobo = false;
		}
		else {
			posYglobos1 -= velocidadMovimientoGlobos;
			posYglobos2 += velocidadMovimientoGlobos;
			if (posYglobos1 <= posYmin)
				upGlobo = true;
		}

		//Izquierda
		GenerarObjeto(Globo, -13.0f, posYglobos1, 25.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, -13.0f, posYglobos2, 5.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, -13.0f, posYglobos1, -15.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, -13.0f, posYglobos2, -35.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, -13.0f, posYglobos1, -55.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Derecha
		GenerarObjeto(Globo, 3.0f, posYglobos2, 25.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, 3.0f, posYglobos1, 5.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, 3.0f, posYglobos2, -15.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, 3.0f, posYglobos1, -35.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Globo, 3.0f, posYglobos2, -55.0f, 0.1f, 0.1f, 0.1f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//DIRECCIONALES
		//Izquierda
		GenerarObjeto(LamparaDir, -63.0f, -1.8f, -45.0f, 1.25f, 1.5f, 1.25f, 135.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//GenerarObjeto(LamparaDir, -63.0f, -1.8f, -72.5f, 1.25f, 1.5f, 1.25f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//GenerarObjeto(LamparaDir, -63.0f, -1.8f, -110.0f, 1.25f, 1.5f, 1.25f, 45.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//Derecha
		GenerarObjeto(LamparaDir, 63.0f, -1.8f, -45.0f, 1.25f, 1.5f, 1.25f, 225.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//GenerarObjeto(LamparaDir, 63.0f, -1.8f, -72.5f, 1.25f, 1.5f, 1.25f, -90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//GenerarObjeto(LamparaDir, 63.0f, -1.8f, -110.0f, 1.25f, 1.5f, 1.25f, -45.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);


		//----------------------------------------------------------------------------------------------------------------------------
		//MUROS
		//Frente
		GenerarObjeto(Muro, 75.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 65.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 55.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 45.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 35.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 25.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 15.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 5.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -15.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -25.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -35.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -45.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -55.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -65.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -75.0f, -2.0f, 30.0f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Atras
		GenerarObjeto(Muro,  75.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  65.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  55.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  45.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  35.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  25.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  15.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,   5.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro,  -5.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -15.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -25.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -35.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -45.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -55.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -65.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -75.0f, -2.0f, -126.5f, 0.85f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Izquierda
		GenerarObjeto(Muro, -79.2f, -2.0f, 23.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, 12.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, 1.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -10.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -21.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -32.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -43.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -54.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -65.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -76.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -87.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -98.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -109.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, -79.2f, -2.0f, -120.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Derecha
		GenerarObjeto(Muro, 79.2f, -2.0f, 23.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, 12.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, 1.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -10.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -21.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -32.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -43.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -54.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -65.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -76.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -87.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -98.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -109.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Muro, 79.2f, -2.0f, -120.0f, 0.85f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//-------------------------------------------------------------------------------------------
		
		//CAMINO
		//Camino principal
		GenerarObjeto(Pasto, -5.0f, -2.0f, 26.0f, 0.03f, 0.03f, 0.03f, -90.0f,  1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, 17.0f, 0.03f, 0.03f, 0.03f, -90.0f,  1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, 8.0f, 0.03f, 0.03f, 0.03f, -90.0f,   1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -1.0f, 0.03f, 0.03f, 0.03f, -90.0f,  1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -10.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -19.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -28.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -37.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -46.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -55.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -64.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -5.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//Camino juegos
		GenerarObjeto(Pasto, 4.0f, -2.0f,  -73.0f, 0.03f, 0.03f, 0.03f, -90.0f,  1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 13.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 22.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 31.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 40.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 49.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 58.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, 67.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		GenerarObjeto(Pasto, -14.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -23.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -32.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -41.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -50.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -59.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Pasto, -68.0f, -2.0f, -73.0f, 0.03f, 0.03f, 0.03f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		
		/*
		//Kilahuea
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 25.7f, -60.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kilahuea.RenderModel();*/
		/*
		//Baños
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(27.0f, -2.0f, -29.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Excusado.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(29.0f, -2.0f, -29.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Excusado.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(31.0f, -2.0f, -29.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Excusado.RenderModel();

		//Baños cubo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(29.0f, -1.0f, -28.0f));
		model = glm::scale(model, glm::vec3(7.0f, 3.0f, 7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();
		//Divisiones
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.5f, -1.0f, -29.0f));
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(27.5f, -1.0f, -29.0f));
		model = glm::scale(model, glm::vec3(5.0f, 3.0f, 5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(29.0f, -1.0f, -23.0f));
		model = glm::scale(model, glm::vec3(7.0f, 3.0f, 7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();
		//Letrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(29.0f, 2.0f, -21.0f));
		model = glm::scale(model, glm::vec3(7.0f, 3.0f, 7.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		wc.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();*/

		//BANCAS
		//Lado izquierdo del pasillo
		GenerarObjeto(Banca, -13.0f, -1.8f, 15.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, -13.0f, -1.8f, -5.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, -13.0f, -1.8f, -25.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, -13.0f, -1.8f, -45.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		//Lado derecho del pasillo
		GenerarObjeto(Banca, 3.0f, -1.8f, 15.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, 3.0f, -1.8f, -5.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, 3.0f, -1.8f, -25.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Banca, 3.0f, -1.8f, -45.0f, 1.0f, 1.0f, 1.0f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//ANIMACION KEYFRAMES
		inputKeyframes(mainWindow.getKeys());
		animate();
		posovni = glm::vec3(posXovni + movOvni_x, posYovni + movOvni_y, posZovni + movOvni_z);
		GenerarObjeto(Ovni, posXovni + movOvni_x, posYovni + movOvni_y, posZovni + movOvni_z, 0.25f, 0.25f, 0.25f, 00.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		/*
		//Carpa de circo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Carpa.RenderModel();*/


		//STANDS
		//------------------------------------------------------------------------------------------------------
		//Stand1:
		GenerarObjeto(Stand1, -20.0f, -2.0f, -10.0f, 1.0f, 1.0f, 1.0f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Stand1, 10.0f, -2.0f, -50.0f, 1.0f, 1.0f, 1.0f, -90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Stand1, 40.0f, -2.0f, -65.0f, 1.0f, 1.0f, 1.0f, 180.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//Baños
		GenerarObjeto(Bath, 70.0f, -2.0f, -90.0f, 0.75f, 0.75f, 0.75f, -90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Bath, 70.0f, -2.0f, -100.0f, 0.75f, 0.75f, 0.75f, -90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Bath, -70.0f, -2.0f, -25.0f, 0.75f, 0.75f, 0.75f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Bath, -70.0f, -2.0f, -35.0f, 0.75f, 0.75f, 0.75f, 90.0f, 0.0f, 1.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);

		//Arboles
		GenerarObjeto(Tree, -30.0f, -2.0f, 15.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, -30.0f, -2.0f, -35.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, -50.0f, -2.0f, -5.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, -50.0f, -2.0f, -5.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, -40.0f, -2.0f, -25.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, -50.0f, -2.0f, -35.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 30.0f, -2.0f, -6.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 50.0f, 2.0f, -20.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 30.0f, -2.0f, -35.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 50.0f, -2.0f, -5.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 50.0f, -2.0f, -5.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 40.0f, -2.0f, -25.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 50.0f, -2.0f, -35.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 30.0f, -2.0f, 10.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);
		GenerarObjeto(Tree, 50.0f, -2.0f, 20.0f, 1.0f, 1.0f, 1.0f, -90.0f, 1.0f, 0.0f, 0.0f, model, uniformModel, uniformSpecularIntensity, uniformShininess);


		//ANIMACION SILLAS
		if (mainWindow.mueveSillas() || camera.getTriggerSillas())
		{
			sillasVarGiroPos += 90.0f * deltaTime;
			if (sillasVarGiroPos == 360.0f) {
				sillasVarGiroPos = 0.0f;
			}
		}
		/*
		//Giro:
		sillasVarGiroPos += 90.0f * deltaTime;
		if (sillasVarGiroPos == 360.0f) {
			sillasVarGiroPos = 0.0f;
		}
		*/

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

//ANIMACION FRAME BY FRAME

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_N])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("presiona M para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_M])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_J])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movOvni_x es: %f\n", movOvni_x);
			//printf("movOvni_y es: %f\n", movOvni_y);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_H])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			//printf("movOvni_x es: %f\n", movOvni_x);
			movOvni_x += 1.0f;
			printf("movOvni_x es: %f\n", movOvni_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_7])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}
	if (keys[GLFW_KEY_8])
	{
		if (ciclo < 1)
		{
			//printf("movOvni_x es: %f\n", movOvni_x);
			movOvni_x -= 1.0f;
			printf("movOvni_x es: %f\n", movOvni_x);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}
	}
	if (keys[GLFW_KEY_9])
	{
		if (ciclo < 1)
		{
			//printf("movOvni_x es: %f\n", movOvni_x);
			movOvni_y += 1.0f;
			printf("movOvni_y es: %f\n", movOvni_y);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_0])
	{
		if (ciclo < 1)
		{
			//printf("movOvni_x es: %f\n", movOvni_x);
			movOvni_y -= 1.0f;
			printf("movOvni_y es: %f\n", movOvni_y);
			ciclo++;
			ciclo2 = 0;
			printf("reinicia con 2\n");
		}

	}
}