#ifndef __CAMERA__
#define __CAMERA__

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 proj;

public:
	// Constructor por defecto
	Camera(){
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -10.0));
		proj = glm::perspective(glm::radians(50.0f), 720.0f / 480.0f, 1.0f, 200.0f);
	}
	// Constructor habitual
	Camera(glm::vec3 posCamera, glm::vec2 angCamera, float fieldOfView, float width, float height, float nearPlane, float farPlane){
		view = glm::translate(glm::mat4(1.0f), posCamera);
		view = glm::rotate(glm::mat4(1.0f), angCamera.x, glm::vec3(1.0f, 0.0f, 0.0f)) * view;
		view = glm::rotate(glm::mat4(1.0f), angCamera.y, glm::vec3(0.0f, 1.0f, 0.0f)) * view;
		proj = glm::perspective(glm::radians(fieldOfView), float(width) / float(height), nearPlane, farPlane);
	}
	//Getters y setterss (Existen estos términos? wtf Irene)
	glm::mat4 Camera::getView(){
		return view;
	}
	glm::mat4 Camera::getProj(){
		return proj;
	}
	void Camera::setProj(glm::mat4 newProj){
		proj = newProj;
	}
	void Camera::setView(glm::mat4 newView){
		view = newView;
	}
	~Camera(){}
};

#endif