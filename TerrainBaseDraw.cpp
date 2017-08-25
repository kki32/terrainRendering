//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury.
//  FILE NAME: TerrainBaseDraw.cpp
//  ========================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadTGA.h"

using namespace std;

GLuint vaoID;
GLuint theProgram;
GLuint matrixLoc;
glm::mat4 proj, view;


float angle = 0.0;
float cam_hgt = 0.0;

GLuint texLoc6;
GLuint texLoc7;

glm::mat4 projView;
float cx = 0, cy = -50, cz =-5;
float ex = 0.0, ey = 40, ez = 20.0;

float CDR = 3.14159265/180.0;   //Conversion from degrees to radians (required in GLM 0.9.6 and later versions)


GLuint loadShader(GLenum shaderType, string filename)
{
	ifstream shaderFile(filename.c_str());
	if(!shaderFile.good()) cout << "Error opening shader file." << endl;
	stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();
	string shaderStr = shaderData.str();
	const char* shaderTxt = shaderStr.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderTxt, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;
		cerr <<  "Compile failure in shader: " << strInfoLog << endl;
		delete[] strInfoLog;
	}
	return shader;
}

void loadTextures()
{
	//GLuint *textures = new GLuint[1];
    GLuint textureID[5];
    glGenTextures(5, textureID); 
    
    // Load brick texture
	glActiveTexture(GL_TEXTURE0);  //Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textureID[0]);
	loadTGA("HeightMap.tga");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glActiveTexture(GL_TEXTURE1);  
    glBindTexture(GL_TEXTURE_2D, textureID[1]);
	loadTGA("Water.tga");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glActiveTexture(GL_TEXTURE2);  
    glBindTexture(GL_TEXTURE_2D, textureID[2]);
	loadTGA("Grass.tga");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        
    glActiveTexture(GL_TEXTURE3);  
    glBindTexture(GL_TEXTURE_2D, textureID[3]);
	loadTGA("Rock.tga");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glActiveTexture(GL_TEXTURE4);  
    glBindTexture(GL_TEXTURE_2D, textureID[4]);
	loadTGA("Snow.tga");
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   
    
}


void initialise()
{
	loadTextures();

	GLuint shaderv = loadShader(GL_VERTEX_SHADER, "TerrainBase.vert");
	GLuint shaderf = loadShader(GL_FRAGMENT_SHADER, "TerrainBase.frag");
	GLuint shaderc = loadShader(GL_TESS_CONTROL_SHADER, "TerrainBase.cont");
	GLuint shadert = loadShader(GL_TESS_EVALUATION_SHADER, "TerrainBase.eval");
		GLuint shaderg = loadShader(GL_GEOMETRY_SHADER, "TerrainBase.geom");

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderv);
	glAttachShader(program, shaderf);
	glAttachShader(program, shaderc);
	glAttachShader(program, shadert);
	glAttachShader(program, shaderg);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	glUseProgram(program);
	

	GLuint texLoc = glGetUniformLocation(program, "wHeightMap");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	glUniform1i(texLoc, 0); 
	
	GLuint texLoc2 = glGetUniformLocation(program, "waterTexture");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	glUniform1i(texLoc2, 1);
	
	GLuint texLoc3 = glGetUniformLocation(program, "wGrassTexture");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	glUniform1i(texLoc3, 2); 
	
	GLuint texLoc4 = glGetUniformLocation(program, "rockTexture");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	glUniform1i(texLoc4, 3); 
	
	GLuint texLoc5 = glGetUniformLocation(program, "snowTexture");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	glUniform1i(texLoc5, 4); 
	
	
	texLoc6 = glGetUniformLocation(program, "wCamY");  //tex1 is a uniform variable of type Sampler2D in the fragment shader

	
	texLoc7 = glGetUniformLocation(program, "wCamZ");  //tex1 is a uniform variable of type Sampler2D in the fragment shader
	
	
