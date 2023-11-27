
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
	float direction;
	float frequency;
	float steepness;
	float speed;
};

uniform DirectionalLight dir;
uniform vec3 color;
uniform float time;

uniform WavesData wavesData;
out vec3 Normal;


vec3 Trochoidale(vec3 Position,float steepness,float amplitude,float frequence,vec2 direction, float phase ){
 	
	float x = Position.x;
 	float z = Position.z;
 	float y = Position.y;

 	float Q = steepness/(frequence);
 	float teta = frequence*dot(direction,Position.xz)+(phase*time);
 	float fQ = Q;

 	float fQTeta = fQ*cos(teta);

 	x += direction.x * fQTeta;
 	z += direction.y * fQTeta;
 	y +=  amplitude   * sin(teta);

	return vec3(x,y,z);
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}


vec3 GersnsterWave(vec3 pos){
		for(int i = 1;i <= 5;i++){

		float amp = 10.5 /(i*i*1.2);
		float angle = i * 3.14159265359 * 2.0 / 5;
		vec2 d = vec2(cos(angle),sin(angle));
		float frq = (i-.6)/3.0;
		float speed = 2.0*(i+1)/5;

		pos = Trochoidale(
		pos,
		wavesData.steepness,
		amp,
		frq,
		normalize(d),
		speed
		
		);
	}

	return pos;
}

void main()
{
	vec3 originPos = aPos;
	vec3 posX = GersnsterWave(aPos + vec3(0.1,0.0,0.0));
	vec3 posZ = GersnsterWave(aPos + vec3(0.0,0.0,0.1));
	vec3 pos =  GersnsterWave(aPos);
	
	vec3 Tangant = normalize(pos-posX);
	vec3 Binormal = normalize(pos-posZ);

	vec3 normal = cross(Binormal,Tangant);

	Normal = vec3(u_ProjectionMatrix  * u_ObjectMatrix * vec4(normal, 0.0));
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(pos, 1.0);
}