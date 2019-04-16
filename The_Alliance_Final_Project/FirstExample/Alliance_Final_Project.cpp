
/****************************************************************************
// Alliance_Final_Project.cpp by Ding Zhang and HsiaoTe Tang (C) 2019 All Rights Reserved.
//
// Final project submission.
//
// Description:
// 
// Click run to see the result
// WASD to move the camera.
// Mouse to rotate the camera.
// 
// 
//
// See the result.
//
//*****************************************************************************/



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

#define PI 3.14159265

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

enum Textures {
	Wall1,
	Wall2,
	Wall3,
	Door,
	Window,
	Roof,
	Count
};




GLuint MatrixID;
GLuint LightPosID;
GLuint LightPosID2;
GLuint LightPosID3;
GLuint ViewID;
GLuint ModelID;

glm::vec3 LightPos;
glm::vec3 LightPos2;
glm::vec3 LightPos3;

glm::mat4 MVP;
glm::mat4 ViewMatrix;
glm::mat4 Projection;

glm::vec3 eye;
glm::vec3 center;
glm::vec3 centerDir;
glm::vec3 camDir;

float cAngle;
float vAngle;

POINT p;
int px = 0;
int py = 0;
void MouseMotion(int x, int y);

GLuint* textureID;
unsigned char **images;
const char* sources[6];
const char* source1 = "Wall1.jpg";
const char* source2 = "Wall2.jpg";
const char* source3 = "Wall3.jpg";
const char* source4 = "door.jpg";
const char* source5 = "window.jpg";
const char* source6 = "roof.jpg";


Primitives** walls;
Primitives** columns;
Primitives** ChopCones;
Primitives** topColumns;
Primitives** Cones;
Primitives** SmallBrick;
Primitives* BaseBlock;
Primitives* Core;
Primitives* CoreRoof;
Primitives* FlagPole;
Primitives* Flag;
Primitives* door;
Primitives** Windows;
Primitives** SmallWindows;
Primitives* RoofTop;

GLuint program;


void init(void)
{
	sources[0] = source1;
	sources[1] = source2;
	sources[2] = source3;
	sources[3] = source4;
	sources[4] = source5;
	sources[5] = source6;


	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};

	//Loading and compiling shaders
	program = LoadShaders(shaders);

	glUseProgram(program);


	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");
	LightPosID = glGetUniformLocation(program, "lightPos[0]");
	LightPosID2 = glGetUniformLocation(program, "lightPos[1]");
	LightPosID3 = glGetUniformLocation(program, "lightPos[2]");
	ViewID = glGetUniformLocation(program, "View");
	ModelID = glGetUniformLocation(program, "Model");

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	eye = glm::vec3(0.0f, 20.0f, 30.0f);
	cAngle = 90.f;
	vAngle = 0.f;
	centerDir = glm::vec3(0.f, 0.f, -5.f);
	center = eye + centerDir;
	camDir = glm::vec3(0.f, 0.f, 0.f);

	// Camera matrix
	ViewMatrix = glm::lookAt(eye, center, glm::vec3(0, 1, 0));
	//init LightPosition
	LightPos = glm::vec3(0.f, 25.f, 20.f);
	LightPos2 = glm::vec3(0.f, 0.f, 0.f);
	LightPos3 = glm::vec3(0.f, 0.f, 0.f);
	
#pragma region WALLS
	walls = new Primitives*[4];
	glm::vec3 wallPos;
	for (int i = 0; i < 4; i++) {
		walls[i] = new Primitives();
		switch (i) {
		case 0:
			wallPos = glm::vec3(0, 4, 14);
			break;
		case 1:
			wallPos = glm::vec3(14, 4, 0);
			break;
		case 2: 
			wallPos = glm::vec3(0, 4, -14);
			break;
		case 3:
			wallPos = glm::vec3(-14, 4, 0);
			break;
		}
		Transform trans = Transform(wallPos, i * 90, glm::vec3(0, 1, 0), glm::vec3(30, 8, 2));
		walls[i]->transform = trans;
	}
#pragma endregion

#pragma region Side Column
	columns = new Primitives*[4];
	glm::vec3 colPos;
	for (int i = 0; i < 4; i++) {
		columns[i] = new Primitives();
		switch (i) {
		case 0:
			colPos = glm::vec3(-14, 5, 14);
			break;
		case 1:
			colPos = glm::vec3(14, 5, 14);
			break;
		case 2:
			colPos = glm::vec3(14, 5, -14);
			break;
		case 3:
			colPos = glm::vec3(-14, 5, -14);
			break;
		}
		Transform trans = Transform(colPos, 0, glm::vec3(0, 1, 0), glm::vec3(3, 10, 3));
		columns[i]->transform = trans;
	}
