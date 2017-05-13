#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "globalVariables.hpp"
#include "Scene.hpp"

//////////////////////////////
//////// PROPIEDADES /////////
//////////////////////////////

// Cámara
glm::vec3 posCamera = glm::vec3(0.0, 0.0f, -30.0);
glm::vec2 angCamera = glm::vec2(0.0, 0.0);
float fieldOfView = 60.0;
float width = 800.0;
float height = 800.0;
float nearPlane = 0.1;
float farPlane = 1000.0;

// Luces direccionales
glm::vec3 dirDirectional1 = glm::vec3(0.0, 0.0, 1.0);
glm::vec3 intensityDirectional1 = glm::vec3(1.0, 1.0, 1.0);

// Luces puntuales
glm::vec3 posPoint1 = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 intensityPoint1 = glm::vec3(1.0, 1.0, 1.0);

// Luces focales
glm::vec3 dirFocal1 = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 posFocal1 = glm::vec3(0.0, 0.0, 40.0);
glm::vec3 intensityFocal1 = glm::vec3(1.0, 1.0, 1.0);
float angleFocal1 = 60.0;

// Objeto 1
float angleOffset1 = 0.0;
glm::vec3 axisAngleOffset1 = glm::vec3(1.0, 0.0, 0.0);
glm::vec3 positionOffset1 = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 translation1 = glm::vec3(0.0, 0.0, 0.0);
float angleRotation1 = 0.01;
glm::vec3 rotationAxis1 = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 translation1_2 = glm::vec3(0.1, 0.0, 0.0);
// Objeto 2
float angleOffset2 = 30.0;
glm::vec3 axisAngleOffset2 = glm::vec3(1.0, 1.0, 0.0);
glm::vec3 positionOffset2 = glm::vec3(3.0, 0.0, 0.0);
glm::vec3 translation2 = glm::vec3(0.01, -0.01, 0.0);
float angleRotation2 = 0.01;
glm::vec3 rotationAxis2 = glm::vec3(0.0, 1.0, 1.0);

/////////////////////////////

void setUpScene(Scene* scene){

	/////////////////////////////////
	//////// Montaje escena /////////
	/////////////////////////////////


	// CÁMARA //
	scene->createCamera(posCamera, angCamera, fieldOfView, width, height, nearPlane, farPlane);

	// LUCES //
	//scene->addDirLight(dirDirectional1, intensityDirectional1);
	//scene->addFocalLight(dirFocal1, posFocal1, intensityFocal1, angleFocal1);
	scene->addPointLight(posPoint1, intensityPoint1);

	// MALLA 1 //
	scene->addMesh("../objects/room.obj", "../shaders/shaderG.vert", "../shaders/shaderG.geom", "../shaders/shaderG.frag");
	scene->addColorTexture("../img/pared.png");
	//scene->addEmiTexture("../img/emissive.png");
	//scene->addNormalTexture("../img/normal.png");
	//scene->addSpecTexture("../img/specMap.png");
	scene->setAlpha(0.0);

	// OBJETOS MALLA 1 //
	scene->addObject(angleOffset1, axisAngleOffset1, positionOffset1);
	//scene->setRotationObject(angleRotation1, rotationAxis1);

	// MALLA 2 //
	//scene->addMeshG("../objects/cube.obj", "../shaders/shaderG.vert", "../shaders/shaderG.geom", "../shaders/shaderG.frag");
	//scene->addColorTexture("../img/color2.png");
	//scene->addEmiTexture("../img/emissive.png");
	//scene->addNormalTexture("../img/normal.png");
	//scene->addSpecTexture("../img/specMap.png");
	//scene->setAlpha(1000.0);

	// OBJETOS MALLA 2//
	//scene->addObject(angleOffset2, axisAngleOffset2, positionOffset2);
	//scene->setSonOf(scene->meshList[0]->objectList[0]);
	//scene->setRotationObject(angleRotation2, rotationAxis2);
	//scene->setTraslationObject(translation2);

	// POST PROCESO //
	//scene->addPostProcess("../shaders/postProcessing.vert", "../shaders/postProcessing.frag");

	///////////////////////////////
	
}