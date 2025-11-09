/*
* =================================================================================
* PRACTICA.CPP (Versión Organizada)
* COMPUTACIÓN GRÁFICA E INTERACCIÓN HUMANO-COMPUTADORA
* =================================================================================
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// --- Headers de Iluminación ---
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

// --- Headers para Keyframes (NUEVO) ---
#include <fstream>
#include <iostream>
#include <string>

const float toRadians = 3.14159265f / 180.0f;

// =================================================================================
// 1. VARIABLES GLOBALES Y OBJETOS PRINCIPALES
// =================================================================================

// --- Sistema Principal ---
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;
Skybox skybox;

// --- Texturas ---
Texture brickTexture;
Texture dirtTexture;
//Texture plainTexture;
Texture pisoTexture;
Texture lamparaTexture;
Texture lampara2Texture;
Texture lampara3Texture;
Texture lampara4Texture;
Texture lampara5Texture;
Texture lampara6Texture;
Texture Juego_Pelota_Texture;
Texture PiedraTexture;
Texture AdvTime_FontTexture;
Texture Marco;
Texture Puerta;

// --- Modelos (Escenario y Ambientación) ---
Model Juego_Pelota_M;
Model Pelota_M;
Model Piedra_M;
Model Marco_M;
Model Letrero_M;
Model PuertaDerecha_M;
Model PuertaIzquierda_M;
Model PiramideT_M;
Model PiramideS_M;
Model lampara_M;
Model Globo_M;
Model Chozas_M;
Model arbol_M;
Model BancaG_M;
Model BancaT_M;
Model CalendarioA_M;
Model Kiosco_M;
Model Pillar_M;
Model Ring_M;
Model PisoRing_M;
Model Cuerdas_M;
Model Coronas_M;
Model EstatuaLuchador_M;

// --- Modelos (Personajes) ---
Model MickeyC_M;
Model MickeyT_M;
Model MickeyBD_M;
Model MickeyBI_M;
Model MickeyPD_M;
Model MickeyPI_M;
Model PeachC_M;
Model PeachT_M;
Model PeachBD_M;
Model PeachBI_M;
Model PeachPD_M;
Model PeachPI_M;
Model FinnC_M;
Model FinnT_M;
Model FinnBD_M;
Model FinnBI_M;
Model FinnPD_M;
Model FinnPI_M;
Model Jake_M;
Model NPC1_M;
Model NPC2_M;
Model NPC3_M;
Model Pluto_M;

// --- Materiales ---
Material Material_brillante;
Material Material_opaco;

// --- Iluminación ---
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// --- Control de Tiempo (DeltaTime) ---
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// --- Shaders (Rutas) ---
static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

// --- Variables de Animación (Letrero) ---
std::vector<glm::vec2> offsets = {
	glm::vec2(0.65f, 0.57f), // P
	glm::vec2(0.88f, 0.57f), // R
	glm::vec2(0.57f, 0.56f), // O
	glm::vec2(0.651f,0.46f), // Y
	glm::vec2(0.45f, 0.67f), // E
	glm::vec2(0.23f, 0.67f), // C
	glm::vec2(0.11f, 0.46f), // T
	glm::vec2(0.57f, 0.56f), // O
	glm::vec2(0.23f, 0.67f), // C
	glm::vec2(0.655f,0.67f), // G
	glm::vec2(0.45f, 0.67f), // E
	glm::vec2(0.86f, 0.67f), // I
	glm::vec2(0.77f, 0.67f), // H
	glm::vec2(0.23f, 0.67f)  // C
};
static float tiempoAcumulado = 0.0f;
static int indiceInicio = 0;
int letrasVisibles = 4;
float intervaloCambio = 0.7f;
glm::vec3 basePos(-12.0f, 2.5f, 9.5f);
float separacion = 1.5f;
glm::vec3 escalaLetras(1.5f, 2.0f, 2.0f);

// --- Variables de Animación (Puerta Izquierda 'O') ---
bool abierta = false;
float ang = 0.0f;
float vel = 90.0f;
static bool teclaOPresionada = false;

// --- Variables de Animación (Puerta Derecha 'P') ---
bool puertaDerechaAbierta = false;
bool puertaDerechaAnimando = false;
bool teclaPPresionada = false;
float progresoAnim = 0.0f;
float duracionFase = 1.0f;
int fase = 0;
glm::vec3 puertaDerechaPos(0.0f);

// --- Variables de Animación (Juego de Pelota 'J') ---
float gravedad = -45.0f;
bool pelotaSaltando = false;
bool teclaJPresionada = false;
float pelotaVelocidadY = 0.0f;
float pelotaPosBaseY = 1.5f;
float fuerzaSalto = 30.0f;
float pelotaAmplitud = 80.0f;
float pelotaVelocidad = 50.6f;
bool jakeGolpeando = false;
float jakeAnimTimer = 0.0f;
float jakeAnguloGolpe = 0.0f;
float pelotaOffset = 0.0f;
float pelotaDireccion = 1.0f;
bool avanza = true; // Esta variable no parece usarse
bool lightsA[2] = { false, false }; // Esta variable no parece usarse

// =================================================================================
// 2. MOTOR DE KEYFRAMES (Para el Globo)
// =================================================================================

// Estructura para almacenar la posición y rotación de un Keyframe
struct FRAME
{
	float posX;
	float posY;
	float posZ;
	float rotY;
};

// Función para guardar Keyframes en un archivo .txt
void saveKeyframes(const std::vector<FRAME>& keyframes, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para guardar!" << std::endl;
		return;
	}
	// Guarda cada variable en una línea
	for (const auto& frame : keyframes) {
		file << frame.posX << " " << frame.posY << " " << frame.posZ << " " << frame.rotY << "\n";
	}
	file.close();
	printf("Keyframes guardados en '%s'\n", filename.c_str());
}

// Función para leer Keyframes desde un archivo .txt
std::vector<FRAME> readKeyframes(const std::string& filename) {
	std::vector<FRAME> keyframes;
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo para leer!" << std::endl;
		return keyframes;
	}
	FRAME frame;
	while (file >> frame.posX >> frame.posY >> frame.posZ >> frame.rotY) {
		keyframes.push_back(frame);
	}
	file.close();
	printf("Keyframes cargados desde '%s'. Total: %d\n", (int)keyframes.size(), filename.c_str());
	return keyframes;
}

// Función de Interpolación (para la transición suave)
float interpolation(float a, float b, float t) {
	return a + (b - a) * t;
}

// Función principal de Animación
void animate(float& posX, float& posY, float& posZ, float& rotY,
	std::vector<FRAME>& keyframes, int& frameIndex, float& playIndex, bool& play) {

	if (play && keyframes.size() > 1) {

		// 'playIndex' va de 0.0 a 1.0. Cuando llega a 1.0, pasamos al siguiente frame.
		if (playIndex > 1.0f) {
			playIndex = 0.0f;
			frameIndex++;
		}

		// Si llegamos al final de la animación, la detenemos y reiniciamos.
		if (frameIndex >= keyframes.size() - 1) {
			frameIndex = 0;
			play = false;
			printf("Animacion terminada.\n");
		}

		// IDs de los frames actual y siguiente
		int currentFrame = frameIndex;
		int nextFrame = frameIndex + 1; // No usamos % para que la animación termine

		// Interpolamos la posición y rotación entre el frame actual y el siguiente
		posX = interpolation(keyframes[currentFrame].posX, keyframes[nextFrame].posX, playIndex);
		posY = interpolation(keyframes[currentFrame].posY, keyframes[nextFrame].posY, playIndex);
		posZ = interpolation(keyframes[currentFrame].posZ, keyframes[nextFrame].posZ, playIndex);
		rotY = interpolation(keyframes[currentFrame].rotY, keyframes[nextFrame].rotY, playIndex);

		// Aumentamos el índice de interpolación. 
		// Ajusta '0.005f' para hacer la animación más rápida o lenta.
		playIndex += 0.005f;
	}
}

//--- Variables de Animación por Keyframes del Globo
std::vector<FRAME> KeyFrameGlobo; // Vector para guardar frames
int FrameIndexGlobo = 0;
bool playGlobo = false;            // Bandera para reproducir animacion
float playIndexGlobo = 0.0f;       // Índice de interpolación 

// Posición y Rotación actual del Globo 
float globoPosX = 0.0f;
float globoPosY = 50.5f;
float globoPosZ = -180.0f;
float globoRotY = 90.0f;

bool teclaLPresionada = false;
bool teclaKPresionada = false;
bool teclaGPresionada = false;
bool teclaCPresionada = false;


//funcion de calculo de normales por promedio de vertices 
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

	unsigned int AdvTimeIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	GLfloat AdvTimeVertices[] = {
		//   X      Y      Z     U       V        Nx    Ny    Nz
		-0.5f, 0.0f,  0.5f,   0.1f,   0.98f,   0.0f, -1.0f, 0.0f, // arriba dere  
		 0.5f, 0.0f,  0.5f,   0.01f,     0.98f,   0.0f, -1.0f, 0.0f, // arriba izq
		 0.5f, 0.0f, -0.5f,   0.01f,     0.9f,  0.0f, -1.0f, 0.0f, // abajo izq
		-0.5f, 0.0f, -0.5f,   0.1f,   0.9f,  0.0f, -1.0f, 0.0f  // abajo der
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(AdvTimeVertices, AdvTimeIndices, 32, 6);
	meshList.push_back(obj5);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void setLetterUV(float* vertices, int col, int row)
{
	float uStep = 1.0f / 9.0f;  // 9 columnas
	float vStep = 1.0f / 3.0f;  // 3 filas
	float u0 = col * uStep;
	float v0 = 1.0f - (row + 1) * vStep;
	float u1 = u0 + uStep;
	float v1 = v0 + vStep;

	// Actualiza las coordenadas UV del plano
	vertices[3] = u0; vertices[4] = v1;  // arriba izq
	vertices[11] = u1; vertices[12] = v1;  // arriba der
	vertices[19] = u1; vertices[20] = v0;  // abajo der
	vertices[27] = u0; vertices[28] = v0;  // abajo izq
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// Configuración de Cámara 
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 50.0f, 0.5f);

	// Carga de Texturas 
	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	//plainTexture = Texture("Textures/plain.png");
	//plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pasto.png");
	pisoTexture.LoadTextureA();
	lamparaTexture = Texture("Textures/StreetLamp1.png");
	lamparaTexture.LoadTextureA();
	lampara2Texture = Texture("Textures/StreetLamp2BaseColor.png");
	lampara2Texture.LoadTextureA();
	lampara3Texture = Texture("Textures/StreetLamp3Metallic.png");
	lampara3Texture.LoadTextureA();
	lampara4Texture = Texture("Textures/StreetLamp4Normal.png");
	lampara4Texture.LoadTextureA();
	lampara5Texture = Texture("Textures/StreetLamp5Opacity.png");
	lampara5Texture.LoadTextureA();
	lampara6Texture = Texture("Textures/StreetLamp6Roughness.png");
	lampara6Texture.LoadTextureA();
	Juego_Pelota_Texture = Texture("Textures/Juego_Pelota.png");
	Juego_Pelota_Texture.LoadTextureA();
	PiedraTexture = Texture("Textures/piedra.png");
	PiedraTexture.LoadTextureA();
	AdvTime_FontTexture = Texture("Textures/AdvTime_Font.png");
	AdvTime_FontTexture.LoadTextureA();
	Marco = Texture("Textures/Corrugated_Metal_Sheet_teendf3q_1K_BaseColor.png");
	Marco.LoadTextureA();
	Puerta = Texture("Textures/Wooden_Log_Wall_udlmecoew_1K_BaseColor.png");
	Puerta.LoadTextureA();

	// Carga de Modelos 
	Juego_Pelota_M = Model();
	Juego_Pelota_M.LoadModel("Models/Juego_Pelota.obj");
	Pelota_M = Model();
	Pelota_M.LoadModel("Models/pelota.fbx");
	Piedra_M = Model();
	Piedra_M.LoadModel("Models/pelota.fbx");
	PiramideT_M = Model();
	PiramideT_M.LoadModel("Models/PiramideT.obj");
	PiramideS_M = Model();
	PiramideS_M.LoadModel("Models/PiramideSol.obj");
	lampara_M = Model();
	lampara_M.LoadModel("Models/lampara.obj");
	Globo_M = Model();
	Globo_M.LoadModel("Models/globo.obj");
	Chozas_M = Model();
	Chozas_M.LoadModel("Models/Chozas.obj");
	arbol_M = Model();
	arbol_M.LoadModel("Models/arboles.obj");
	BancaG_M = Model();
	BancaG_M.LoadModel("Models/banca.obj");
	BancaT_M = Model();
	BancaT_M.LoadModel("Models/Banca_Texturizada.obj");
	CalendarioA_M = Model();
	CalendarioA_M.LoadModel("Models/calendario_azteca.obj");
	Kiosco_M = Model();
	Kiosco_M.LoadModel("Models/kiosco.obj");
	Pillar_M = Model();
	Pillar_M.LoadModel("Models/pillar.obj");
	Ring_M = Model();
	Ring_M.LoadModel("Models/Ring.obj");
	PisoRing_M = Model();
	PisoRing_M.LoadModel("Models/PisoRing.obj");
	Cuerdas_M = Model();
	Cuerdas_M.LoadModel("Models/Cuerdas.obj");
	Coronas_M = Model();
	Coronas_M.LoadModel("Models/Coronas.obj");
	EstatuaLuchador_M = Model();
	EstatuaLuchador_M.LoadModel("Models/luchadores.obj");
	// Personajes
	MickeyC_M = Model();
	MickeyC_M.LoadModel("Models/MickeyC.obj");
	MickeyT_M = Model();
	MickeyT_M.LoadModel("Models/MickeyT.obj");
	MickeyBD_M = Model();
	MickeyBD_M.LoadModel("Models/MickeyBD.obj");
	MickeyBI_M = Model();
	MickeyBI_M.LoadModel("Models/MickeyBI.obj");
	MickeyPD_M = Model();
	MickeyPD_M.LoadModel("Models/MickeyPD.obj");
	MickeyPI_M = Model();
	MickeyPI_M.LoadModel("Models/MickeyPI.obj");
	PeachC_M = Model();
	PeachC_M.LoadModel("Models/PeachC.obj");
	PeachT_M = Model();
	PeachT_M.LoadModel("Models/PeachT.obj");
	PeachBD_M = Model();
	PeachBD_M.LoadModel("Models/PeachBD.obj");
	PeachBI_M = Model();
	PeachBI_M.LoadModel("Models/PeachBI.obj");
	PeachPD_M = Model();
	PeachPD_M.LoadModel("Models/PeachPD.obj");
	PeachPI_M = Model();
	PeachPI_M.LoadModel("Models/PeachPI.obj");
	FinnC_M = Model();
	FinnC_M.LoadModel("Models/FinnC.obj");
	FinnT_M = Model();
	FinnT_M.LoadModel("Models/FinnT.obj");
	FinnBD_M = Model();
	FinnBD_M.LoadModel("Models/FinnBD.obj");
	FinnBI_M = Model();
	FinnBI_M.LoadModel("Models/FinnBI.obj");
	FinnPD_M = Model();
	FinnPD_M.LoadModel("Models/FinnPD.obj");
	FinnPI_M = Model();
	FinnPI_M.LoadModel("Models/FinnPI.obj");
	Jake_M = Model();
	Jake_M.LoadModel("Models/jake.obj");
	NPC1_M = Model();
	NPC1_M.LoadModel("Models/NPC1.obj");
	NPC2_M = Model();
	NPC2_M.LoadModel("Models/NPC2.obj");
	NPC3_M = Model();
	NPC3_M.LoadModel("Models/NPC3.obj");
	Pluto_M = Model();
	Pluto_M.LoadModel("Models/Pluto.obj");
	// Modelos de Arco
	Marco_M = Model();
	Marco_M.LoadModel("Models/arco.obj");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/cartel.obj");
	PuertaDerecha_M = Model();
	PuertaDerecha_M.LoadModel("Models/puerta_der.obj");
	PuertaIzquierda_M = Model();
	PuertaIzquierda_M.LoadModel("Models/puerta_izq.obj");

	// Configuración de Skybox 
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");
	skybox = Skybox(skyboxFaces);

	// Configuración de Materiales 
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// Configuración de Luces 
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f, // Linterna
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f, // Fija
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// Variables Uniform 
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;

	// Proyección 
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


	// Posición y Rotación inicial del Avatar
	float avatarPosX = -80.0f;
	float avatarPosY = -0.5f;
	float avatarPosZ = 5.0f;
	float avatarRotY = 0.0f;

	float anguloCaminata = 0.0f;
	float velocidadCaminata = 8.0f;

	// Variable para el modo de cámara
	bool camara3raPersona = false;
	bool teclaVPresionada = false;


	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();
		if (!camara3raPersona) {
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
		}
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Movimiento Manual del Globo  
		float globoMoveSpeed = 20.0f; // Velocidad de movimiento del globo

		// Mover en -Z
		if (mainWindow.getsKeys()[GLFW_KEY_T]) {
			globoPosZ -= globoMoveSpeed * deltaTime;
		}
		// Mover en Z
		if (mainWindow.getsKeys()[GLFW_KEY_G]) {
			globoPosZ += globoMoveSpeed * deltaTime;
		}
		// Mover en -X
		if (mainWindow.getsKeys()[GLFW_KEY_F]) {
			globoPosX -= globoMoveSpeed * deltaTime;
		}
		// Mover en X
		if (mainWindow.getsKeys()[GLFW_KEY_H]) {
			globoPosX += globoMoveSpeed * deltaTime;
		}
		// Mover en Y
		if (mainWindow.getsKeys()[GLFW_KEY_UP]) {
			globoPosY += globoMoveSpeed * deltaTime;
		}
		// Mover en -Y
		if (mainWindow.getsKeys()[GLFW_KEY_DOWN]) {
			globoPosY -= globoMoveSpeed * deltaTime;
		}

		// Lógica de Animaciones 

		// Lógica del Letrero Deslizante
		tiempoAcumulado += deltaTime;
		if (tiempoAcumulado >= intervaloCambio) {
			tiempoAcumulado = 0.0f;
			indiceInicio++;
			if (indiceInicio > (int)offsets.size() - letrasVisibles)
				indiceInicio = 0;
		}

		// Lógica de Puerta Izquierda (Tecla 'O')
		if (mainWindow.getsKeys()[GLFW_KEY_O]) {
			if (!teclaOPresionada) {
				abierta = !abierta;
				teclaOPresionada = true;
			}
		}
		else {
			teclaOPresionada = false;
		}
		float targetAng = abierta ? -90.0f : 0.0f;
		if (fabs(ang - targetAng) > 0.1f) {
			float step = vel * deltaTime / 10;
			if (ang < targetAng) ang = std::min(ang + step, targetAng);
			else ang = std::max(ang - step, targetAng);
		}

		// Lógica de Puerta Derecha (Tecla 'P')
		if (mainWindow.getsKeys()[GLFW_KEY_P]) {
			if (!teclaPPresionada && !puertaDerechaAnimando) {
				puertaDerechaAnimando = true;
				teclaPPresionada = true;
				if (!puertaDerechaAbierta) fase = 1;
				else fase = 3;
				progresoAnim = 0.0f;
			}
		}
		else {
			teclaPPresionada = false;
		}
		if (puertaDerechaAnimando) {
			progresoAnim += deltaTime / 20;
			float t = progresoAnim / duracionFase;
			if (fase == 1) {
				puertaDerechaPos.z = glm::min(t, 1.0f) * 0.5f;
				if (t >= 1.0f) { fase = 2; progresoAnim = 0.0f; }
			}
			else if (fase == 2) {
				puertaDerechaPos = glm::vec3(glm::min(t, 1.0f) * 4.0f, 0.0f, 1.0f);
				if (t >= 1.0f) { puertaDerechaAnimando = false; puertaDerechaAbierta = true; fase = 0; }
			}
			else if (fase == 3) {
				puertaDerechaPos = glm::vec3((1.0f - glm::min(t, 1.0f)) * 4.0f, 0.0f, 1.0f);
				if (t >= 1.0f) { fase = 4; progresoAnim = 0.0f; }
			}
			else if (fase == 4) {
				puertaDerechaPos.z = (1.0f - glm::min(t, 1.0f)) * 0.5f;
				if (t >= 1.0f) { puertaDerechaAnimando = false; puertaDerechaAbierta = false; fase = 0; }
			}
		}

		// Lógica de Juego de Pelota (Tecla 'J')
		if (mainWindow.getsKeys()[GLFW_KEY_J]) {
			if (!teclaJPresionada) {
				teclaJPresionada = true;
				pelotaDireccion = -pelotaDireccion;
				if (!jakeGolpeando) {
					jakeGolpeando = true;
					jakeAnimTimer = 0.5f;
				}
				if (!pelotaSaltando) {
					pelotaSaltando = true;
					pelotaVelocidadY = fuerzaSalto;
				}
			}
		}
		else {
			teclaJPresionada = false;
		}
		if (jakeGolpeando) {
			jakeAnimTimer -= deltaTime;
			if (jakeAnimTimer <= 0.0f) {
				jakeGolpeando = false;
				jakeAnguloGolpe = 0.0f;
			}
			else {
				if (jakeAnimTimer > 0.25f) jakeAnguloGolpe = 45.0f * ((0.5f - jakeAnimTimer) / 0.25f);
				else jakeAnguloGolpe = 45.0f * (jakeAnimTimer / 0.25f);
			}
		}
		if (pelotaSaltando) {
			pelotaVelocidadY += gravedad * deltaTime;
			pelotaPosBaseY += pelotaVelocidadY * deltaTime;
			if (pelotaPosBaseY <= 1.5f) {
				pelotaPosBaseY = 1.8f;
				pelotaSaltando = false;
				pelotaVelocidadY = 0.0f;
			}
		}

		// Lógica de Keyframes del Globo (Teclas 'L', 'K', 'G', 'C')
		if (mainWindow.getsKeys()[GLFW_KEY_L] && !teclaLPresionada) {
			teclaLPresionada = true;
			FRAME newFrame;
			newFrame.posX = globoPosX;
			newFrame.posY = globoPosY;
			newFrame.posZ = globoPosZ;
			newFrame.rotY = globoRotY;
			KeyFrameGlobo.push_back(newFrame);
			printf("Keyframe %d guardado: (%.2f, %.2f, %.2f)\n", (int)KeyFrameGlobo.size(), newFrame.posX, newFrame.posY, newFrame.posZ);
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_L]) {
			teclaLPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_K] && !teclaKPresionada) {
			teclaKPresionada = true;
			if (KeyFrameGlobo.size() > 1) {
				playGlobo = true;
				FrameIndexGlobo = 0;
				playIndexGlobo = 0.0f;
				printf("Reproduciendo animación del globo...\n");
			}
			else {
				printf("¡Error! Necesitas al menos 2 keyframes para reproducir.\n");
			}
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_K]) {
			teclaKPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_G] && !teclaGPresionada) {
			teclaGPresionada = true;
			saveKeyframes(KeyFrameGlobo, "globo_keyframes.txt");
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_G]) {
			teclaGPresionada = false;
		}
		if (mainWindow.getsKeys()[GLFW_KEY_C] && !teclaCPresionada) {
			teclaCPresionada = true;
			KeyFrameGlobo = readKeyframes("globo_keyframes.txt");
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_C]) {
			teclaCPresionada = false;
		}

		// Actualiza la animación del globo si se está reproduciendo
		animate(globoPosX, globoPosY, globoPosZ, globoRotY,
			KeyFrameGlobo, FrameIndexGlobo, playIndexGlobo, playGlobo);

		// Lógica de Movimiento y Animación del Avatar 
		if (camara3raPersona) {

			// Movimiento 
			float avatarMoveSpeed = 15.0f;
			if (mainWindow.getsKeys()[GLFW_KEY_W]) {
				avatarPosZ -= avatarMoveSpeed * deltaTime;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_S]) {
				avatarPosZ += avatarMoveSpeed * deltaTime;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_A]) {
				avatarPosX -= avatarMoveSpeed * deltaTime;
			}
			if (mainWindow.getsKeys()[GLFW_KEY_D]) {
				avatarPosX += avatarMoveSpeed * deltaTime;
			}

			// Animación 
			if (mainWindow.getsKeys()[GLFW_KEY_W] || mainWindow.getsKeys()[GLFW_KEY_S] ||
				mainWindow.getsKeys()[GLFW_KEY_A] || mainWindow.getsKeys()[GLFW_KEY_D])
			{
				anguloCaminata = glm::sin(glfwGetTime() * velocidadCaminata) * 25.0f;
			}
			else
			{
				anguloCaminata = 0.0f; // Reset de movimiento
			}
		}
		else
		{
			// Si no estamos en modo 3ra persona, forzamos la animación a 0
			anguloCaminata = 0.0f;
		}

		// Lógica Cambio de Cámara 
		if (mainWindow.getsKeys()[GLFW_KEY_V] && !teclaVPresionada) {
			teclaVPresionada = true;
			camara3raPersona = !camara3raPersona; // Invertir el modo de la camara
		}
		if (!mainWindow.getsKeys()[GLFW_KEY_V]) {
			teclaVPresionada = false;
		}


		// Renderizado 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		// Lógica Cámara en 3ra Persona 
		if (camara3raPersona) {
			glm::vec3 posDeseada = glm::vec3(avatarPosX, avatarPosY, avatarPosZ) + glm::vec3(0.0f, 8.0f, 20.0f);
			glm::vec3 target = glm::vec3(avatarPosX, avatarPosY + 8.0f, avatarPosZ);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(glm::lookAt(posDeseada, target, glm::vec3(0.0f, 1.0f, 0.0f))));
		}
		else {
			// Camara normal
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		}

		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// Configuración de Luces
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		// Renderizado de la Escena 

		// Arco con Letrero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -1.5f, -2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Marco.UseTexture();
		Marco_M.RenderModel();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Letrero_M.RenderModel();

		// Letrero animado
		glm::mat4 letraModel = glm::mat4(1.0f);
		letraModel = glm::rotate(letraModel, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 2.0f));
		AdvTime_FontTexture.UseTexture();
		for (int i = 0; i < letrasVisibles; i++) {
			int idx = (indiceInicio + i) % offsets.size();
			glm::vec2 toffset = offsets[idx];
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glm::mat4 letraModel = glm::mat4(1.0f);
			letraModel = glm::rotate(letraModel, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			float startX = basePos.x + (letrasVisibles - 1) * separacion;
			letraModel = glm::translate(letraModel, glm::vec3(startX - i * separacion, basePos.y, basePos.z));
			letraModel = glm::scale(letraModel, escalaLetras);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(letraModel));
			meshList[4]->RenderMesh();
		}

		// Puertas
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.3f, 0.0f, 4.7f));
		model = glm::rotate(model, -ang * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Puerta.UseTexture();
		PuertaIzquierda_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(puertaDerechaPos.x, puertaDerechaPos.z, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Puerta.UseTexture();
		PuertaDerecha_M.RenderModel();

		// Juego de Pelota
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -1.5f, -180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.0f, 17.0f, 17.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		Juego_Pelota_Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Juego_Pelota_M.RenderModel();

		// Jake
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(110.0f, -1.8f, -260.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(jakeAnguloGolpe), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Jake_M.RenderModel();

		// Pelota
		float pelotaBaseX = 114.5f;
		float pelotaBaseZ = -180.0f;
		pelotaOffset += pelotaVelocidad * pelotaDireccion * deltaTime;
		if (pelotaOffset > pelotaAmplitud) {
			pelotaOffset = pelotaAmplitud;
			pelotaDireccion = -1.0f;
		}
		else if (pelotaOffset < -pelotaAmplitud) {
			pelotaOffset = -pelotaAmplitud;
			pelotaDireccion = 1.0f;
		}
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(pelotaBaseX, pelotaPosBaseY, pelotaBaseZ + pelotaOffset));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
		PiedraTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Piedra_M.RenderModel();

		// Piso
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// Piramide Chichen
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.5f, -1.5f, 180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.8f, 1.8f, 1.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Juego_Pelota_Texture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PiramideT_M.RenderModel();

		// Piramide Sol
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-180.0f, -1.0f, -180.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PiramideS_M.RenderModel();

		// Globo (Animado por Keyframes)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(globoPosX, globoPosY, globoPosZ));
		model = glm::rotate(model, glm::radians(globoRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(14.0f, 14.0f, 14.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Globo_M.RenderModel();

		// Chozas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Chozas_M.RenderModel();

		// Arboles
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(210.0f, -0.5f, -210.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -165.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, -115.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -75.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -25.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, -5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, -0.5f, -0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 135.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 185.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -0.5f, 205.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, -0.5f, 225.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, 205.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 225.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-220.0f, -0.5f, 35.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-240.0f, -0.5f, 95.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, 15.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		arbol_M.RenderModel();
		

		// Banca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -120.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -120.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -150.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -240.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -0.5f, -240.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaG_M.RenderModel();


		// BancaT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BancaT_M.RenderModel();

		// Kiosco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -90.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-145.0f, -0.5f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 90.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(145.0f, -0.5f, 50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(210.0f, -0.5f, 50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kiosco_M.RenderModel();
		

		// Pillar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -50.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -110.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -110.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -170.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -170.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.5f, -230.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, -0.5f, -230.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.2f, 5.2f, 5.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pillar_M.RenderModel();

		

		//	Lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 120.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 240.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f, 4.2f, 2.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		lampara_M.RenderModel();


		// Ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Ring_M.RenderModel();

		// PisoRing
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PisoRing_M.RenderModel();

		// Cuerdas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cuerdas_M.RenderModel();

		// Coronas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 12.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Coronas_M.RenderModel();

		// EstatuaLuchador
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(32.0f, -0.5f, 32.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EstatuaLuchador_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-32.0f, -0.5f, 32.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		EstatuaLuchador_M.RenderModel();

		// Calendario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -0.5f, 180.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CalendarioA_M.RenderModel();

		// PERSONAJES CON JERARQUÍA


		/*	//									//
					MICKEY MOUSE
		*/	//									//

		// Matriz Padre 
		// Posición general del avatar 
		glm::mat4 modelAvatar = glm::mat4(1.0);
		modelAvatar = glm::translate(modelAvatar, glm::vec3(avatarPosX, avatarPosY, avatarPosZ));
		modelAvatar = glm::rotate(modelAvatar, glm::radians(avatarRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAvatar = glm::scale(modelAvatar, glm::vec3(0.05f, 0.05f, 0.05f));

		// Torso (Padre) 
		glm::mat4 modelTorso = modelAvatar;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTorso));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyT_M.RenderModel();

		// Cabeza (Hijo del Torso) 
		glm::mat4 modelCabeza = modelTorso;
		modelCabeza = glm::translate(modelCabeza, glm::vec3(0.0f, -2.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabeza));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyC_M.RenderModel(); // Dibuja la Cabeza

		// Brazo Derecho (Hijo del Torso) 
		glm::mat4 modelBrazoD = modelTorso;
		modelBrazoD = glm::rotate(modelBrazoD, glm::radians(anguloCaminata), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazoD = glm::translate(modelBrazoD, glm::vec3(-1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoD));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyBD_M.RenderModel();

		// Brazo Izquierdo (Hijo del Torso) 
		glm::mat4 modelBrazoI = modelTorso;
		modelBrazoI = glm::rotate(modelBrazoI, glm::radians(-anguloCaminata), glm::vec3(1.0f, 0.0f, 0.0f));
		modelBrazoI = glm::translate(modelBrazoI, glm::vec3(1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBrazoI));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyBI_M.RenderModel();

		// --- 5. Pierna Derecha (Hijo del Torso) ---
		glm::mat4 modelPiernaD = modelTorso;
		modelPiernaD = glm::rotate(modelPiernaD, glm::radians(-anguloCaminata), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPiernaD = glm::translate(modelPiernaD, glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaD));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyPD_M.RenderModel();

		// --- 6. Pierna Izquierda (Hijo del Torso) ---
		glm::mat4 modelPiernaI = modelTorso;
		modelPiernaI = glm::rotate(modelPiernaI, glm::radians(anguloCaminata), glm::vec3(1.0f, 0.0f, 0.0f));
		modelPiernaI = glm::translate(modelPiernaI, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPiernaI));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MickeyPI_M.RenderModel();


		// Peach
		glm::mat4 baseModelPeach = glm::mat4(1.0);
		baseModelPeach = glm::translate(baseModelPeach, glm::vec3(-5.0f, -0.5f, -5.0f));
		baseModelPeach = glm::rotate(baseModelPeach, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		baseModelPeach = glm::scale(baseModelPeach, glm::vec3(0.2f, 0.2f, 0.2f));

		glm::mat4 modelCPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachC_M.RenderModel();

		glm::mat4 modelTPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachT_M.RenderModel();

		glm::mat4 modelBDPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBDPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachBD_M.RenderModel();

		glm::mat4 modelBIPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBIPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachBI_M.RenderModel();

		glm::mat4 modelPDPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPDPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachPD_M.RenderModel();

		glm::mat4 modelPIPeach = baseModelPeach;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPIPeach));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PeachPI_M.RenderModel();

		// Finn
		glm::mat4 baseModelFinn = glm::mat4(1.0);
		baseModelFinn = glm::translate(baseModelFinn, glm::vec3(0.0f, -0.5f, -15.0f));
		baseModelFinn = glm::rotate(baseModelFinn, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		baseModelFinn = glm::scale(baseModelFinn, glm::vec3(2.0f, 2.0f, 2.0f));

		glm::mat4 modelCFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnC_M.RenderModel();

		glm::mat4 modelTFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelTFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnT_M.RenderModel();

		glm::mat4 modelBDFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBDFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnBD_M.RenderModel();

		glm::mat4 modelBIFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBIFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnBI_M.RenderModel();

		glm::mat4 modelPDFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPDFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnPD_M.RenderModel();

		glm::mat4 modelPIFinn = baseModelFinn;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPIFinn));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		FinnPI_M.RenderModel();

		//	NPC1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, 130.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -0.5f, -40.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -0.5f, -40.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC1_M.RenderModel();

		//	NPC2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-205.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, 22.0f, -80.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(175.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC2_M.RenderModel();

		//	NPC3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-200.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-185.0f, -0.5f, 140.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(195.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, -0.5f, 60.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-180.0f, 22.0f, -80.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.2f, 10.2f, 10.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		NPC3_M.RenderModel();

		//Pluto 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(60.0f, -0.5f, -70.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pluto_M.RenderModel();



		// --- 5.5. Finalización del Frame ---
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}