#pragma endregion

#pragma region Chop Cone
	ChopCones = new Primitives*[4];
	glm::vec3 chopCPos;
	for (int i = 0; i < 4; i++) {
		ChopCones[i] = new Primitives();
		switch (i) {
		case 0:
			chopCPos = glm::vec3(-14, 9, 14);
			break;
		case 1:
			chopCPos = glm::vec3(14, 9, 14);
			break;
		case 2:
			chopCPos = glm::vec3(14, 9, -14);
			break;
		case 3:
			chopCPos = glm::vec3(-14, 9, -14);
			break;
		}
		Transform trans = Transform(chopCPos, 180, glm::vec3(1, 0, 0), glm::vec3(4, 4, 4));
		ChopCones[i]->transform = trans;
	}
#pragma endregion

#pragma region Column2
	topColumns = new Primitives*[4];
	glm::vec3 topColPos;
	for (int i = 0; i < 4; i++) {
		topColumns[i] = new Primitives();
		switch (i) {
		case 0:
			topColPos = glm::vec3(-14, 13, 14);
			break;
		case 1:
			topColPos = glm::vec3(14, 13, 14);
			break;
		case 2:
			topColPos = glm::vec3(14, 13, -14);
			break;
		case 3:
			topColPos = glm::vec3(-14, 13, -14);
			break;
		}
		Transform trans = Transform(topColPos, 0, glm::vec3(0, 1, 0), glm::vec3(4, 4, 4));
		topColumns[i]->transform = trans;
	}

#pragma endregion

#pragma region Cone
	Cones = new Primitives*[4];
	glm::vec3 ConePos;
	for (int i = 0; i < 4; i++) {
		Cones[i] = new Primitives();
		switch (i) {
		case 0:
			ConePos = glm::vec3(-14, 17, 14);
			break;
		case 1:
			ConePos = glm::vec3(14, 17, 14);
			break;
		case 2:
			ConePos = glm::vec3(14, 17, -14);
			break;
		case 3:
			ConePos = glm::vec3(-14, 17, -14);
			break;
		}
		Transform trans = Transform(ConePos, 0, glm::vec3(1, 0, 0), glm::vec3(4, 4, 4));
		Cones[i]->transform = trans;
	}
#pragma endregion

#pragma region Small Brick
	SmallBrick = new Primitives*[4 * 4];
	glm::vec3 brickPos;
	for (int i = 0; i < 16; i++) {
		SmallBrick[i] = new Primitives();
		if (i < 4)
			brickPos = glm::vec3(-7.5 + (i * 5), 8.5, 14.5);
		else if (i < 8)
			brickPos = glm::vec3(14.5, 8.5, -7.5 + ((i - 4) * 5));
		else if (i < 12)
			brickPos = glm::vec3(-7.5 + ((i - 8) * 5), 8.5, -14.5);
		else
			brickPos = glm::vec3(-14.5 , 8.5, -7.5 + ((i - 12) * 5));
		Transform trans = Transform(brickPos, (i / 4) * 90, glm::vec3(0, 1, 0), glm::vec3(3, 1, 1));
		SmallBrick[i]->transform = trans;
	}
	
#pragma endregion

#pragma region Block
	BaseBlock = new Primitives();
	BaseBlock->transform = Transform(glm::vec3(0, 3, 0), 0, glm::vec3(0, 1, 0), glm::vec3(27, 6, 27));

	Core = new Primitives();
	Core->transform = Transform(glm::vec3(0, 10, 0), 0, glm::vec3(0, 1, 0), glm::vec3(18, 10, 18));

#pragma endregion

#pragma region Core roof
	CoreRoof = new Primitives();
	CoreRoof->transform = Transform(glm::vec3(0, 17 ,0), 45, glm::vec3(0, 1, 0), glm::vec3(13, 4, 13));

	RoofTop = new Primitives();
	RoofTop->transform = Transform(glm::vec3(0, 20, 0), 0, glm::vec3(0, 1, 0), glm::vec3(2, 6, 2));

#pragma endregion

#pragma region Cross Pole
	FlagPole = new Primitives();
	FlagPole->transform = Transform(glm::vec3(0, 26, 0), 0, glm::vec3(0, 1, 0), glm::vec3(0.25, 8, 0.25));

	Flag = new Primitives();
	Flag->transform = Transform(glm::vec3(0, 27, 0), 90, glm::vec3(0, 0, 1), glm::vec3(0.25, 4, 0.25));
