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
	VAO = new GLuint * [8];
	VAO[Type::Cube] = Cube();
	VAO[Type::Cuboid] = Cuboid();
	VAO[Type::Bar] = Bar();

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

GLuint* Primitives::Bar()
{
	int numberOfVertices = 6;
	GLfloat* vertices = new GLfloat[(numberOfVertices + 1) * 2 * 3];
	int vertex_index = 0;
	int index_index = 0;
	int uv_index = 0;
	int normal_index = 0;

	float theta = 360 / numberOfVertices;
	float radian = 3.1415 / 180;

#pragma region Vertices setup

	for (int i = 0; i < numberOfVertices; i++) {
		// bot vertex
		vertices[vertex_index] = cos(theta * radian * i); // x
		vertices[vertex_index + 1] = -0.5; // y
		vertices[vertex_index + 2] = sin(theta * radian * i); // z
		
		vertex_index += 3;
	}
	for(int i = 0; i < numberOfVertices; i++){
		// top vertex
		vertices[vertex_index ] = cos(theta * radian * i);// x top
		vertices[vertex_index + 1] = 0.5; // y top
		vertices[vertex_index + 2] = sin(theta * radian * i); // z top

		vertex_index += 3;
	}
	// bot origin
	vertices[vertex_index] = 0;
	vertices[vertex_index + 1] = -0.5;
	vertices[vertex_index + 2] = 0;
	// top origin
	vertices[vertex_index + 3] = 0;
	vertices[vertex_index + 4] = 0.5;
	vertices[vertex_index + 5] = 0;
#pragma endregion

	GLushort* index = new GLushort[numberOfVertices * 4 * 3];


// indices for top and bot
	// bot index
	for (int i = 0; i < numberOfVertices; i++) {
		if (i == numberOfVertices - 1)
		{
			// bot
			index[index_index] = i;
			index[index_index + 1] = 0;
			index[index_index + 2] = numberOfVertices * 2;

			std::cout << "Last bot: " << i << std::endl;
			std::cout << "Last bot: 0" << std::endl;
			std::cout << "Last bot: " << numberOfVertices * 2 << std::endl;

		}
		else {
			// bot
			index[index_index] = i;
			index[index_index + 1] = i + 1;
			index[index_index + 2] = numberOfVertices * 2;
			//std::cout << i + 1 << " bot: " << i << std::endl;
			//std::cout << i + 1 << " bot: " << i + 1 << std::endl;
		}
		index_index += 3;
	}

	// bot index
	for (int i = 0; i < numberOfVertices; i++) {
		if (i == numberOfVertices - 1) {
			// top
			index[index_index] = i + numberOfVertices;
			index[index_index + 1] = numberOfVertices;
			index[index_index + 2] = 1 + numberOfVertices * 2;
		}
		else {
			// top
			index[index_index] = i + numberOfVertices;
			index[index_index + 1] = i + numberOfVertices + 1;
			index[index_index + 2] = 1 + numberOfVertices * 2;
			std::cout << i + 1 << " top: " << i + numberOfVertices << std::endl;
			std::cout << i + 1 << " top: " << i + numberOfVertices + 1 << std::endl;
			std::cout << "//////////////////////////////////////////" << std::endl;
		}
		index_index += 3;
	}

	for (int i = 0; i < numberOfVertices; i++) {
		if (i == numberOfVertices - 1) {
			// side
			// first tri
			index[index_index] = i;
			index[index_index + 1] = 0;
			index[index_index + 2] = i + numberOfVertices;
			// second tri
			index[index_index + 3] = 0;
			index[index_index + 4] = i + numberOfVertices;
			index[index_index + 5] = numberOfVertices;
		}
		else {
			// side
			// first tri
			index[index_index] = i;
			index[index_index + 1] = i + 1;
			index[index_index + 2] = i + numberOfVertices;
			// second tri
			index[index_index + 3] = i + 1;
			index[index_index + 4] = i + numberOfVertices;
			index[index_index + 5] = i + numberOfVertices + 1;
		}
		index_index += 6;
	}


	
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
	glBufferData(GL_ARRAY_BUFFER, (numberOfVertices + 1) * 2 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfVertices * 4 * 3 * sizeof(GLushort), index, GL_STATIC_DRAW);

	mNumOfIndex[Type::Bar] = numberOfVertices * 4 * 3;

	return gVAO;
}

