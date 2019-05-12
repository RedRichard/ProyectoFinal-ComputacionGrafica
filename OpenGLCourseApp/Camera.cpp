#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed*15;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::checkTriggers() {
	if (position.x > 19.0f && position.x < 55.0f && position.y > 0.0f && position.y < 20.0f && position.z < -55.0f && position.z > -123.0f) {
		triggerSillas = true;
	}
	else
	{
		triggerSillas = false;
	}
	if (position.x < 15.0f && position.x > -60.0f && position.y > 0.0f && position.y < 20.0f && position.z < -55.0f && position.z > -123.0f) {
		triggerTazas = true;
	}
	else
	{
		triggerTazas = false;
	}
	if (position.x < 5.0f && position.x > -15.0f && position.y > 0.0f && position.y < 20.0f && position.z < 24.0f && position.z > -67.0f) {
		triggerLucesPoint = true;
	}
	else
	{
		triggerLucesPoint = false;
	}
	if (position.x < 65.0f && position.x > -65.0f && position.y > 0.0f && position.y < 20.0f && position.z < -68.0f && position.z > -100.0f) {
		triggerLucesDir = true;
	}
	else
	{
		triggerLucesDir = false;
	}
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	//printf("\nx: %f, y: %f, z: %f", position.x, position.y, position.z);
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
	//printf("yaw: %f, pitch: %f\n", yaw, pitch);
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
