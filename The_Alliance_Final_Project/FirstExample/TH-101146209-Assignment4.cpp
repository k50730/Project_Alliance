using namespace std;

#include <iostream>
#include "stdlib.h"
#include "iostream"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "SOIL.h"
#include "GeometryGenerator.h"



enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };



GLuint numOfDivisions = 1;

float increment; //increment for every square
int vertex_cnt = 0; //index for vertex
int index_cnt = 0; //index for index array 
int uv_cnt = 0; //index for uv map

						   //MVP mat
GLuint MatrixID;
GLuint LightPosID;
GLuint ViewID;
GLuint ModelID;

glm::vec3 LightPos;

glm::mat4 MVP;
glm::mat4 ViewMatrix;
glm::mat4 Projection;

glm::vec3 eye;

GLint width, height;
GLuint textureID = 0;
unsigned char *image = SOIL_load_image("mario.png", &width, &height, 0, SOIL_LOAD_RGB);

Primitives* cube;

GLuint gVAO;
GLuint program;


void init(void)
{
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	program = LoadShaders(shaders);

	glUseProgram(program);

	//cout << "Enter the number of divisions on each side: ";
	//cin >> numOfDivisions;
	//numOfDivisions = 1;

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");
	LightPosID = glGetUniformLocation(program, "lightPos");
	ViewID = glGetUniformLocation(program, "View");
	ModelID = glGetUniformLocation(program, "Model");

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	eye = glm::vec3(0.0f, 0.0f, 3.0f);
	

	// Camera matrix
	ViewMatrix = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//init LightPosition
	LightPos = glm::vec3(0.f, 0.f, 6.f);

	cube = new Primitives();
	//cuboid = new Primitives();
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);

	/*/
#pragma region Assignment Demo
	
	
	GLfloat* plane_vertice = new GLfloat[(numOfDivisions + 1) * (numOfDivisions + 1) * 3];
	GLfloat* texture_UV = new GLfloat[(numOfDivisions + 1) * (numOfDivisions + 1) * 2];
	GLfloat* plane_normals = new GLfloat[(numOfDivisions + 1) * (numOfDivisions + 1) * 3];

	increment = 1.0f / (float)numOfDivisions; // 1 divides by the number of divisions

	for (int i = 0; i < numOfDivisions + 1; i++) {
		for (int j = 0; j < numOfDivisions + 1; j++) {
			// Vertice 
			plane_vertice[vertex_cnt] = -0.5f + (increment * j); // x
			plane_vertice[vertex_cnt + 1] = 0.5f - (increment * i); // y
			plane_vertice[vertex_cnt + 2] = 0.f;
			//Normals
			plane_normals[vertex_cnt] = 0.f; // x
			plane_normals[vertex_cnt + 1] = 0.f; // y 
			plane_normals[vertex_cnt + 2] = 1.f; // z

			//UV vertices
			texture_UV[uv_cnt] = (increment * j);
			texture_UV[uv_cnt + 1] = (increment * i);

			vertex_cnt += 3;
			uv_cnt += 2;
		}
	} 

	GLushort* plane_indice = new GLushort[numOfDivisions * numOfDivisions * 6];

	  //Index array
	for (int i = 0; i < numOfDivisions; i++) {
		for (int j = 0; j < numOfDivisions; j++) {
			//first triangle
			plane_indice[index_cnt] = j + ((numOfDivisions + 1) * i);
			plane_indice[index_cnt + 1] = j + (numOfDivisions + 1) * (i + 1);
			plane_indice[index_cnt + 2] = (j + 1) + ((numOfDivisions + 1) * i);
			//second triangle
			plane_indice[index_cnt + 3] = j + (numOfDivisions + 1) * (i + 1);
			plane_indice[index_cnt + 4] = (j + 1) + ((numOfDivisions + 1) * (i + 1));
			plane_indice[index_cnt + 5] = (j + 1) + ((numOfDivisions + 1) * i);

			//increment for index
			index_cnt += 6;
		}
	}

	
	GLuint plane_VBO, plane_texture_vbo, plane_IBO, plane_normal_vbo;


	//sending lightPos
	glUniform3fv(LightPosID, 1, &LightPos[0]);
	//generate and bind vao
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	//Vertice VBO
	glGenBuffers(1, &plane_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, plane_VBO);
	glBufferData(GL_ARRAY_BUFFER, ((numOfDivisions + 1) * (numOfDivisions + 1) * 3) * sizeof(GLfloat), plane_vertice, GL_STATIC_DRAW);
	// Vertice Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	//Texture binding
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	//Texture VBO
	glGenBuffers(1, &plane_texture_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane_texture_vbo);
	glBufferData(GL_ARRAY_BUFFER, (numOfDivisions + 1) * (numOfDivisions + 1) * 2 * sizeof(GLfloat), texture_UV, GL_STATIC_DRAW);
	// Texture Pointer
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	//Normal VBO
	glGenBuffers(1, &plane_normal_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, plane_normal_vbo);
	glBufferData(GL_ARRAY_BUFFER, (numOfDivisions + 1) * (numOfDivisions + 1) * 3 * sizeof(GLfloat), plane_normals, GL_STATIC_DRAW);
	// Normal Pointer
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	//IBO
	glGenBuffers(1, &plane_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (numOfDivisions * numOfDivisions * 6) * sizeof(GLushort), plane_indice, GL_STATIC_DRAW);
	
#pragma endregion
	//*/
}

