#include "GeometryGenerator.h"
#include <cmath>


Transform::Transform() 
{
	mPosition = glm::vec3(0);
	mRotation = 0;
	mRotationAxis = glm::vec3(0, 1, 0);
	mScale = glm::vec3(1);
}

Transform::Transform(glm::vec3 position, float rotation, glm::vec3 rotationAxis, glm::vec3 scale)
{
	mPosition = position;
	mRotation = rotation;
	mRotationAxis = rotationAxis;
	mScale = scale;
}
Primitives::Primitives()
{
	VAO = new GLuint * [10];
	VAO[Type::Cube] = Cube();
	VAO[Type::Cuboid] = Cuboid();
	VAO[Type::Bar] = Bar();
	VAO[Type::Pyramid] = Pyramid();
	VAO[Type::HexagonCone] = HexagonCone();
	VAO[Type::Cylinder] = Cylinder();
	VAO[Type::Cone] = Cone();
	VAO[Type::Triangle] = Triangle();
	VAO[Type::Octagon] = Octagon();
	VAO[Type::OctagonCone] = OctagonCone();
}

GLfloat* Primitives::Shape(int numVert)
{
	float angle = 360.0f / (float)numVert;
	GLfloat* shape = new GLfloat[numVert * 2];

	for (int i = 0; i < numVert * 2; i += 2)
	{
		float degree = (i / 2) * angle *-1;
		float radian = glm::radians(degree);

		if (degree == 180 || degree == -180)
			shape[i] = 0.0f;
		else
			shape[i] = glm::sin(radian);

		if (degree == 90 || degree == 270 || degree == -90 || degree == -270)
			shape[i + 1] = 0.0f;
		else
			shape[i + 1] = glm::cos(radian);
	}

	return shape;
}