#pragma endregion

#pragma Door
	door = new Primitives();
	door->transform = Transform(glm::vec3(0, 3, 15), 0, glm::vec3(0, 1, 0), glm::vec3(7, 3, 0.5));

#pragma endregion

#pragma region Windows
	Windows = new Primitives*[2];
	glm::vec3 windowsRot;
	for (int i = 0; i < 2; i++) {
		Windows[i] = new Primitives();
		switch (i) {
		case 0:
			windowsRot = glm::vec3(1, 0, 0);
			break;
		case 1:
			windowsRot = glm::vec3(0, 0, 1);
			break;
		}
		Transform trans = Transform(glm::vec3(0, 11, 0), 90, windowsRot, glm::vec3(2, 19, 2));
		Windows[i]->transform = trans;
	}

	SmallWindows = new Primitives*[2];
	glm::vec3 windowPos;
	for (int i = 0; i < 2; i++) {
		SmallWindows[i] = new Primitives();
		switch (i) {
		case 0:
			windowPos = glm::vec3(4, 11, 0);
			break;
			
		case 1:
			windowPos = glm::vec3(-4, 11, 0);
			break;
		}
		Transform trans = Transform(windowPos, 90, glm::vec3(1, 0, 0), glm::vec3(1, 19, 1));
		SmallWindows[i]->transform = trans;
	}


#pragma endregion



#pragma region TEXTURES SETUP
	int* width;
	int* height;

	images = new unsigned char*[Textures::Count];
	textureID = new GLuint[Textures::Count];
	width = new int[Textures::Count];
	height = new int[Textures::Count];

	for (int i = 0; i < Textures::Count; i++) {
		images[i] = SOIL_load_image(sources[i], &width[i], &height[i], 0, SOIL_LOAD_RGB);

		glGenTextures(1, &textureID[i]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	}
#pragma endregion
	
}

