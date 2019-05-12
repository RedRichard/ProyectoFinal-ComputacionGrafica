#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	void setCameraPosition(glm::vec3 newPos) { position = newPos; }
	void setCameraFront(glm::vec3 newFront) { front = newFront; }

	bool getTriggerTazas() { return triggerTazas; };
	bool getTriggerSillas() { return triggerSillas; };
	bool getTriggerLucesPoint() { return triggerLucesPoint;	};
	bool getTriggerLucesDir() { return triggerLucesDir; };

	void checkTriggers();

	glm::vec3 getCameraPosition();
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();

	GLfloat yaw;
	GLfloat pitch;

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	bool triggerTazas = false, triggerSillas = false, triggerLucesDir = false, triggerLucesPoint = false;

	void update();
};