void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	ViewMatrix = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);
	MVP = Projection * ViewMatrix * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
}

void transformObject(Transform transform)
{
	transformObject(transform.mScale, transform.mRotationAxis, transform.mRotation, transform.mPosition);
}

void display(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.f, 1.f, 1.f, 1.f);

	//glBindVertexArray(gVAO);
	//glDrawElements(GL_TRIANGLES, (numOfDivisions * numOfDivisions * 6), GL_UNSIGNED_SHORT, 0);


	glUniform3fv(LightPosID, 1, &LightPos[0]);
	//glBindVertexArray(*cube->VAO[Type::Cube]);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	
	//transformObject(cube->transform);
	//glDrawElements(GL_TRIANGLES, cube->mNumOfIndex[Type::Cube], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*cube->VAO[Type::Bar]);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	transformObject(cube->transform);
	glDrawElements(GL_TRIANGLES, cube->mNumOfIndex[Type::Bar], GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
	
}

void Timer(int id) {
	glutPostRedisplay();
	glutTimerFunc(33, Timer, 0);
}

void KeyDown(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		eye -= glm::vec3(0.f, 0.f, 0.3f);
		break;
	case 's':
		eye += glm::vec3(0.f, 0.f, 0.3f);
		break;
	case 'a':
		eye -= glm::vec3(0.3f, 0.f, 0.f);
		break;
	case 'd':
		eye += glm::vec3(0.3f, 0.f, 0.f);
		break;
	case 'r':
		eye += glm::vec3(0.f, 0.3f, 0.f);
		break;
	case 'f':
		eye -= glm::vec3(0.f, 0.3f, 0.f);
		break;
	case 'i':
		LightPos += glm::vec3(0.f, 0.3f, 0.f);
		break;
	case 'k':
		LightPos -= glm::vec3(0.f, 0.3f, 0.f);
		break;
	case 'j':
		LightPos -= glm::vec3(0.3f, 0.f, 0.f);
		break;
	case 'l':
		LightPos += glm::vec3(0.3f, 0.f, 0.f);
		break;
	}
}

void idle()
{
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(960, 720);
	glutCreateWindow("HsiaoTe Tang, 101131908");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	//glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_DEPTH_TEST);

	init();

	glutKeyboardFunc(KeyDown);

	glutDisplayFunc(display);

	glutTimerFunc(33, Timer, 0);

	glutIdleFunc(idle);

	glutMainLoop();

}