GLuint* Primitives::Cylinder(int numVert)
{
	GLfloat* shape = Primitives::Shape(numVert);

	int numVertex = ((numVert * 3) * 2) + (numVert * 6);

	GLfloat* vert = new GLfloat[numVertex * 3];
	GLfloat* normal = new GLfloat[numVertex * 3];
	GLfloat* uv = new GLfloat[numVertex * 2];

	GLushort* index_array = new GLushort[numVertex];

	int index_vert = 0;
	int index_uv = 0;



#pragma region Bottom
	for (int i = 0; i < numVert * 2; i += 2)
	{
#pragma region Vertex
		vert[index_vert] = shape[i];
		vert[index_vert + 1] = -0.5f;
		vert[index_vert + 2] = shape[(i + 1) % (numVert * 2)];

		vert[index_vert + 3] = shape[(i + 2) % (numVert * 2)];
		vert[index_vert + 4] = -0.5f;
		vert[index_vert + 5] = shape[(i + 3) % (numVert * 2)];

		// center
		vert[index_vert + 6] = 0.0f;
		vert[index_vert + 7] = -0.5f;
		vert[index_vert + 8] = 0.0f;
#pragma endregion

#pragma region Normal
		normal[index_vert] = 0.0f;
		normal[index_vert + 1] = -1.0f;
		normal[index_vert + 2] = 0.0f;

		normal[index_vert + 3] = 0.0f;
		normal[index_vert + 4] = -1.0f;
		normal[index_vert + 5] = 0.0f;

		normal[index_vert + 6] = 0.0f;
		normal[index_vert + 7] = -1.0f;
		normal[index_vert + 8] = 0.0f;
#pragma endregion

#pragma region UV
		uv[index_uv] = (shape[i] + 1) / 2.f;
		uv[index_uv + 1] = (shape[(i + 1) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 2] = (shape[(i + 2) % (numVert * 2)] + 1) / 2.f;
		uv[index_uv + 3] = (shape[(i + 3) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 4] = 0.5f;
		uv[index_uv + 5] = 0.5f;
#pragma endregion

#pragma region IndexIncrement
		index_vert += 9;
		index_uv += 6;
#pragma endregion
	}
#pragma endregion

#pragma region Top
	for (int i = (numVert * 2) - 2; i >= 0; i -= 2)
	{
#pragma region Vertex
		vert[index_vert] = shape[i];
		vert[index_vert + 1] = 0.5f;
		vert[index_vert + 2] = shape[i + 1];

		int temp_i = i - 2 >= 0 ? i - 2 : (numVert * 2) - 2;
		int temp_i2 = i - 1 >= 0 ? i - 1 : (numVert * 2) - 1;
		vert[index_vert + 3] = shape[temp_i];
		vert[index_vert + 4] = 0.5f;
		vert[index_vert + 5] = shape[temp_i2];

		vert[index_vert + 6] = 0.0f;
		vert[index_vert + 7] = 0.5f;
		vert[index_vert + 8] = 0.0f;
#pragma endregion

#pragma region Normal
		normal[index_vert] = 0.0f;
		normal[index_vert + 1] = 1.0f;
		normal[index_vert + 2] = 0.0f;

		normal[index_vert + 3] = 0.0f;
		normal[index_vert + 4] = 1.0f;
		normal[index_vert + 5] = 0.0f;

		normal[index_vert + 6] = 0.0f;
		normal[index_vert + 7] = 1.0f;
		normal[index_vert + 8] = 0.0f;
#pragma endregion

#pragma region UV
		uv[index_uv] = (shape[i] + 1) / 2.f;
		uv[index_uv + 1] = (shape[(i + 1)] + 1) / 2.f;

		uv[index_uv + 2] = (shape[temp_i] + 1) / 2.f;
		uv[index_uv + 3] = (shape[temp_i2] + 1) / 2.f;

		uv[index_uv + 4] = 0.5f;
		uv[index_uv + 5] = 0.5f;
#pragma endregion

#pragma region IndexIncrement
		index_vert += 9;
		index_uv += 6;
#pragma endregion
	}
#pragma endregion

#pragma region Side

	for (int i = (numVert * 2) - 2; i >= 0; i -= 2)
	{
#pragma region Vertex

		int temp_i = i - 2 >= 0 ? i - 2 : (numVert * 2) - 2;
		int temp_i2 = i - 1 >= 0 ? i - 1 : (numVert * 2) - 1;

		//First triangle
		//Left bottom
		vert[index_vert] = shape[i];
		vert[index_vert + 1] = -0.5f;
		vert[index_vert + 2] = shape[i + 1];
		glm::vec3 a = glm::vec3(vert[index_vert], vert[index_vert + 1], vert[index_vert + 2]);

		//right bottom
		vert[index_vert + 3] = shape[temp_i];
		vert[index_vert + 4] = -0.5f;
		vert[index_vert + 5] = shape[temp_i2];
		glm::vec3 b = glm::vec3(vert[index_vert + 3], vert[index_vert + 4], vert[index_vert + 5]);

		//right top
		vert[index_vert + 6] = shape[temp_i];
		vert[index_vert + 7] = 0.5f;
		vert[index_vert + 8] = shape[temp_i2];
		glm::vec3 c = glm::vec3(vert[index_vert + 6], vert[index_vert + 7], vert[index_vert + 8]);


		//Second triangle
		//right top
		vert[index_vert + 9] = shape[temp_i];
		vert[index_vert + 10] = 0.5f;
		vert[index_vert + 11] = shape[temp_i2];

		//left top
		vert[index_vert + 12] = shape[i];
		vert[index_vert + 13] = 0.5f;
		vert[index_vert + 14] = shape[i + 1];

		//left bottom
		vert[index_vert + 15] = shape[i];
		vert[index_vert + 16] = -0.5f;
		vert[index_vert + 17] = shape[i + 1];

#pragma endregion

#pragma region Normal
		glm::vec3 normalPerVertex = glm::cross((c - b), (a - b));
		glm::normalize(normalPerVertex);
		for (int j = 0; j < 18; j += 3)
		{
			normal[index_vert + j] = normalPerVertex.x;
			normal[index_vert + j + 1] = normalPerVertex.y;
			normal[index_vert + j + 2] = normalPerVertex.z;
		}
#pragma endregion
		index_vert += 18;
	}

#pragma region UV
	float increment = 1.f / (float)numVert;
	for (int i = 0; i < numVert; i++)
	{
		uv[index_uv] = i * increment;
		uv[index_uv + 1] = 1;

		uv[index_uv + 2] = (i + 1) * increment;
		uv[index_uv + 3] = 1;

		uv[index_uv + 4] = (i + 1) * increment;
		uv[index_uv + 5] = 0;

		uv[index_uv + 6] = (i + 1) * increment;
		uv[index_uv + 7] = 0;

		uv[index_uv + 8] = i * increment;
		uv[index_uv + 9] = 0;

		uv[index_uv + 10] = i * increment;
		uv[index_uv + 11] = 1;
		index_uv += 12;
	}
#pragma endregion
#pragma endregion


#pragma region Index
	for (int i = 0; i < numVertex; i++)
	{
		index_array[i] = i;
	}
#pragma endregion



	GLuint* VAO = new GLuint();
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// buffer for vertex
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 3 * sizeof(GLfloat), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// buffer for texture coordinate
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_ARRAY_BUFFER, UBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 2 * sizeof(GLfloat), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// buffer for normal
	GLuint NBO;
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 3 * sizeof(GLfloat), normal, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// buffer for index
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertex * sizeof(GLushort), index_array, GL_STATIC_DRAW);
	//mNumOfIndex[Type::Cylinder] = numVertex;
	return VAO;
}

GLuint* Primitives::Cone(int numVert)
{
	GLfloat* shape = Shape(numVert);

	int numVertex = ((numVert * 3) * 2) + (numVert * 6);

	GLfloat* vert = new GLfloat[numVertex * 3];
	GLfloat* normal = new GLfloat[numVertex * 3];
	GLfloat* uv = new GLfloat[numVertex * 2];

	GLushort* index_array = new GLushort[numVertex];

	int index_vert = 0;
	int index_uv = 0;

	//Bottom
	for (int i = 0; i < numVert * 2; i += 2)
	{
		vert[index_vert] = shape[i];
		vert[index_vert + 1] = -0.5f;
		vert[index_vert + 2] = shape[(i + 1) % (numVert * 2)];

		vert[index_vert + 3] = shape[(i + 2) % (numVert * 2)];
		vert[index_vert + 4] = -0.5f;
		vert[index_vert + 5] = shape[(i + 3) % (numVert * 2)];

		vert[index_vert + 6] = 0.0f;
		vert[index_vert + 7] = -0.5f;
		vert[index_vert + 8] = 0.0f;

		normal[index_vert] = 0.0f;
		normal[index_vert + 1] = -1.0f;
		normal[index_vert + 2] = 0.0f;

		normal[index_vert + 3] = 0.0f;
		normal[index_vert + 4] = -1.0f;
		normal[index_vert + 5] = 0.0f;

		normal[index_vert + 6] = 0.0f;
		normal[index_vert + 7] = -1.0f;
		normal[index_vert + 8] = 0.0f;

		uv[index_uv] = (shape[i] + 1) / 2.f;
		uv[index_uv + 1] = (shape[(i + 1) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 2] = (shape[(i + 2) % (numVert * 2)] + 1) / 2.f;
		uv[index_uv + 3] = (shape[(i + 3) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 4] = 0.5f;
		uv[index_uv + 5] = 0.5f;

		index_vert += 9;
		index_uv += 6;
	}
	//Top
	for (int i = (numVert * 2) - 2; i >= 0; i -= 2)
	{
		int temp_i = i - 2 >= 0 ? i - 2 : (numVert * 2) - 2;
		int temp_i2 = i - 1 >= 0 ? i - 1 : (numVert * 2) - 1;

		vert[index_vert] = shape[i];
		vert[index_vert + 1] = -0.5f;
		vert[index_vert + 2] = shape[i + 1];
		glm::vec3 a = glm::vec3(vert[index_vert], vert[index_vert + 1], vert[index_vert + 2]);

		vert[index_vert + 3] = shape[temp_i];
		vert[index_vert + 4] = -0.5f;
		vert[index_vert + 5] = shape[temp_i2];
		glm::vec3 b = glm::vec3(vert[index_vert + 3], vert[index_vert + 4], vert[index_vert + 5]);

		vert[index_vert + 6] = 0.0f;
		vert[index_vert + 7] = 0.5f;
		vert[index_vert + 8] = 0.0f;
		glm::vec3 c = glm::vec3(vert[index_vert + 6], vert[index_vert + 7], vert[index_vert + 8]);

		glm::vec3 normalPerVertex = glm::cross((a - c), (b - c));
		glm::normalize(normalPerVertex);

		normal[index_vert] = normalPerVertex.x;
		normal[index_vert + 1] = normalPerVertex.y;
		normal[index_vert + 2] = normalPerVertex.z;

		normal[index_vert + 3] = normalPerVertex.x;
		normal[index_vert + 4] = normalPerVertex.y;
		normal[index_vert + 5] = normalPerVertex.z;

		normal[index_vert + 6] = normalPerVertex.x;
		normal[index_vert + 7] = normalPerVertex.y;
		normal[index_vert + 8] = normalPerVertex.z;

		uv[index_uv] = (shape[i] + 1) / 2.f;
		uv[index_uv + 1] = (shape[(i + 1) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 2] = (shape[(i + 2) % (numVert * 2)] + 1) / 2.f;
		uv[index_uv + 3] = (shape[(i + 3) % (numVert * 2)] + 1) / 2.f;

		uv[index_uv + 4] = 0.5f;
		uv[index_uv + 5] = 0.5f;

		index_vert += 9;
		index_uv += 6;
	}

	for (int i = 0; i < numVertex; i++)
	{
		index_array[i] = i;
	}

	GLuint* VAO = new GLuint();
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	// buffer for vertex
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 3 * sizeof(GLfloat), vert, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// buffer for texture coordinate
	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_ARRAY_BUFFER, UBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 2 * sizeof(GLfloat), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// buffer for normal
	GLuint NBO;
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, numVertex * 3 * sizeof(GLfloat), normal, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// buffer for index
	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertex * sizeof(GLushort), index_array, GL_STATIC_DRAW);

	return VAO;
}

GLuint* Primitives::Cube() 
{
	GLfloat* vertices = new GLfloat[24 * 3]
	{	// front
		-0.50,-0.50, 0.50, // 0
		 0.50,-0.50, 0.50, // 1
		 0.50, 0.50, 0.50, // 2 
		-0.50, 0.50, 0.50, // 3
		 //back
		 0.50,-0.50,-0.50, // 4
		-0.50,-0.50,-0.50, // 5
		-0.50, 0.50,-0.50, // 6
		 0.50, 0.50,-0.50, // 7
		 //left
		 0.50,-0.50, 0.50, // 8
		 0.50,-0.50,-0.50, // 9 
		 0.50, 0.50,-0.50, // 10
		 0.50, 0.50, 0.50, // 11
		 //right
		-0.50,-0.50,-0.50, // 12
		-0.50,-0.50, 0.50, // 13
		-0.50, 0.50, 0.50, // 14
		-0.50, 0.50,-0.50, // 15
		 //top
		-0.50, 0.50, 0.50, // 16
		 0.50, 0.50, 0.50, // 17
		 0.50, 0.50,-0.50, // 18
		-0.50, 0.50,-0.50, // 19
		 //bottom
		-0.50,-0.50,-0.50, // 20
		 0.50,-0.50,-0.50, // 21
		 0.50,-0.50, 0.50, // 22
		-0.50,-0.50, 0.50  // 23
	};

	GLushort* index = new GLushort[36]
	{
		 //front
		0, 1, 2,
		2, 3, 0,
		 //back
		6, 5, 4,
		6, 4, 7,
		 //top
		16, 17, 18,
		16, 18, 19,
		 //bottom
		20, 21, 22,
		20, 22, 23,
		 //left
		8, 9, 10,
		8, 10, 11,
		 //right
		12, 13, 14,
		12, 14, 15
	};

	GLfloat* UV = new GLfloat[24 * 2]
	{
		 //front
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		 //back
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		 //left
		0, 1,
		1, 1,
		1, 0,
		0, 0,
		 //right
		0, 1,
		1, 1,
		1, 0,
		0, 0,

		0,1,
		1,1,
		1,0,
		0,0,

		0,1,
		1,1,
		1,0,
		0,0
	};

	GLfloat* normals = new GLfloat[24 * 3]
	{
		 //front 
		 0, 0, 1,
		 0, 0, 1,
		 0, 0, 1,
		 0, 0, 1,
		 //back
		 0, 0,-1,
		 0, 0,-1,
		 0, 0,-1,
		 0, 0,-1,
		 //left 
		 1, 0, 0,
		 1, 0, 0,
		 1, 0, 0,
		 1, 0, 0,
		 //right
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		-1, 0, 0,
		 //top
		 0, 1, 0,
		 0, 1, 0,
		 0, 1, 0,
		 0, 1, 0,
		 //bottom
		 0,-1, 0,
		 0,-1, 0,
		 0,-1, 0,
		 0,-1, 0
	};



	GLuint* gVAO = new GLuint();
	glGenVertexArrays(1, gVAO);
	glBindVertexArray(*gVAO);
	
	std::cout << "VAO generate..." << std::endl;

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	std::cout << "VBO generate..." << std::endl;

	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_ARRAY_BUFFER, UBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(GLfloat), UV, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint NBO;
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLushort), index, GL_STATIC_DRAW);
	
	mNumOfIndex[Type::Cube] = 36;

	return gVAO;
}

GLuint* Primitives::Cuboid()
{
	GLfloat* vertices = new GLfloat[24 * 3]
	{	// front
		-0.50,-1.00, 0.50, // 0
		 0.50,-1.00, 0.50, // 1
		 0.50, 1.00, 0.50, // 2 
		-0.50, 1.00, 0.50, // 3
		 //back
		 0.50,-1.00,-0.50, // 4
		-0.50,-1.00,-0.50, // 5
		-0.50, 1.00,-0.50, // 6
		 0.50, 1.00,-0.50, // 7
		 //left
		 0.50,-1.00, 0.50, // 8
		 0.50,-1.00,-0.50, // 9 
		 0.50, 1.00,-0.50, // 10
		 0.50, 1.00, 0.50, // 11
		 //right
		-0.50,-1.00,-0.50, // 12
		-0.50,-1.00, 0.50, // 13
		-0.50, 1.00, 0.50, // 14
		-0.50, 1.00,-0.50, // 15
		 //top
		-0.50, 1.00, 0.50, // 16
		 0.50, 1.00, 0.50, // 17
		 0.50, 1.00,-0.50, // 18
		-0.50, 1.00,-0.50, // 19
		 //bottom
		-0.50,-1.00,-0.50, // 20
		 0.50,-1.00,-0.50, // 21
		 0.50,-1.00, 0.50, // 22
		-0.50,-1.00, 0.50  // 23
	};

	GLushort* index = new GLushort[36]
	{
		//front
		0, 1, 2,
		2, 3, 0,
		//back
		6, 5, 4,
		6, 4, 7,
		//top
		16, 17, 18,
		16, 18, 19,
		//bottom
		20, 21, 22,
		20, 22, 23,
		//left
		8, 9, 10,
		8, 10, 11,
		//right
		12, 13, 14,
		12, 14, 15
	};

	GLfloat* UV = new GLfloat[24 * 2]
	{
		//front
	   0, 1,
	   1, 1,
	   1, 0,
	   0, 0,
	   //back
		0, 1,
		1, 1,
		1, 0,
		0, 0,
	  //left
		0, 1,
		1, 1,
		1, 0,
		0, 0,
	 //right
		0, 1,
		1, 1,
		1, 0,
		0, 0,

		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0
	};

	GLfloat* normals = new GLfloat[24 * 3]
	{
		//front 
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		//back
		0, 0,-1,
		0, 0,-1,
		0, 0,-1,
		0, 0,-1,
		//left 
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		//right
	   -1, 0, 0,
	   -1, 0, 0,
	   -1, 0, 0,
	   -1, 0, 0,
	   //top
		0, 1, 0,
	    0, 1, 0,
	    0, 1, 0,
	    0, 1, 0,
	   //bottom
	    0,-1, 0,
	    0,-1, 0,
	    0,-1, 0,
	    0,-1, 0
	};



	GLuint* gVAO = new GLuint();
	glGenVertexArrays(1, gVAO);
	glBindVertexArray(*gVAO);


	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	GLuint UBO;
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_ARRAY_BUFFER, UBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(GLfloat), UV, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	GLuint NBO;
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	GLuint IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLushort), index, GL_STATIC_DRAW);

	mNumOfIndex[Type::Cuboid] = 36;

	return gVAO;
}

