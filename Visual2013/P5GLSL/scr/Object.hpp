#ifndef __OBJECT__
#define __OBJECT__

#include "ProgramShader.hpp"
#include "Texture.hpp"
#include "globalVariables.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "assimp/quaternion.h"

#include <iostream>
#include <string>
#include <vector>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 

using namespace std;

class Object
{
private:
	
	
public:
	glm::mat4 identity = glm::mat4(1.0);
	glm::mat4 model;
	glm::mat4 model_init;
	std::vector<glm::mat4x4> transformations;
	std::vector<glm::mat4x4> transformations_acum;

	//Crear un objeto de una malla en concreto
	Object(float angle, glm::vec3 axis, glm::vec3 position)
	{
		model_init = glm::mat4(1.0f);
		model_init = glm::rotate(model, angle, axis);
		model_init = glm::translate(model,position);
	}

	~Object()
	{

	}
	//Definir la jerarquía de objetos
	void Object::setSonOf(Object* son)
	{
		for (int i = 0; i < son->transformations.size(); i++)
		{
			transformations.push_back(son->transformations[i]);
			transformations_acum.push_back(son->transformations[i]);
		}
	}

	//Transformaciones de objetos
	void Object::rotateObject(float angle, glm::vec3 axis)
	{
		transformations.push_back(glm::rotate(identity, angle, axis));
		transformations_acum.push_back(glm::rotate(identity, angle, axis));
	}

	void Object::translateObject(glm::vec3 position)
	{
		transformations.push_back(glm::translate(identity, position));
		transformations_acum.push_back(glm::translate(identity, position));
	}

	//Getters y setters

	void Object::setModel(glm::mat4 model){
		this->model = model;
	}
	
};

#endif