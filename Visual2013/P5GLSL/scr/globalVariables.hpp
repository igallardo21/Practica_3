#ifndef __GLOBALVARIABLES__
#define __GLOBALVARIABLES__

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


//////////////////////////
///////// LUCES //////////
//////////////////////////

glm::vec3 dirLightsDirection[3];
glm::vec3 dirLightsIntensity[3];
int dirLights[3] = { 0, 0, 0 };

glm::vec3 focalLightsDirection[3];
glm::vec3 focalLightsPosition[3];
glm::vec3 focalLightsIntensity[3];
float focalLightsAngle[3];
int focalLights[3] = { 0, 0, 0 };

glm::vec3 pointLightsPosition[3];
glm::vec3 pointLightsIntensity[3];
int pointLights[3] = { 0, 0, 0 };

/////////////////
///// IDLE //////
/////////////////

float angleY;
glm::vec3 posicion;

//control de teclado//
bool editing = false;

///////////////////////////
/////// POST PROCESO //////
///////////////////////////
//Plano
unsigned int planeVAO;
unsigned int planeVertexVBO;
//FBO
unsigned int fbo;
unsigned int colorBuffTexId;
unsigned int depthBuffTexId;
//Motion blur//
//Variables para controlar el motion blur
float rgb = 0.5f;
float a = 0.5f;
///Máscaras de convolución//
// Valores del DOF
float focalDistance = -25.0f;
float maxDistanceFactor = 1.0f / 5.0f;

//Valores frustrum
float n = 0.1;
float f = 500.0;

// Máscaras de convolución
float maskFactor = 1.0 / 9.0;
glm::mat3 mask_user = glm::mat3(1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor,
	1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor,
	1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor);

//segunda máscara
glm::mat3 mask_user2 = glm::mat3(1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor,
	1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor,
	1.0*maskFactor, 1.0*maskFactor, 1.0*maskFactor);

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
void edition();
void navigation();
// Eventos gestionados por el sistema operativo
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void PassiveMotionFunc(int x, int y);

//Funciones de inicialización y destrucción
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj();
void initPlane();
void initFBO();
void resizeFBO(unsigned int w, unsigned int h);
void destroy();

//Carga el shader indicado, devuele el ID del shader
GLuint loadShader(const char *fileName, GLenum type);

//Crea una textura, la configura, la sube a OpenGL, y devuelve el identificador de la textura 
unsigned int loadTex(const char *fileName);

//Actualiza la matriz view
void updateView();

#endif