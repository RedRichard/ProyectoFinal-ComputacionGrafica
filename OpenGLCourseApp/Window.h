#pragma once

#include "stdio.h"

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat mueveSillas() { return movSillas; }
	GLfloat mueveTazas() { return movTazas; }
	bool getEstadoLucesPuntuales() { return estadoLucesPuntuales; };
	bool getEstadoLucesDir() { return estadoLucesDir; };
	int getEstadoCamara() { return estadoCamara; };
	bool getJustChangedCamara(){ return justChangedCamara; };
	void doneJustChangedCamera() { justChangedCamara = false; };
	bool* getsKeys() { return keys; }
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~Window();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//Teclas del teclado
	bool keys[1024];

	GLfloat lastX, lastY, xChange, yChange;
	bool mouseFirstMoved, estadoLucesPuntuales = true, estadoLucesDir = true, movTazas = false, movSillas = false, justChangedCamara = false;

	int estadoCamara = 0;

	void CreateCallBacks();
	static void handleKeys(GLFWwindow *window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow *window, double xPos, double yPos);
};