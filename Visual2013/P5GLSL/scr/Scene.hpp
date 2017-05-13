#ifndef __SCENE__
#define __SCENE__

#include <windows.h>
#include <stdio.h>
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"
#include "PLANE.h"
#include "ProgramShader.hpp"
#include <fstream>
#include <iostream>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;


class Scene
{
private:
	// Listas luces
	std::vector<DirLight*> dirLightsList;
	std::vector<FocalLight*> focalLightsList;
	std::vector<PointLight*> pointLightsList;
	// Postproceso
	ProgramShader* ppProgShader;
	unsigned int ppProgramShader;

	bool postProcess = false;
public:
	// Cámara
	Camera* cam;
	// Lista mallas
	std::vector<Mesh*> meshList;
	
	Scene(){}

	////Eliminar la escena y sus componentes////
	~Scene()
	{
		while (!dirLightsList.empty())
		{
			DirLight* dl = dirLightsList.back();
			dirLightsList.pop_back();
			delete dl;
		}

		while (!focalLightsList.empty())
		{
			FocalLight* fl = focalLightsList.back();
			focalLightsList.pop_back();
			delete fl;
		}

		while (!pointLightsList.empty())
		{
			PointLight* pl = pointLightsList.back();
			pointLightsList.pop_back();
			delete pl;
		}

		delete cam;

		while (!meshList.empty())
		{
			Mesh* m = meshList.back();
			meshList.pop_back();
			delete m;
		}

		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &planeVertexVBO);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &colorBuffTexId);
		glDeleteTextures(1, &depthBuffTexId);

		delete ppProgShader;
	}
	
	////MESH////
	//Cargar una nueva malla con unos shader específicos
	void Scene::addMesh(const char *pFile, const char *vname, const char *fname)
	{
		meshList.push_back(new Mesh(pFile, vname, fname));
	}

	void Scene::addMesh(const char *pFile, const char *vname, const char *gname, const char *fname)
	{
		meshList.push_back(new Mesh(pFile, vname, gname, fname));
	}

	////OBJECTS////
	// Añade un nuevo objeto a la malla
	void Scene::addObject(float angle, glm::vec3 axis, glm::vec3 position){
		meshList[meshList.size() - 1]->addObject(angle, axis, position);
	}
	// Añade como un objeto como dependiente de otro en cuanto a transformaciones geométricas
	void Scene::setSonOf(Object* son){
		meshList[meshList.size() - 1]->setSonOf(son);
	}
	// Especifica rotación de un objeto en el tiempo
	void Scene::setRotationObject(float angle, glm::vec3 axis){
		meshList[meshList.size() - 1]->setRotationObject(angle, axis);
	}
	// Especifica translación de un objeto en el tiempo
	void Scene::setTraslationObject(glm::vec3 position){
		meshList[meshList.size() - 1]->setTraslationObject(position);
	}

	////Texturas y propiedades////
	//Añadir diferentes tipos de textura a la última malla cargada
	void Scene::addColorTexture(const char *pTexture)
	{
		meshList[meshList.size() - 1]->addColorTexture(pTexture);
	}
	void Scene::addEmiTexture(const char *pTexture)
	{
		meshList[meshList.size() - 1]->addEmiTexture(pTexture);
	}
	void Scene::addNormalTexture(const char *pTexture)
	{
		meshList[meshList.size() - 1]->addNormalTexture(pTexture);
	}
	void Scene::addSpecTexture(const char *pTexture)
	{
		meshList[meshList.size() - 1]->addSpecTexture(pTexture);
	}
	void Scene::setAlpha(float alpha){
		meshList[meshList.size() - 1]->setAlpha(alpha);
	}

	////Cámara////
	void Scene::createCamera(glm::vec3 posCamera, glm::vec2 angCamera, float fieldOfView, float width, float height, float nearPlane, float farPlane)
	{
		posicion = posCamera;
		cam = new Camera(posCamera, angCamera, fieldOfView, width, height, nearPlane, farPlane);
	}

	glm::mat4 Scene::getProj(){
		return cam->getProj();
	}

	void Scene::setProj(glm::mat4 newProj){
		cam->setProj(newProj);
	}

	glm::mat4 Scene::getView(){
		return cam->getView();
	}

	void Scene::setView(glm::mat4 newView){
		cam->setView(newView);
	}

	////Luces////
	//Añadir distintos tipos de luces
	void Scene::addDirLight(glm::vec3 userDir, glm::vec3 userIntensity)
	{
		dirLightsList.push_back(new DirLight(userDir, userIntensity));
		dirLightsDirection[dirLightsList.size() - 1] = this->dirLightsList[dirLightsList.size() - 1]->getLightDir();
		dirLightsIntensity[dirLightsList.size() - 1] = this->dirLightsList[dirLightsList.size() - 1]->getLightIntensity();
		dirLights[dirLightsList.size() - 1] = TRUE;
	}
	void Scene::addPointLight(glm::vec3 userPos, glm::vec3 userIntensity)
	{
		pointLightsList.push_back(new PointLight(userPos, userIntensity));
		pointLightsPosition[pointLightsList.size() - 1] = this->pointLightsList[pointLightsList.size() - 1]->getLightPos();
		pointLightsIntensity[pointLightsList.size() - 1] = this->pointLightsList[pointLightsList.size() - 1]->getLightIntensity();
		pointLights[pointLightsList.size() - 1] = TRUE;
	}
	void Scene::addFocalLight(glm::vec3 userDir, glm::vec3 userPos, glm::vec3 userIntensity, float userApertureAngle)
	{
		focalLightsList.push_back(new FocalLight(userDir, userPos, userIntensity, userApertureAngle));
		focalLightsDirection[focalLightsList.size() - 1] = this->focalLightsList[focalLightsList.size() - 1]->getLightDir();
		focalLightsPosition[focalLightsList.size() - 1] = this->focalLightsList[focalLightsList.size() - 1]->getLightPos();
		focalLightsIntensity[focalLightsList.size() - 1] = this->focalLightsList[focalLightsList.size() - 1]->getLightIntensity();
		focalLightsAngle[focalLightsList.size() - 1] = this->focalLightsList[focalLightsList.size() - 1]->getLightAngle();
		focalLights[focalLightsList.size() - 1] = TRUE;
	}
	
	///Postproceso///
	//Añadir los shader de postproceso a la escena
	void Scene::addPostProcess(const char *vname, const char *fname)
	{
		ppProgShader = new ProgramShader();
		ppProgShader->initPPShader(vname, fname);
		initPlane();
		initFBO();
		resizeFBO(500, 500);
		postProcess = true;
	}

	void Scene::initPlane()
	{
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		glGenBuffers(1, &planeVertexVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
		glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float)* 3, planeVertexPos, GL_STATIC_DRAW);
		glVertexAttribPointer(ppProgShader->getPPAtribute(), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(ppProgShader->getPPAtribute());
	}

	void Scene::postProcessRendering()
	{
		// Activo frame buffer por defecto
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Activo programa que contiene los shaders de postprocesado
		glUseProgram(ppProgShader->getPostProccesProgram());
		// Deshabilito test de profundidad y caras que no se ven
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		// Activo blending y configuro
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_CONSTANT_COLOR, GL_CONSTANT_ALPHA);
		// Controlo pesos entre el frame actual y el frame anterior
		glBlendColor(rgb, rgb, rgb, a);
		glBlendEquation(GL_FUNC_ADD);

		// Texturas del color buffer
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorBuffTexId);

		// Texturas del depth buffer
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, depthBuffTexId);

		loadPPUniforms();

		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}


	void Scene::initFBO()
	{
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &colorBuffTexId);
		glGenTextures(1, &depthBuffTexId);
	}

	void Scene::resizeFBO(unsigned int w, unsigned int h)
	{
		glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorBuffTexId, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffTexId, 0);

		const GLenum buffs[1] = { GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(1, buffs);
		if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		{
			std::cerr << "Error configurando el FBO" << std::endl;
			exit(-1);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Scene::loadPPUniforms(){
		std::vector<int> uniforms = ppProgShader->getPPUniformList();
		// Primera máscara
		if (uniforms[0] != -1) {
			glUniformMatrix3fv(uniforms[0], 1, GL_FALSE, &(mask_user[0][0]));
		}
		// Segunda máscara
		if (uniforms[1] != -1) {
			glUniformMatrix3fv(uniforms[1], 1, GL_FALSE, &(mask_user2[0][0]));
		}
		// Distancia focal
		if (uniforms[2] != -1) {
			glUniform1f(uniforms[2], focalDistance);
		}
		// Distancia máxima desenfoque
		if (uniforms[3] != -1) {
			glUniform1f(uniforms[3], maxDistanceFactor);
		}
		// Distancia plano near
		if (uniforms[4] != -1) {
			glUniform1f(uniforms[4], n);
		}
		// Distancia plano far
		if (uniforms[5] != -1) {
			glUniform1f(uniforms[5], f);
		}
	}
	
	
	////Función de render////
	//Renderiza los diferentes objetos de todas las mallas de la escena, aplicando postproceso después
	void Scene::render()
	{
		for (int i = 0; i < meshList.size(); i++){
			meshList[i]->renderFunction(this->getProj(), this->getView());
		}
		if (postProcess)
			postProcessRendering();
	}

}; //class Scene

#endif