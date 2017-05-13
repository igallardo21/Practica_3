#ifndef __MESH__
#define __MESH__

#include "ProgramShader.hpp"
#include "Texture.hpp"
#include "Object.hpp"
#include "globalVariables.hpp"
#include "PLANE.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <vector>

#include <gl/glew.h>
#include <gl/gl.h>
#define SOLVE_FGLUT_WARNING
#include <gl/freeglut.h> 


////ASSIMP////
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;

class Mesh
{
private:

	glm::mat4 modelView;
	glm::mat4 modelViewProj;
	glm::mat4 normal;

	//VAO 
	unsigned int vao;

	//VBOs que forman parte del objeto 
	unsigned int posVBO;
	unsigned int colorVBO;
	unsigned int normalVBO;
	unsigned int texCoordVBO;
	unsigned int tangentVBO;
	unsigned int triangleIndexVBO;

	//Lista de texturas
	std::vector<Texture*> textureList;

	////Variables de assimp////
	unsigned int meshNVertex;
	unsigned int meshNTriangleIndex;
	unsigned int *meshTriangleIndex;
	float *meshVertexPos;
	float *meshVertexNormal;
	float *meshVertexColor;
	float *meshVertexTexCoord;
	float *meshVertexTangent;

	////Shader program/////
	ProgramShader* progShader;
	unsigned int programShader;

	///Propiedades del objeto///
	float alpha;

public:

	//Lista de objectos
	std::vector<Object*> objectList;


	Mesh()
	{

	}
	//Crear una nueva malla con unos determiandos shaders
	Mesh(const char *pFile, const char *vname, const char *fname)
	{
		progShader = new ProgramShader();
		progShader->initShader(vname, fname);
		
		loadModel(pFile);
		initObj();
		
	}

	Mesh(const char *pFile, const char *vname, const char *gname, const char *fname)
	{
		progShader = new ProgramShader();
		progShader->initShaderG(vname, gname, fname);

		loadModel(pFile);
		initObj();

	}

	~Mesh()
	{

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDeleteBuffers(1, &posVBO);
		glDeleteBuffers(1, &colorVBO);
		glDeleteBuffers(1, &normalVBO);
		glDeleteBuffers(1, &texCoordVBO);
		glDeleteBuffers(1, &tangentVBO);

		glDeleteBuffers(1, &triangleIndexVBO);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);

		while (!textureList.empty()){
			Texture* t = textureList.back();
			textureList.pop_back();
			delete t;
		}

		while (!objectList.empty()){
			Object* o = objectList.back();
			objectList.pop_back();
			delete o;
		}

