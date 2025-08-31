//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
// shaders nuevos: colores 
static const char* fRed = "shaders/red.frag";
static const char* fGreen = "shaders/green.frag";
static const char* fBlue = "shaders/blue.frag";
static const char* fBrown = "shaders/brown.frag";
static const char* fDarkGreen = "shaders/darkgreen.frag";

//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = { 
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3
		
	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.5f, -0.5f,  0.5f,
	0.5f, -0.5f,  0.5f,
	0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	// back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
			-0.95f, 0.86f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.86f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,
			-0.95f, 0.86f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,   -0.95f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,
			-0.60f, 0.72f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.72f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,
			-0.60f, 0.72f, 0.5f,  1.0f,0.0f,0.0f,   -0.55f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,   -0.60f, 0.90f, 0.5f,  1.0f,0.0f,0.0f,
			-0.85f, 0.70f, 0.5f,  1.0f,0.0f,0.0f,   -0.60f, 0.70f, 0.5f,  1.0f,0.0f,0.0f,   -0.60f, 0.74f, 0.5f,  1.0f,0.0f,0.0f,
			-0.85f, 0.70f, 0.5f,  1.0f,0.0f,0.0f,   -0.60f, 0.74f, 0.5f,  1.0f,0.0f,0.0f,   -0.85f, 0.74f, 0.5f,  1.0f,0.0f,0.0f,

			//S (verde: 0,1,0)
			-0.45f, 0.86f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.86f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.86f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,   -0.45f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.41f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.41f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.41f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,   -0.45f, 0.90f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.78f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,   -0.45f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,
			-0.09f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,
			-0.09f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,   -0.09f, 0.82f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.74f, 0.5f,  0.0f,1.0f,0.0f,
			-0.45f, 0.70f, 0.5f,  0.0f,1.0f,0.0f,   -0.05f, 0.74f, 0.5f,  0.0f,1.0f,0.0f,   -0.45f, 0.74f, 0.5f,  0.0f,1.0f,0.0f,

			//M(azul: 0,0,1)
			0.05f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.09f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.09f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,
			0.05f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.09f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.05f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,
			0.41f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.45f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.45f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,
			0.41f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.45f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.41f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,
			0.09f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.13f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.27f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,
			0.09f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.27f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.23f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,
			0.37f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.41f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.27f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,
			0.37f, 0.90f, 0.5f,   0.0f,0.0f,1.0f,    0.27f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,    0.23f, 0.70f, 0.5f,   0.0f,0.0f,1.0f,	
	};
	MeshColor* letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras, 482);
	meshColorList.push_back(letras);

	// ---------- TRIÁNGULOS ----------
	// Base: triangulorojo (mismos vértices para todos los triángulos)
	GLfloat vertices_triangulorojo[] = {
		//   X      Y     Z      R   G   B
		-1.0f, -1.0f, 0.5f,    1.0f,0.0f,0.0f,
		 1.0f, -1.0f, 0.5f,    1.0f,0.0f,0.0f,
		 0.0f,  1.0f, 0.5f,    1.0f,0.0f,0.0f,
	};
	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	// Triángulo azul (copiando vértices, cambiando color)
	GLfloat vertices_trianguloazul[] = {
		-1.0f, -1.0f, 0.5f,    0.0f,0.0f,1.0f,
		 1.0f, -1.0f, 0.5f,    0.0f,0.0f,1.0f,
		 0.0f,  1.0f, 0.5f,    0.0f,0.0f,1.0f,
	};
	MeshColor* trianguloazul = new MeshColor();
	trianguloazul->CreateMeshColor(vertices_trianguloazul, 18);
	meshColorList.push_back(trianguloazul);

	// Triángulo verde (0, 0.5, 0)
	GLfloat vertices_trianguloverde[] = {
		-1.0f, -1.0f, 0.5f,    0.0f,0.5f,0.0f,
		 1.0f, -1.0f, 0.5f,    0.0f,0.5f,0.0f,
		 0.0f,  1.0f, 0.5f,    0.0f,0.5f,0.0f,
	};
	MeshColor* trianguloverde = new MeshColor();
	trianguloverde->CreateMeshColor(vertices_trianguloverde, 18);
	meshColorList.push_back(trianguloverde);

	// ---------- CUADRADOS (dos triángulos = 6 vértices) ----------
	// Cuadrado verde (base)
	GLfloat vertices_cuadradoverde[] = {
		-0.5f,-0.5f,0.5f,   0.0f,1.0f,0.0f,
		 0.5f,-0.5f,0.5f,   0.0f,1.0f,0.0f,
		 0.5f, 0.5f,0.5f,   0.0f,1.0f,0.0f,
		-0.5f,-0.5f,0.5f,   0.0f,1.0f,0.0f,
		 0.5f, 0.5f,0.5f,   0.0f,1.0f,0.0f,
		-0.5f, 0.5f,0.5f,   0.0f,1.0f,0.0f,
	};
	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);

	// Cuadrado rojo
	GLfloat vertices_cuadradorojo[] = {
		-0.5f,-0.5f,0.5f,   1.0f,0.0f,0.0f,
		 0.5f,-0.5f,0.5f,   1.0f,0.0f,0.0f,
		 0.5f, 0.5f,0.5f,   1.0f,0.0f,0.0f,
		-0.5f,-0.5f,0.5f,   1.0f,0.0f,0.0f,
		 0.5f, 0.5f,0.5f,   1.0f,0.0f,0.0f,
		-0.5f, 0.5f,0.5f,   1.0f,0.0f,0.0f,
	};
	MeshColor* cuadradorojo = new MeshColor();
	cuadradorojo->CreateMeshColor(vertices_cuadradorojo, 36);
	meshColorList.push_back(cuadradorojo);

	// Cuadrado café (0.478, 0.255, 0.067)
	GLfloat vertices_cuadradocafe[] = {
		-0.5f,-0.5f,0.5f,   0.478f,0.255f,0.067f,
		 0.5f,-0.5f,0.5f,   0.478f,0.255f,0.067f,
		 0.5f, 0.5f,0.5f,   0.478f,0.255f,0.067f,
		-0.5f,-0.5f,0.5f,   0.478f,0.255f,0.067f,
		 0.5f, 0.5f,0.5f,   0.478f,0.255f,0.067f,
		-0.5f, 0.5f,0.5f,   0.478f,0.255f,0.067f,
	};
	MeshColor* cuadradocafe = new MeshColor();
	cuadradocafe->CreateMeshColor(vertices_cuadradocafe, 36);
	meshColorList.push_back(cuadradocafe);
}

