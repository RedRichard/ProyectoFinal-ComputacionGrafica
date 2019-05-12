#include "PointLight.h"



PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, 
						GLfloat aIntensity, GLfloat dIntensity, 
						GLfloat xPos, GLfloat yPos, GLfloat zPos, 
						GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation, 
							GLfloat diffuseIntensityLocation, GLfloat positionLocation, 
							GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation)
{
	glUniform3f(ambientcolorLocation, color.x, color.y, color.z);
	if (estadoLuz == true) {
		glUniform1f(ambientIntensityLocation, ambientIntensity);
		glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	}
	else {
		glUniform1f(ambientIntensityLocation, 0.0f);
		glUniform1f(diffuseIntensityLocation, 0.0f);
	}

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);
}

void PointLight::PrenderLuz() {
	estadoLuz = true;
}

void PointLight::ApagarLuz() {
	estadoLuz = false;
}

PointLight::~PointLight()
{
}