//camPosZ
	proj = glm::perspective(50.0f*CDR, 1.0f, 5.0f, 1000.0f);  //perspective projection matrix
	view = glm::lookAt(glm::vec3(ex,ey,ez), glm::vec3(cx, cy, cz), glm::vec3(0.0, 1.0, 0.0)); //view matrix
	projView = proj*view;  //Product matrix
	

	
	GLuint vboID[3];

	glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenVertexArrays(3, vboID);



	const int range = 15;
	const int verts_s = 4*(range*range) + (4*range) + 1;
	//std::cout << verts_s << std::endl;
		
	GLfloat verts[verts_s*3] = {};
	int i = 0;
	for(int x = range * -1; x <= range; x++){
		for(int y = range * -1; y <= range; y++){
			for(int z = 0; z < 3; z++){
				if(z == 0){
					 verts[i] = y;
				} else if(z == 1){
					verts[i] = 0;	
				} else if(z == 2){
					verts[i] = -1*x;		
				}
				i++;	
			}
		
		}
	}
	
	/*
	for(int i = 0; i < verts_s*3; i =i+3){
			 std::cout << verts[i] << " " << verts[i+1] << " " << verts[i+2] << std::endl;
	}
	*/
	
	
	const int itexCoord = range*2;
	const int texCoord_s = itexCoord*itexCoord + 2*itexCoord + 1;
	GLfloat texCoord[texCoord_s*2] = {};
	 
	i = 0;
	for(int x = 0; x <= itexCoord; x++){
		for(int y = 0; y <= itexCoord; y++){
			for(int z = 0; z < 2; z++){
				if(z == 0){
					texCoord[i] = (1.0/itexCoord) * y;
				} else if(z == 1){
					texCoord[i] = (1.0/itexCoord) * x;		
				}
				i++;
			}
		}
	}
	
	/*
	for(int i = 0; i < texCoord_s*2; i = i+2){
			std::cout << texCoord[i] << " " << texCoord[i+1] << std::endl;
	}
	*/
	 
	//Elems
	
	int skip = 0;
	const int grid_s = range*2;
	const int elems_s = grid_s*grid_s;

	//std::cout << elems_s << std::endl;
		
	GLushort elems[elems_s*4] = {};
	i = 0;
	for(int x = 0; x <= grid_s*grid_s + (grid_s-2); x++){
		if(skip != grid_s){
			for(int z = 0; z < 4; z++){
					if(z == 0){
						 elems[i] = x;
					} else if(z == 1){
						elems[i] = x+1;	
					} else if(z == 2){
						elems[i] = x+(grid_s+2);		
					} else if(z == 3){
						elems[i] = x+(grid_s+1);		
					}
					i++;
				} 
				//std::cout << elems[i-4] << " " << elems[i-3] << " " << elems[i-2] << " " << elems[i-1] << std::endl;
	
			skip += 1;
		} else{
			skip = 0;
		}
	}
	
	/*
	for(int i = 0; i < elems_s*4; i = i+4){
		std::cout << elems[i] << " " << elems[i+1] << " " << elems[i+2] << " " << elems[i+3] << std::endl;
	}
	*/

    
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts) , verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position

  
 
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);  //Texture Coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);  // texture coords
    

	
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW); //Vertex indices

    glBindVertexArray(0);
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


void display()
{
	glUniform1f(texLoc6, ey);
	glUniform1f(texLoc7, ez);
		proj = glm::perspective(30.0f*CDR, 1.0f, 5.0f, 1000.0f);  //perspective projection matrix
	view = glm::lookAt(glm::vec3(ex,ey,ez), glm::vec3(cx, cy, cz), glm::vec3(0.0, 1.0, 0.0)); //view matrix
	projView = proj*view;  //Product matrix
	

	glm::mat4 matrix = glm::mat4(1.0);
	matrix = glm::rotate(matrix, angle*CDR, glm::vec3(0.0, 1.0, 0.0));  //rotation matrix
	
	glm::mat4 prodMatrix = projView * matrix;        //Model-view-proj matrix

	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &prodMatrix[0][0]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoID);
	glDrawElements(GL_PATCHES, 3000, GL_UNSIGNED_SHORT, NULL);

	glFlush();
}

void special(int key, int x, int y)
{
	glm::mat4 proj, view;
	   if(key == GLUT_KEY_LEFT) angle-= 20; 
           if(key == GLUT_KEY_RIGHT) angle += 20; 
		if(key == GLUT_KEY_UP) {	
			ez= ez+0.1;
		}
			 
		else if(key == GLUT_KEY_DOWN)
		{ 		 
			ez= ez-0.1;
		}
		else if(key == GLUT_KEY_F1)
		{ 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if(key == GLUT_KEY_F2)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if(key == GLUT_KEY_F3)
		{
			if((ey + 2) < 100){
				ey += 5;
			}
		}
		else if(key == GLUT_KEY_F4)
		{
			if((ey + 2) > 20){
				ey -= 5;
			}
		}
		proj = glm::perspective(20.0f*CDR, 1.0f, 5.0f, 1000.0f);  //perspective projection matrix
		view = glm::lookAt(glm::vec3(ex,ey,ez), glm::vec3(cx,cy,cz), glm::vec3(0.0, 1.0, 0.0)); //view matrix
	
		projView = proj*view;  //Product matrix

		glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Terrain");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
	}
	else
	{
		cerr << "Unable to initialize GLEW  ...exiting." << endl;
		exit(EXIT_FAILURE);
	}

	initialise();
	glutDisplayFunc(display);
	
	glutSpecialFunc(special);
	glutMainLoop();
}






