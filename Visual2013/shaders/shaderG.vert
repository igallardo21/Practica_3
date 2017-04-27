#version 330 core
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable


layout(location = 0) in vec3 inPos;	
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inNormal;
layout(location = 4) in vec3 inTangent;

layout(location = 0) uniform mat4 modelViewProj;
layout(location = 1) uniform mat4 modelView;
layout(location = 2) uniform mat4 normal;

out vec3 color;
out vec3 pos;
out mat3 TBN;
out vec2 texCoord;


void main()
{
	color = inColor;
	texCoord = inTexCoord;
	vec3 pointN = normalize((normal * vec4(inNormal,0.0)).xyz);
	vec3 pointT = normalize((modelView * vec4(inTangent,0.0)).xyz);
	vec3 pointB = cross(pointN, pointT);
	TBN[0].xyz = pointT;
	TBN[1].xyz = pointB;
	TBN[2].xyz = pointN;
	pos = (modelView * vec4(inPos, 1.0)).xyz;
	
	gl_Position =  modelViewProj * vec4 (inPos,1.0);
}
