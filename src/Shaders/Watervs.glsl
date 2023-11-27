#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ObjectMatrix;
};

struct DirectionalLight{
	vec3 direction;
	vec3 color;
	float intensity;
};

uniform DirectionalLight dir;
uniform vec3 color;
uniform float time;

out vec3 Normal;


vec3 TrochoidaleWave(vec2 direction,float steepness,float WaveLength, vec3 pos,inout vec3 tangent,inout vec3 binormal){
	
	float k = (2*3.14159265359) / WaveLength;
	float c = sqrt(9.8 / k);
	float a = steepness /k;
	float f = k * (dot(pos.xz,direction) - c * time );

	tangent += vec3(
		direction.x * direction.x * (steepness * sin(f)),
		-direction.x * (steepness * cos(f)),
		direction.x * direction.y * (steepness * sin(f))
	);
	binormal += vec3(
		direction.x * direction.y * (steepness * sin(f)),
		-direction.y * (steepness * cos(f)),
		direction.y * direction.y * (steepness * sin(f))
	);

	return vec3(direction.x * a*sin(f),-a*cos(f),direction.y * a*sin(f));
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{

	Normal = vec3(u_ProjectionMatrix*u_ObjectMatrix*vec4(aNormal,0.0));
	vec3 pos = aPos;
	float WaveLength = 50.0;
	vec2 direction= normalize(vec2(1.0,0.0));
	vec2 direction2= normalize(vec2(1.0,0.5));
	float s = .5;
	vec3 tangent = vec3(1,0,0);
	vec3 binormal =  vec3(0,0,1);
	pos += TrochoidaleWave(direction,.7,50,pos,tangent,binormal);
	pos += TrochoidaleWave(normalize(vec2(-.5,.8)),.7,50,pos,tangent,binormal);

	vec3 normal = normalize(cross(binormal, tangent));

	Normal = vec3(u_ProjectionMatrix  * u_ObjectMatrix * vec4(normal, 0.0));
    gl_Position = u_ProjectionMatrix*u_ViewMatrix*vec4(pos, 1.0);
}