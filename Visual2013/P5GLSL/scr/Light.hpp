#ifndef __LIGHT__
#define __LIGHT__

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Luz direccional
class DirLight
{
private:
	glm::vec3 dir, intensity;
public:
	DirLight()
	{
		dir = glm::vec3(1.0f, 0.0f, 0.0f);
		intensity = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	DirLight(glm::vec3 userDir, glm::vec3 userIntensity)
	{
		dir = userDir;
		intensity = userIntensity;
	}

	~DirLight()
	{
	}

	void DirLight::setLight(glm::vec3 userDir, glm::vec3 userIntensity)
	{
		dir = userDir;
		intensity = userIntensity;
	}
	glm::vec3 DirLight::getLightDir()
	{
		return dir;
	}
	glm::vec3 DirLight::getLightIntensity()
	{
		return intensity;
	}

}; //class DirLight

//Luz focal
class FocalLight
{
private:
	glm::vec3 dir, pos, intensity;
	float apertureAngle;

public:
	FocalLight()
	{
		dir = glm::vec3(1.0f, 0.0f, 0.0f);
		pos = glm::vec3(0.0f, 0.0f, -5.0f);
		intensity = glm::vec3(1.0f, 1.0f, 1.0f);
		apertureAngle = 60.0f;
	}
	FocalLight(glm::vec3 userDir, glm::vec3 userPos, glm::vec3 userIntensity, float userApertureAngle)
	{
		dir = userDir;
		pos = userPos;
		intensity = userIntensity;
		apertureAngle = userApertureAngle;
	}

	~FocalLight()
	{

	}
	void FocalLight::setLight(glm::vec3 userDir, glm::vec3 userPos, glm::vec3 userIntensity, float userApertureAngle)
	{
		dir = userDir;
		pos = userPos;
		intensity = userIntensity;
		apertureAngle = userApertureAngle;
	}
	glm::vec3 FocalLight::getLightDir()
	{
		return dir;
	}
	glm::vec3 FocalLight::getLightPos()
	{
		return pos;
	}
	glm::vec3 FocalLight::getLightIntensity()
	{
		return intensity;
	}
	float FocalLight::getLightAngle()
	{
		return apertureAngle;
	}
	

}; //class FocalLight

//Luz puntual
class PointLight
{
private:
	glm::vec3 pos, intensity;

public:
	PointLight()
	{
		pos = glm::vec3(0.0f, 0.0f, -5.0f);
		intensity = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	PointLight(glm::vec3 userPos, glm::vec3 userIntensity)
	{
		pos = userPos;
		intensity = userIntensity;
	}

	~PointLight()
	{

	}
	void PointLight::setLight(glm::vec3 userPos, glm::vec3 userIntensity)
	{
		pos = userPos;
		intensity = userIntensity;
	}
	
	glm::vec3 PointLight::getLightPos()
	{
		return pos;
	}
	glm::vec3 PointLight::getLightIntensity()
	{
		return intensity;
	}
	

}; //class PointLight

#endif