		//ASSIMP//
		delete[] meshTriangleIndex;
		delete[] meshVertexPos;
		delete[] meshVertexNormal;
		delete[] meshVertexColor;
		delete[] meshVertexTexCoord;
		delete[] meshVertexTangent;

	}
	
	//Carga de la malla usando Assimp
	void Mesh::loadModel(const std::string& pFile)
	{
		std::cout << "Cargando modelo" << std::endl;
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(pFile,
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_GenNormals

			);


		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}


		aiMesh* mesh = scene->mMeshes[0];

		meshNVertex = mesh->mNumVertices;
		//std::cout << "Número de vértices: " << meshNVertex << std::endl;
		meshNTriangleIndex = mesh->mNumFaces;
		//std::cout << "Número de caras: " << meshNTriangleIndex << std::endl;
		meshTriangleIndex = new unsigned int[mesh->mNumFaces * 3];
		meshVertexPos = new float[mesh->mNumVertices * 3];
		meshVertexNormal = new float[mesh->mNumVertices * 3];
		meshVertexColor = new float[mesh->mNumVertices * 3];
		meshVertexTexCoord = new float[mesh->mNumVertices * 2];
		meshVertexTangent = new float[mesh->mNumVertices * 3];

		//std::cout << "Calculando topología" << std::endl;

		for (int i = 0; i < mesh->mNumVertices; i++) {

			// Process vertex positions, normals and texture coordinates
			if (mesh->HasPositions()) {
				meshVertexPos[i * 3] = mesh->mVertices[i].x;
				meshVertexPos[i * 3 + 1] = mesh->mVertices[i].y;
				meshVertexPos[i * 3 + 2] = mesh->mVertices[i].z;
			}
			if (mesh->HasNormals()) {
				meshVertexNormal[i * 3] = mesh->mNormals[i].x;
				meshVertexNormal[i * 3 + 1] = mesh->mNormals[i].y;
				meshVertexNormal[i * 3 + 2] = mesh->mNormals[i].z;

				//std::cout << "vn: " << mesh->mNormals[i].x << " - " << mesh->mNormals[i].y << " - " << mesh->mNormals[i].z << std::endl;

			}

			if (mesh->HasTextureCoords(0)) {
				meshVertexTexCoord[i * 2] = mesh->mTextureCoords[0][i].x;
				meshVertexTexCoord[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
			}
			if (mesh->HasVertexColors(0)) {
				meshVertexColor[i * 3] = mesh->mColors[0][i].r;
				meshVertexColor[i * 3 + 1] = mesh->mColors[0][i].g;
				meshVertexColor[i * 3 + 2] = mesh->mColors[0][i].b;;
			}
			if (mesh->HasTangentsAndBitangents()) {
				meshVertexTangent[i * 3] = mesh->mTangents[i].x;
				meshVertexTangent[i * 3 + 1] = mesh->mTangents[i].y;
				meshVertexTangent[i * 3 + 2] = mesh->mTangents[i].z;
			}

		}

		if (mesh->HasFaces()) {
			for (int i = 0; i < mesh->mNumFaces; i++) {
				aiFace face = mesh->mFaces[i];
				meshTriangleIndex[i * 3] = mesh->mFaces[i].mIndices[0];
				meshTriangleIndex[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
				meshTriangleIndex[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
			}
		}

		std::cout << "Modelo cargado con éxito" << std::endl << std::endl;

	}

	void Mesh::initObj()
	{
		std::vector<int> atributes = progShader->getAttributeList();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		if (atributes[0] != -1) {
			glGenBuffers(1, &posVBO);
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glBufferData(GL_ARRAY_BUFFER, meshNVertex*sizeof(float)* 3, meshVertexPos, GL_STATIC_DRAW);
			glVertexAttribPointer(atributes[0], 3, GL_FLOAT, GL_FALSE, 0, 0); glEnableVertexAttribArray(atributes[0]);
		}
		if (atributes[1] != -1) {
			glGenBuffers(1, &colorVBO);
			glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			glBufferData(GL_ARRAY_BUFFER, meshNVertex*sizeof(float)* 3, meshVertexColor, GL_STATIC_DRAW);
			glVertexAttribPointer(atributes[1], 3, GL_FLOAT, GL_FALSE, 0, 0); glEnableVertexAttribArray(atributes[1]);
		}
		if (atributes[2] != -1) {
			glGenBuffers(1, &normalVBO);
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferData(GL_ARRAY_BUFFER, meshNVertex*sizeof(float)* 3, meshVertexNormal, GL_STATIC_DRAW);
			glVertexAttribPointer(atributes[2], 3, GL_FLOAT, GL_FALSE, 0, 0); glEnableVertexAttribArray(atributes[2]);
		}
		if (atributes[3] != -1) {
			glGenBuffers(1, &texCoordVBO);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferData(GL_ARRAY_BUFFER, meshNVertex*sizeof(float)* 2, meshVertexTexCoord, GL_STATIC_DRAW);
			glVertexAttribPointer(atributes[3], 2, GL_FLOAT, GL_FALSE, 0, 0); glEnableVertexAttribArray(atributes[3]);
		}
		if (atributes[4] != -1) {
			glGenBuffers(1, &tangentVBO);
			glBindBuffer(GL_ARRAY_BUFFER, tangentVBO);
			glBufferData(GL_ARRAY_BUFFER, meshNVertex * sizeof(float)* 3, meshVertexTangent, GL_STATIC_DRAW);
			glVertexAttribPointer(atributes[4], 3, GL_FLOAT, GL_FALSE, 0, 0); glEnableVertexAttribArray(atributes[4]);
		}

		glGenBuffers(1, &triangleIndexVBO); //Buffer de posiciones
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			meshNTriangleIndex*sizeof(unsigned int)* 3, meshTriangleIndex,
			GL_STATIC_DRAW);
	}

	void Mesh::renderFunction(glm::mat4 proj, glm::mat4 view)
	{

		glUseProgram(progShader->getProgram());

		//Render the different textures
		for (int i = 0; i < textureList.size(); i++){
			switch (textureList[i]->getTextureType()){
			case 0:
				textureList[i]->renderColorTexture();
				break;
			case 1:
				textureList[i]->renderEmiTexture();
				break;
			case 2:
				textureList[i]->renderNormalTexture();
				break;
			case 3:
				textureList[i]->renderSpecTexture();
				break;
			default:
				break;
			}
		}

		//load uniforms
		loadUniforms(view);

		for (int i = 0; i < objectList.size(); i++){
			renderMesh(proj, view, objectList[i]->model);
		}

	}
	void Mesh::renderMesh(glm::mat4 proj, glm::mat4 view, glm::mat4 model)
	{
		std::vector<int> uniforms = progShader->getUniformsList();
		modelView = view * model;
		modelViewProj = proj * view * model;
		normal = glm::transpose(glm::inverse(modelView));

		if (uniforms[0] != -1)
			glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &(modelViewProj[0][0]));
		if (uniforms[1] != -1)
			glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &(modelView[0][0]));
		if (uniforms[2] != -1)
			glUniformMatrix4fv(uniforms[2], 1, GL_FALSE, &(normal[0][0]));

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, meshNTriangleIndex * 3, GL_UNSIGNED_INT, (void*)0);
	}

	void Mesh::loadUniforms(glm::mat4 view){

		std::vector<int> uniforms = progShader->getUniformsList();

		if (uniforms[3] != -1)
		{
			glUniformMatrix4fv(uniforms[3], 1, GL_FALSE, &(view[0][0]));
		}
		//Propiedades de las luces//
		if (uniforms[4] != -1)
		{
			glUniform3fv(uniforms[4], 3, &(dirLightsDirection[0][0]));
		}
		if (uniforms[5] != -1)
		{
			glUniform3fv(uniforms[5], 3, &(dirLightsIntensity[0][0]));
		}
		if (uniforms[6] != -1)
		{
			glUniform1iv(uniforms[6], 3, &(dirLights[0]));
		}
		if (uniforms[7] != -1)
		{
			glUniform3fv(uniforms[7], 3, &(focalLightsDirection[0][0]));
		}
		if (uniforms[8] != -1)
		{
			glUniform3fv(uniforms[8], 3, &(focalLightsPosition[0][0]));
		}
		if (uniforms[9] != -1)
		{
			glUniform3fv(uniforms[9], 3, &(focalLightsIntensity[0][0]));
		}
		if (uniforms[10] != -1)
		{
			glUniform1fv(uniforms[10], 3, &(focalLightsAngle[0]));
		}
		if (uniforms[11] != -1)
		{
			glUniform1iv(uniforms[11], 3, &(focalLights[0]));
		}
		if (uniforms[12] != -1)
		{
			glUniform3fv(uniforms[12], 3, &(pointLightsPosition[0][0]));
		}
		if (uniforms[13] != -1)
		{
			glUniform3fv(uniforms[13], 3, &(pointLightsIntensity[0][0]));
		}
		if (uniforms[14] != -1)
		{
			glUniform1iv(uniforms[14], 3, &(pointLights[0]));
		}

		//PROPIEDADES OBJETO//
		if (uniforms[15] != -1)
		{
			glUniform1f(uniforms[15], alpha);
		}
	}

	//Crear objetos y aplicarles traslaciones
	void Mesh::addObject(float angle, glm::vec3 axis, glm::vec3 position){
		objectList.push_back(new Object(angle, axis, position));
	}
	void Mesh::setSonOf(Object* son){
		objectList[objectList.size() - 1]->setSonOf(son);
	}
	void Mesh::setRotationObject(float angle, glm::vec3 axis){
		objectList[objectList.size() - 1]->rotateObject(angle, axis);
	}
	void Mesh::setTraslationObject(glm::vec3 position){
		objectList[objectList.size() - 1]->translateObject(position);
	}

	////Texturas y propiedades////

	void Mesh::addColorTexture(const char *pTexture){
		Texture* colorTexture = new Texture();
		colorTexture->addColorTexture(pTexture);
		textureList.push_back(colorTexture);
	}

	void Mesh::addEmiTexture(const char *pTexture){
		Texture* emiTexture = new Texture();
		emiTexture->addEmiTexture(pTexture);
		textureList.push_back(emiTexture);
	}

	void Mesh::addNormalTexture(const char *pTexture){
		Texture* normalTexture = new Texture();
		normalTexture->addNormalTexture(pTexture);
		textureList.push_back(normalTexture);
	}

	void Mesh::addSpecTexture(const char *pTexture){
		Texture* specTexture = new Texture();
		specTexture->addSpecTexture(pTexture);
		textureList.push_back(specTexture);
	}

	void Mesh::setAlpha(float alpha){
		this->alpha = alpha;
	}
};

#endif