void CreateShaders()
{

	Shader *shader1 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader *shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//Sahders nuevos 

	Shader* sRed = new Shader();
	sRed->CreateFromFiles(vShader, fRed);
	shaderList.push_back(*sRed);

	Shader* sGreen = new Shader();
	sGreen->CreateFromFiles(vShader, fGreen);
	shaderList.push_back(*sGreen);

	Shader* sBlue = new Shader();
	sBlue->CreateFromFiles(vShader, fBlue);
	shaderList.push_back(*sBlue);

	Shader* sBrown = new Shader();
	sBrown->CreateFromFiles(vShader, fBrown);
	shaderList.push_back(*sBrown);

	Shader* sDarkGreen = new Shader();
	sDarkGreen->CreateFromFiles(vShader, fDarkGreen);
	shaderList.push_back(*sDarkGreen);

}



int main()
{ 
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	//glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		glfwPollEvents();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//LETRAS J S M
		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.90f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		*/
		//CASA CON CUBOS Y PIRÁMIDES 

		//Cuerpo de la casa (cubo rojo)
		shaderList[2].useShader(); // rojo
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.35f, -4.00f));
		model = glm::scale(model, glm::vec3(0.80f, 0.95f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // cubo

		// Techo (pirámide azul)
		shaderList[4].useShader(); // azul
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.42f, -3.98f));
		model = glm::scale(model, glm::vec3(0.95f, 0.50f, 1.00f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh(); // pirámide

		//Puerta (cubo verde)
		shaderList[3].useShader(); // verde
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.68f, -3.96f));
		model = glm::scale(model, glm::vec3(0.18f, 0.30f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh(); // cubo

		//Ventana izquierda (cubo verde)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.22f, -0.18f, -3.96f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Ventana derecha (cubo verde)
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.22f, -0.18f, -3.96f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Árbol izquierdo

		// Tronco (cubo café)
		shaderList[5].useShader(); // café
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.72f, -3.97f));
		model = glm::scale(model, glm::vec3(0.12f, 0.28f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		// Copa(pirámide verde oscuro)
		shaderList[6].useShader(); // verde oscuro
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, -0.18f, -4.00f));
		model = glm::scale(model, glm::vec3(0.30f, 0.45f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Árbol derecho

		shaderList[5].useShader(); // tronco café
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.72f, -3.97f));
		model = glm::scale(model, glm::vec3(0.12f, 0.28f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		shaderList[6].useShader(); // copa verde oscuro
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.18f, -4.00f));
		model = glm::scale(model, glm::vec3(0.30f, 0.45f, 1.00f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
};

// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/