#pragma once

#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>



struct Transform {
	glm::vec3 mPosition;
	float	  mRotation;
	glm::vec3 mRotationAxis;
	glm::vec3 mScale;

	Transform(); // no transform apply
	Transform(glm::vec3 position, float rotation, glm::vec3 rotationAxis, glm::vec3 scale); // transform apply
};

enum Type
{
	Cube,
	Cuboid,
	Pyramid,
	ChopPyramid,
	Cone,
	ChopCone,
	Cylinder,
	Wedge,
	Triangle,
	Hexagon,
	Bar,
};
class Primitives
{

public:
	GLuint** VAO;

public:
	Primitives();
	GLuint* Cube();
	GLuint* Cuboid();
	GLuint* Pyramid();
	GLuint* ChopPyramid();
	GLuint* Cone();
	GLuint* ChopCone();
	GLuint* Cylinder();
	GLuint* Wedge();
	GLuint* Triangle();
	GLuint* Hexagon();
	GLuint* Bar();

	int mNumOfIndex[8];
	Transform transform;
	

};

