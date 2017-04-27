#version 330 core
#extension GL_ARB_shading_language_420pack : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(location = 0) out vec4 outColor;

in vec3 fragColor;
in vec3 fragPos;
in vec2 fragTexCoord;
in mat3 fragTBN;

layout(binding = 0) uniform sampler2D colorTex;
layout(binding = 1) uniform sampler2D emiTex;
layout(binding = 2) uniform sampler2D normalTex;
layout(binding = 3) uniform sampler2D specTex;

layout(location = 3) uniform mat4 view;
layout(location = 4) uniform vec3 dirLightsDirection[3];
layout(location = 7) uniform vec3 dirLightsIntensity[3];
layout(location = 10) uniform int dirLights[3];
layout(location = 13) uniform vec3 focalLightsDirection[3];
layout(location = 16) uniform vec3 focalLightsPosition[3];
layout(location = 19) uniform vec3 focalLightsIntensity[3];
layout(location = 22) uniform float focalLightsAngle[3];
layout(location = 25) uniform int focalLights[3];
layout(location = 28) uniform vec3 pointLightsPosition[3];
layout(location = 31) uniform vec3 pointLightsIntensity[3];
layout(location = 34) uniform int pointLights[3];

//Color
vec3 c = vec3(0.0);

//Propiedades del objeto
layout(location = 40) uniform float alpha;

vec3 Ka;
vec3 Kd;
vec3 Ks;
vec3 N;
vec3 Ke;

//Propiedades de la luz
vec3 Ia = vec3 (0.1);
vec3 D;
vec3 lpos;
vec3 Id;
vec3 Is;
float xi;
float f;

//Disminución de la luz con la distancia
float c_1 = 1;
float c_2 = 0.03;
float c_3 = 0.003;
float d;
float fatt;

vec3 L;
float Ip;
vec3 diffuse;
vec3 V;
vec3 R;
float factor;
vec3 specular;

int i = 0;

vec3 shade();

void main()
{
	Ka = texture(colorTex, fragTexCoord).rgb;
	Kd = texture(colorTex, fragTexCoord).rgb;
	
	Ke = texture(emiTex, fragTexCoord).rgb;
	Ks = texture(specTex, fragTexCoord).rgb;

	vec3 N_text = texture(normalTex,fragTexCoord).xyz;
	N = normalize(N_text * 2.0 - 1.0);
	N = normalize(fragTBN * N);
	
	outColor = vec4(shade(), 1.0);   
}

vec3 shade()
{
	c = Ia * Ka;

	i = 0;
	while (dirLights[i] == 1)
	{
		D = dirLightsDirection[i].xyz;
		Id = dirLightsIntensity[i].xyz;
		Is = dirLightsIntensity[i].xyz;

		D = (view * vec4(D,0.0)).xyz; //Dirección de la luz, equivalente a L
		diffuse = Id * Kd * dot (N,-D);
		c += clamp(diffuse, 0.0, 1.0);
		V = normalize (-fragPos);
		R = normalize (reflect (D,N));
		factor = max (dot (R,V), 0.01);
		specular = Is*Ks*pow(factor,alpha);
		c += clamp(specular, 0.0, 1.0);

		i++;
	}

	i = 0;
	while (focalLights[i] == 1)
	{
		D = focalLightsDirection[i].xyz;
		lpos = vec3(view * vec4(focalLightsPosition[i].xyz,1.0)).xyz;
		Id = focalLightsIntensity[i].xyz;
		Is = focalLightsIntensity[i].xyz;
		xi = focalLightsAngle[i] * 3.14159264 / 180;
		f = 5; //Control de la atenuación de la luz

		d = length(lpos - fragPos);
		fatt = 1/(c_1+c_2*d+c_3*pow(d,2));
		fatt = min(fatt,1);

		L = normalize (lpos - fragPos);
		D = (view * vec4(D,0.0f)).xyz; //Dirección del foco, no es equivalente a L como en direccional
		Ip  = (dot(D,-L)-cos(xi))/(1-cos(xi)); 
		Ip  = pow(Ip ,f);
		diffuse = fatt * Ip * Id * Kd * dot (L,N);
		c += clamp(diffuse, 0.0, 1.0);
		V = normalize (-fragPos);
		R = normalize (reflect (-L,N));
		factor = max (dot (R,V), 0.01);
		specular = fatt*Ip*Is*Ks*pow(factor,alpha);
		c += clamp(specular, 0.0, 1.0);

		i++;
	}

	i = 0;
	while (pointLights[i] == 1)
	{
		lpos = vec3(view * vec4(pointLightsPosition[i].xyz,1.0)).xyz;
		Id = pointLightsIntensity[i].xyz;
		Is = pointLightsIntensity[i].xyz;

		d = length(lpos - fragPos);
		fatt = 1/(c_1+c_2*d+c_3*pow(d,2));
		fatt = min(fatt,1);

		L = normalize (lpos - fragPos);
		diffuse = fatt * Id * Kd * dot (L,N);
		c += clamp(diffuse, 0.0, 1.0);
		V = normalize (-fragPos);
		R = normalize (reflect (-L,N));
		factor = max (dot (R,V), 0.01);
		specular = fatt*Is*Ks*pow(factor,alpha);
		c += clamp(specular, 0.0, 1.0);

		i++;
	}

	c+=Ke;
	
	return c;
}