void transformObject(glm::vec3 scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	centerDir = glm::vec3(5 * cos(cAngle * PI / 180), 5 * sin(vAngle * PI / 180), -5 * (sin(cAngle * PI / 180) + cos(vAngle * PI / 180)));
	center = eye + centerDir;
	ViewMatrix = glm::lookAt(eye, center, glm::vec3(0, 1, 0));

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

	glUniform3fv(LightPosID, 1, &LightPos[0]);
	glUniform3fv(LightPosID2, 1, &LightPos2[0]);
	glUniform3fv(LightPosID3, 1, &LightPos3[0]);

	if (GetCursorPos(&p)) { // get cursor pos
		MouseMotion(p.x, p.y);
	}

	camDir = glm::normalize(centerDir);

#pragma region draw walls
	for (int i = 0; i < 4; i++) {
	// draw walls
		glBindVertexArray(*walls[i]->VAO[Type::Cube]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall3]);
		transformObject(walls[i]->transform);
		glDrawElements(GL_TRIANGLES, walls[i]->mNumOfIndex[Type::Cube], GL_UNSIGNED_SHORT, 0);
	// draw columns
		glBindVertexArray(*columns[i]->VAO[Type::Cylinder]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall2]);
		transformObject(columns[i]->transform);
		glDrawElements(GL_TRIANGLES, columns[i]->mNumOfIndex[Type::Cylinder], GL_UNSIGNED_SHORT, 0);
	// draw chop cone
		glBindVertexArray(*ChopCones[i]->VAO[Type::Cone]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall2]);
		transformObject(ChopCones[i]->transform);
		glDrawElements(GL_TRIANGLES, ChopCones[i]->mNumOfIndex[Type::Cone], GL_UNSIGNED_SHORT, 0);
	// draw top columns
		glBindVertexArray(*topColumns[i]->VAO[Type::Cylinder]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall2]);
		transformObject(topColumns[i]->transform);
		glDrawElements(GL_TRIANGLES, topColumns[i]->mNumOfIndex[Type::Cylinder], GL_UNSIGNED_SHORT, 0);
	// draw top cone
		glBindVertexArray(*Cones[i]->VAO[Type::Cone]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Roof]);
		transformObject(Cones[i]->transform);
		glDrawElements(GL_TRIANGLES, Cones[i]->mNumOfIndex[Type::Cone], GL_UNSIGNED_SHORT, 0);
	}

	for (int i = 0; i < 16; i++) {
	// draw small bricks
		glBindVertexArray(*SmallBrick[i]->VAO[Type::Cube]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall2]);
		transformObject(SmallBrick[i]->transform);
		glDrawElements(GL_TRIANGLES, SmallBrick[i]->mNumOfIndex[Type::Cube], GL_UNSIGNED_SHORT, 0);
	}

	glBindVertexArray(*BaseBlock->VAO[Type::Cube]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall2]);
	transformObject(BaseBlock->transform);
	glDrawElements(GL_TRIANGLES, BaseBlock->mNumOfIndex[Type::Cube], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*Core->VAO[Type::Cube]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall1]);
	transformObject(Core->transform);
	glDrawElements(GL_TRIANGLES, Core->mNumOfIndex[Type::Cube], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*CoreRoof->VAO[Type::Pyramid]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Roof]);
	transformObject(CoreRoof->transform);
	glDrawElements(GL_TRIANGLES, CoreRoof->mNumOfIndex[Type::Pyramid], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*RoofTop->VAO[Type::OctagonCone]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Roof]);
	transformObject(RoofTop->transform);
	glDrawElements(GL_TRIANGLES, RoofTop->mNumOfIndex[Type::OctagonCone], GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(*FlagPole->VAO[Type::Bar]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall1]);
	transformObject(FlagPole->transform);
	glDrawElements(GL_TRIANGLES, FlagPole->mNumOfIndex[Type::Bar], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*FlagPole->VAO[Type::Bar]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall1]);
	transformObject(FlagPole->transform);
	glDrawElements(GL_TRIANGLES, FlagPole->mNumOfIndex[Type::Bar], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(*Flag->VAO[Type::Triangle]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Wall1]);
	transformObject(Flag->transform);
	glDrawElements(GL_TRIANGLES, Flag->mNumOfIndex[Type::Triangle], GL_UNSIGNED_SHORT, 0);


	glBindVertexArray(*door->VAO[Type::Cuboid]);
	glBindTexture(GL_TEXTURE_2D, textureID[Textures::Door]);
	transformObject(door->transform);
	glDrawElements(GL_TRIANGLES, door->mNumOfIndex[Type::Cuboid], GL_UNSIGNED_SHORT, 0);


	for (int i = 0; i < 2; i++) {
		glBindVertexArray(*Windows[i]->VAO[Type::Bar]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Window]);
		transformObject(Windows[i]->transform);
		glDrawElements(GL_TRIANGLES, Windows[i]->mNumOfIndex[Type::Bar], GL_UNSIGNED_SHORT, 0);

		glBindVertexArray(*SmallWindows[i]->VAO[Type::Octagon]);
		glBindTexture(GL_TEXTURE_2D, textureID[Textures::Window]);
		transformObject(SmallWindows[i]->transform);
		glDrawElements(GL_TRIANGLES, SmallWindows[i]->mNumOfIndex[Type::Octagon], GL_UNSIGNED_SHORT, 0);

	}



#pragma endregion
	glutSwapBuffers();
}

void Timer(int id) {
	glutPostRedisplay();
	glutTimerFunc(33, Timer, 0);
}

void MouseMotion(int x, int y) {
	int dx = x - px;
	int dy = y - py;

	cout << "x: " << dx << "y: " << dy << endl;
	//rotate camera (move center) with dx and dy
	cAngle -= dx;
	if (-60 <= vAngle <= 60)
		vAngle -= dy;

	//reset dx and dy
	px = x;
	py = y;
}


void KeyDown(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		eye += camDir;
		break;
	case 's':
		eye -= camDir;
		break;
	case 'a':
		//eye -= glm::vec3(0.3f, 0.f, 0.f);
		cAngle += 1.f; //strafe left
		break;
	case 'd':
		//eye += glm::vec3(0.3f, 0.f, 0.f);
		cAngle -= 1.f; //strafe right
		break;
	case 'r':
		eye += glm::vec3(0.f, 0.3f, 0.f);
		break;
	case 'f':
		eye -= glm::vec3(0.f, 0.3f, 0.f);
		break;
	/*case 'i':
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
		break;*/
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
	glutCreateWindow("Alliance Castle!");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	//glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
	glEnable(GL_DEPTH_TEST);

	init();

	glutMotionFunc(MouseMotion);
	glutKeyboardFunc(KeyDown);

	glutDisplayFunc(display);

	glutTimerFunc(33, Timer, 0);

	glutIdleFunc(idle);

	glutMainLoop();

}
