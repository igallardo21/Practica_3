#version 330 core
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

//Color de salida
out vec4 outColor;

//Variables Variantes
in vec2 texCoord;

//Textura
layout (binding = 0) uniform sampler2D colorTex;
layout (binding = 1) uniform sampler2D depthTex;

layout (location = 0) uniform mat3 mask_user;
layout (location = 1) uniform mat3 mask_user2;

layout (location = 2) uniform float focalDistance;
layout (location = 3) uniform float maxDistanceFactor;
layout (location = 4) uniform float n;
layout (location = 5) uniform float f;


	//Coordenadas de textura
const vec2 texIdx[9u] = vec2[](
	vec2(-1.0,1.0), vec2(0.0,1.0), vec2(1.0,1.0),
	vec2(-1.0,0.0), vec2(0.0,0.0), vec2(1.0,1.0),
	vec2(-1.0,-1.0), vec2(0.0,-1.0), vec2(1.0,-1.0));

	//Definir un vector de floats
const float mask[9u] = float[](
	float (mask_user[0][0]), float (mask_user[1][0]), float (mask_user[2][0]),
	float (mask_user[0][1]), float (mask_user[1][1]), float (mask_user[2][1]),
	float (mask_user[0][2]), float (mask_user[1][2]), float (mask_user[2][2]));

const float mask2[9u] = float[](
	float (mask_user2[0][0]), float (mask_user2[1][0]), float (mask_user2[2][0]),
	float (mask_user2[0][1]), float (mask_user2[1][1]), float (mask_user2[2][1]),
	float (mask_user2[0][2]), float (mask_user2[1][2]), float (mask_user2[2][2]));

void main()
{
	//Sería más rápido utilizar una variable uniform el tamaño de la textura.
	vec2 ts = vec2(1.0) / vec2 (textureSize (colorTex,0));
	
	float vcz = - ((n * f) / (f + texture(depthTex,texCoord).x * (n - f)));
	float dof = abs(vcz - focalDistance) * maxDistanceFactor;
	dof = clamp (dof, 0.0, 1.0);
	dof *= dof; 
	vec4 color = vec4 (0.0);
	for (uint i = 0u; i < 9u; i++)
	{
		vec2 iidx = texCoord + ts * texIdx[i];
		color += texture(colorTex, iidx,0.0) * mask[i];
		color += texture(colorTex, iidx,0.0) * mask2[i];
	}
	
	outColor = color;
}