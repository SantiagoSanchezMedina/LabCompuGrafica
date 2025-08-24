#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <time.h>
#include <stdlib.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;
GLuint VAO, VBO, shader;
float rojo, verde, azul;
time_t ultimoCambio;  

//LENGUAJE DE SHADER (SOMBRAS) GLSL
//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.5f,1.0f,0.7f,1.0f);	 			\n\
}";

void CrearTriangulo()
{
	GLfloat vertices[] = {

		// ===== LETRA J =====
	-0.9f,  0.8f, 0.0f,
	-0.5f,  0.8f, 0.0f,
	-0.5f,  0.7f, 0.0f,
	-0.9f,  0.8f, 0.0f,
	-0.5f,  0.7f, 0.0f,
	-0.9f,  0.7f, 0.0f,
	-0.55f,  0.8f, 0.0f,
	-0.45f,  0.8f, 0.0f,
	-0.45f, -0.2f, 0.0f,
	-0.55f,  0.8f, 0.0f,
	-0.45f, -0.2f, 0.0f,
	-0.55f, -0.2f, 0.0f,
	-0.9f, -0.3f, 0.0f,
	-0.5f, -0.3f, 0.0f,
	-0.5f, -0.2f, 0.0f,
	-0.9f, -0.3f, 0.0f,
	-0.5f, -0.2f, 0.0f,
	-0.9f, -0.2f, 0.0f,

	// ===== LETRA S =====
	-0.2f,  0.8f, 0.0f,
	 0.2f,  0.8f, 0.0f,
	 0.2f,  0.7f, 0.0f,
	-0.2f,  0.8f, 0.0f,
	 0.2f,  0.7f, 0.0f,
	-0.2f,  0.7f, 0.0f,
	-0.2f,  0.8f, 0.0f,
	-0.1f,  0.8f, 0.0f,
	-0.1f,  0.2f, 0.0f,
	-0.2f,  0.8f, 0.0f,
	-0.1f,  0.2f, 0.0f,
	-0.2f,  0.2f, 0.0f,
	-0.2f,  0.2f, 0.0f,
	 0.2f,  0.2f, 0.0f,
	 0.2f,  0.1f, 0.0f,
	-0.2f,  0.2f, 0.0f,
	 0.2f,  0.1f, 0.0f,
	-0.2f,  0.1f, 0.0f,
	 0.1f,  0.2f, 0.0f,
	 0.2f,  0.2f, 0.0f,
	 0.2f, -0.3f, 0.0f,
	 0.1f,  0.2f, 0.0f,
	 0.2f, -0.3f, 0.0f,
	 0.1f, -0.3f, 0.0f,
	 -0.2f, -0.3f, 0.0f,
	  0.2f, -0.3f, 0.0f,
	  0.2f, -0.2f, 0.0f,
	 -0.2f, -0.3f, 0.0f,
	  0.2f, -0.2f, 0.0f,
	 -0.2f, -0.2f, 0.0f,

	 // ===== LETRA M =====
	  0.5f, -0.3f, 0.0f,
	  0.6f, -0.3f, 0.0f,
	  0.6f,  0.8f, 0.0f,
	  0.5f, -0.3f, 0.0f,
	  0.6f,  0.8f, 0.0f,
	  0.5f,  0.8f, 0.0f,
	   0.8f, -0.3f, 0.0f,
	   0.9f, -0.3f, 0.0f,
	   0.9f,  0.8f, 0.0f,
	   0.8f, -0.3f, 0.0f,
	   0.9f,  0.8f, 0.0f,
	   0.8f,  0.8f, 0.0f,
	   0.6f,  0.8f, 0.0f,
	   0.7f,  0.5f, 0.0f,
	   0.7f,  0.8f, 0.0f,
	   0.8f,  0.8f, 0.0f,
	   0.7f,  0.5f, 0.0f,
	   0.7f,  0.8f, 0.0f

	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);
	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);
}

void CompileShaders() {
	shader = glCreateProgram();
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
}

//funci�n para cambiar color aleatorio cada segundo
void CambiarColorRandom() {
	time_t ahora;
	time(&ahora);

	if (difftime(ahora, ultimoCambio) >= 1.0) {
		rojo = (float)rand() / RAND_MAX;
		verde = (float)rand() / RAND_MAX;
		azul = (float)rand() / RAND_MAX;
		ultimoCambio = ahora;
	}
}

int main()
{
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}

	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, BufferWidth, BufferHeight);

	CrearTriangulo();
	CompileShaders();

	//Inicializaci�n de color aleatorio
	srand(time(NULL));  
	rand();              
	rojo = (float)rand() / RAND_MAX;
	verde = (float)rand() / RAND_MAX;
	azul = (float)rand() / RAND_MAX;
	time(&ultimoCambio); 

	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();

	//color random cada segundo
		CambiarColorRandom();

		glClearColor(rojo, verde, azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 72);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
