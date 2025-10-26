#pragma once
#include <stdio.h>
#include <glew.h>
#include <glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();


	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLfloat getmuevex() { return muevex; }
	GLfloat getmuevez() { return muevez; }
	GLboolean getprendida() { return luzprendida; }          
	GLfloat getlucespuntuales() { return lucespuntuales; }   
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	GLfloat getarticulacionCofre() { return articulacionCofre; }
	int getActivar_tecla() { return Activar_tecla; }

	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();

	// Variables de movimiento y cámara
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLfloat muevex;
	GLfloat muevez;

	// Control de luces
	GLfloat lucespuntuales;
	GLboolean luzprendida;

	// NUEVAS variables para el cofre
	GLfloat articulacionCofre;
	int Activar_tecla;

	bool mouseFirstMoved;

	// Callbacks
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};
