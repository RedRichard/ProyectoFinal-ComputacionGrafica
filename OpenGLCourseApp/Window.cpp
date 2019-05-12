#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		printf("Error Initialising GLFW");
		glfwTerminate();
		return 1;
	}

	//Propiedades de la ventana
	//Versión de OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//No compatibilidad hacia atras
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Compatibilidad hacia adelante
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
	if (!mainWindow)
	{
		printf("Error creating GLFW window!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set the current context
	glfwMakeContextCurrent(mainWindow);

	//Encargarse de Teclado y ratón
	CreateCallBacks();

	//Desparece la flecha del ratón
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow modern extension access
	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Create Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);

	//Esta clase es el usuario de esta ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::CreateCallBacks()
{
	//Cuando una tecla es presionada en mainwindow llama a handle keys
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::handleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
	//Es estático, ahora sabe a que ventana acceder
	Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//JUEGOS
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
		if (theWindow->movTazas == false)
			theWindow->movTazas = true;
		else
			theWindow->movTazas = false;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
		if (theWindow->movSillas == false)
			theWindow->movSillas = true;
		else
			theWindow->movSillas = false;
	}

	//LUCES
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		if (theWindow->estadoLucesPuntuales == false)
			theWindow->estadoLucesPuntuales = true;
		else
			theWindow->estadoLucesPuntuales = false;
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		if (theWindow->estadoLucesDir == false)
			theWindow->estadoLucesDir = true;
		else
			theWindow->estadoLucesDir = false;
	}

	//CAMARAS
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 0;
	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 1;
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 2;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 3;
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 4;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		theWindow->justChangedCamara = true;
		theWindow->estadoCamara = 5;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}

}

void Window::handleMouse(GLFWwindow *window, double xPos, double yPos)
{
	//Es estático, ahora sabe a que ventana acceder
	Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//Encargarse del primer movimiento
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;

}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
