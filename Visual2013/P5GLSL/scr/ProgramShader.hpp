#ifndef __PROGRAM__
#define __PROGRAM__

#include <gl/glew.h>
#include <gl/gl.h>
#include "auxiliar.hpp"
#include "globalVariables.hpp"
#include <iostream>
#include <vector>

class ProgramShader
{
private:
	unsigned int program;
	unsigned int vshader;
	unsigned int gshader;
	unsigned int fshader;

	unsigned int postProccesProgram;
	unsigned int postProccesVShader;
	unsigned int postProccesFShader;

public:

	ProgramShader(){
		program = NULL;
		postProccesProgram = NULL;
	}
	~ProgramShader()
	{
		if (program != NULL){
			glDetachShader(program, vshader);
			glDetachShader(program, fshader);
			glDeleteShader(vshader);
			if (gshader != NULL){
				glDetachShader(program, gshader);
				glDeleteShader(gshader);
			}
			glDeleteShader(fshader);
			glDeleteProgram(program);
		}
		if (postProccesProgram != NULL){
			glDetachShader(postProccesProgram, postProccesVShader);
			glDetachShader(postProccesProgram, postProccesFShader);
			glDeleteShader(postProccesVShader);
			glDeleteShader(postProccesFShader);
			glDeleteProgram(postProccesProgram);
		}

	}

	//Inicializar el shader

	void ProgramShader::initShader(const char *vname, const char *fname)
	{
		//Cargar shaders
		vshader = loadShader(vname, GL_VERTEX_SHADER);		
		fshader = loadShader(fname, GL_FRAGMENT_SHADER);
		

		//Cargar programa & asignarlos al programa
		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);

		// Linkeo el programa
		glLinkProgram(program);

		// ¿Error en el linkado?
		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(program, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteProgram(program);
			program = 0;
			exit(-1);
		}

	}

	void ProgramShader::initShaderG(const char *vname, const char *gname, const char *fname)
	{
		//Cargar shaders
		vshader = loadShader(vname, GL_VERTEX_SHADER);
		gshader = loadShader(gname, GL_GEOMETRY_SHADER);
		fshader = loadShader(fname, GL_FRAGMENT_SHADER);


		//Cargar programa & asignarlos al programa
		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, gshader);
		glAttachShader(program, fshader);

		// Linkeo el programa
		glLinkProgram(program);

		// ¿Error en el linkado?
		int linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(program, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteProgram(program);
			program = 0;
			exit(-1);
		}

	}

	//Inicializar postproceso
	void ProgramShader::initPPShader(const char *vname, const char *fname)
	{
		postProccesVShader = loadShader(vname, GL_VERTEX_SHADER);
		postProccesFShader = loadShader(fname, GL_FRAGMENT_SHADER);

		postProccesProgram = glCreateProgram();
		glAttachShader(postProccesProgram, postProccesVShader);
		glAttachShader(postProccesProgram, postProccesFShader);

		glLinkProgram(postProccesProgram);

		int linked;
		glGetProgramiv(postProccesProgram, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetProgramiv(postProccesProgram, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetProgramInfoLog(postProccesProgram, logLen, NULL, logString);
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteProgram(postProccesProgram);
			exit(-1);
		}

	}

	//Cargar los ficheros del shader
	GLuint loadShader(const char *fileName, GLenum type)
	{
		unsigned int fileLen;
		char *source = loadStringFromFile(fileName, fileLen);

		//Creación y compilación del Shader
		GLuint shader;
		shader = glCreateShader(type);
		glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
		glCompileShader(shader);
		delete source;

		//Comprobamos que se compiló bien
		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			//Calculamos una cadena de error
			GLint logLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
			char *logString = new char[logLen];
			glGetShaderInfoLog(shader, logLen, NULL, logString);
			std::cout << "File name: " << fileName << std::endl;
			std::cout << "Error: " << logString << std::endl;
			delete logString;
			glDeleteShader(shader);
			exit(-1);
		}

		return shader;
	}
	

	unsigned int ProgramShader::getProgram()
	{
		return program;
		
	}

	unsigned int ProgramShader::getPostProccesProgram()
	{
		return postProccesProgram;
	}

	//Atributos
	std::vector<int> ProgramShader::getAttributeList()
	{
		std::vector<int> atributteList;

		atributteList.push_back(glGetAttribLocation(program, "inPos"));
		atributteList.push_back(glGetAttribLocation(program, "inColor"));
		atributteList.push_back(glGetAttribLocation(program, "inNormal"));
		atributteList.push_back(glGetAttribLocation(program, "inTexCoord"));
		atributteList.push_back(glGetAttribLocation(program, "inTangent"));

		return atributteList;
	}
	//Variables uniformes
	std::vector<int> ProgramShader::getUniformsList()
	{
		std::vector<int> uniformsList;
		uniformsList.push_back(glGetUniformLocation(program, "modelViewProj"));
		uniformsList.push_back(glGetUniformLocation(program, "modelView"));
		uniformsList.push_back(glGetUniformLocation(program, "normal"));
		uniformsList.push_back(glGetUniformLocation(program, "view"));

		uniformsList.push_back(glGetUniformLocation(program, "dirLightsDirection"));
		uniformsList.push_back(glGetUniformLocation(program, "dirLightsIntensity"));
		uniformsList.push_back(glGetUniformLocation(program, "dirLights"));

		uniformsList.push_back(glGetUniformLocation(program, "focalLightsDirection"));
		uniformsList.push_back(glGetUniformLocation(program, "focalLightsPosition"));
		uniformsList.push_back(glGetUniformLocation(program, "focalLightsIntensity"));
		uniformsList.push_back(glGetUniformLocation(program, "focalLightsAngle"));
		uniformsList.push_back(glGetUniformLocation(program, "focalLights"));

		uniformsList.push_back(glGetUniformLocation(program, "pointLightsPosition"));
		uniformsList.push_back(glGetUniformLocation(program, "pointLightsIntensity"));
		uniformsList.push_back(glGetUniformLocation(program, "pointLights"));
		
		uniformsList.push_back(glGetUniformLocation(program, "alpha"));

		return uniformsList;
	}

	///POSTPROCESO//
	
	//Atributo postproceso
	int ProgramShader::getPPAtribute()
	{
		return glGetAttribLocation(postProccesProgram, "inPos");
	}
	//Uniformes postproceso
	std::vector<int> ProgramShader::getPPUniformList()
	{
		std::vector<int> uniformsList;
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "mask_user"));
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "mask_user2"));
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "focalDistance"));
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "maxDistanceFactor"));
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "n"));
		uniformsList.push_back(glGetUniformLocation(postProccesProgram, "f"));
		
		return uniformsList;
	}
}; //class Program

#endif