#include "auxiliar.hpp"
#include "Scene.hpp"
#include "globalVariables.hpp"
#include "InitEvents.hpp"
#include "edition.hpp"

#include <windows.h>
#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Scene* scene = new Scene();

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));
	initContext(argc, argv);
	initOGL();
	setUpScene(scene);
	navigation();
	glutMainLoop();
	delete scene;
	return 0;
}

// Renderizado de la escena
void renderFunc()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->render();
	glUseProgram(NULL);
	glutSwapBuffers();
}

// Redimensionado ventana
void resizeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	scene->setProj(glm::perspective(glm::radians(60.0f), float(width) / float(height), 1.0f, 50.0f));
	scene->resizeFBO(width, height);
	glutPostRedisplay();
}

// Actualiza la matriz cámara en función de parámetros de teclado
void updateView()
{
	scene->setView(glm::translate(glm::mat4(1.0f), posicion));
	scene->setView(glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f)) * scene->getView());
}

// Función idle, encargada de transformaciones
void idleFunc()
{
	for (int k = 0; k < scene->meshList.size(); k++)
	{
		for (int j = 0; j < scene->meshList[k]->objectList.size(); j++)
		{
			scene->meshList[k]->objectList[j]->model = scene->meshList[k]->objectList[j]->model_init;
			for (int i = 0; i < scene->meshList[k]->objectList[j]->transformations.size(); i++)
			{
				scene->meshList[k]->objectList[j]->setModel(scene->meshList[k]->objectList[j]->transformations_acum[i] * scene->meshList[k]->objectList[j]->model);
				scene->meshList[k]->objectList[j]->transformations_acum[i] *= scene->meshList[k]->objectList[j]->transformations[i];
			}

		}
	}
	glutPostRedisplay();
}