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
	Triangle,
	Bar,
	HexagonCone,
	Cone,
	Cylinder,
	Octagon,
	OctagonCone
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
	GLuint* Triangle();
	GLuint* Bar();
	GLuint* HexagonCone();
	GLuint* Cone();
	GLuint* Cylinder();
	GLuint* Octagon();
	GLuint* OctagonCone();


	GLfloat* Shape(int numVert);
	GLuint* Cone(int numVert);
	GLuint* Cylinder(int numVert);

	int mNumOfIndex[10];
	Transform transform;
	

};

