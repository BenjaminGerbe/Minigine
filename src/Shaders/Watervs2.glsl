
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

struct WavesData{
	float amplitude;
	vec2 direction;
	float frequency;
	float steepness;
	float speed;
};

uniform DirectionalLight dir;
uniform vec3 color;
uniform float time;

uniform WavesData wavesData;
out vec3 Normal;


vec3 Trochoidale(vec3 Position,float steepness,float amplitude,float frequence,vec2 direction, float phase){
 	
	float x = Position.x;
 	float z = Position.z;
 	float y = Position.y;

 	float Q = steepness/(amplitude*frequence);
 	float teta = frequence*dot(direction,Position.xz)+(phase*time);
 	float fQ = Q*amplitude;

 	float fQTeta = fQ*cos(teta);

 	x += direction.x * fQTeta;
 	z += direction.y * fQTeta;
 	y +=  amplitude   * sin(teta);

	return vec3(x,y,z);
}


void main()
{
	vec3 originPos = aPos;
	vec3 pos = Trochoidale(aPos,wavesData.steepness,wavesData.amplitude,wavesData.frequency,normalize(wavesData.direction),wavesData.speed);
	pos = Trochoidale(pos,wavesData.steepness,0.5,wavesData.frequency,normalize(vec2(1.0,0.0)),wavesData.speed);
 	pos = Trochoidale(pos,0.6,1,.1,normalize(vec2(1.0,.5)),1.0);
 	pos =  Trochoidale(pos,0.5,0.1,10,normalize(vec2(-0.2,1.0)),1.0);
 	pos =  Trochoidale(pos,0.5,0.1,7.5,normalize(vec2(0.5,.0)),1.0);

	Normal = vec3(u_ProjectionMatrix  * u_ObjectMatrix * vec4(aNormal, 0.0));
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(pos, 1.0);
}