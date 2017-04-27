#version 330 core
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 inPos;

out vec2 texCoord;	

void main()
{
	texCoord = 0.5* inPos.xy + vec2(0.5); // Cambio rango 
	gl_Position = vec4(inPos,1.0);
}