GLuint * Primitives::Pyramid()
{
	mNumOfIndex[Type::Pyramid] = ((4 * 3) * 2) + (4 * 6);
	return Cone(4);
}

GLuint * Primitives::Triangle()
{
	mNumOfIndex[Type::Triangle] = ((3 * 3) * 2) + (3 * 6);
	return Cylinder(3);
}

GLuint* Primitives::Bar()
{
	mNumOfIndex[Type::Bar] = ((6 * 3) * 2) + (6 * 6);
	return Cylinder(6);
}

GLuint* Primitives::HexagonCone()
{
	mNumOfIndex[Type::HexagonCone] = ((6 * 3) * 2) + (6 * 6);
	return Cone(6);
}

GLuint* Primitives::Cone()
{
	mNumOfIndex[Type::Cone] = ((20 * 3) * 2 + (20 * 6));
	return Cone(20);
}

GLuint* Primitives::Cylinder() 
{
	mNumOfIndex[Type::Cylinder] = ((20 * 3) * 2 + (20 * 6));
	return Cylinder(20);
}

GLuint* Primitives::Octagon() {
	mNumOfIndex[Type::Octagon] = ((8 * 3) * 2 + (8 * 6));
	return Cylinder(8);
}

GLuint* Primitives::OctagonCone() {
	mNumOfIndex[Type::OctagonCone] = ((8 * 3) * 2) + (8 * 6);
	return Cone(8);
}
