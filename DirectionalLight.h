#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, 
					GLfloat aIntensity, GLfloat dIntensity,
					GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);


	// new: permitir ajustar y leer la intensidad ambiental en tiempo de ejecucion
	void SetAmbientIntensity(GLfloat aIntensity);
	GLfloat GetAmbientIntensity();
	~DirectionalLight();

private:
	glm::vec3 direction;
};

