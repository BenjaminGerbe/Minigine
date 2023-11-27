
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


vec3 Wave(vec3 Position){
 	
 	/*float r = 0.03*exp(-4.0*(0.5 - Position.y)) * Position.x * 2.0;
    float k = 2.0 * 3.1415926 / .5;

    float f = k * (Position.x - 0.1 * time);

	Position.y += r * sin(f)*100.0;*/
	Position.y += sin( (Position.x+ time)*4)*0.2;
	Position.x += cos((Position.x + time)*4)*0.1;
	Position.z += sin((Position.z - time)*4)*0.2;
	return Position;
}

vec3 Trochoidale(vec3 Position){
 	
 	float x = 0;
 	float y = 0;
 	float z = 0;

 	vec3 k = normalize(vec3(0.0,0.0,1.0));
 	float amplitude = .8;
 	float h =  2;
	float phase = 3.14159265359;// (en radian);
	float WaveLength = 2.0;
 	float km = sqrt(k.x*k.x + k.z*k.z);
 	float pulsation = 2*3.14159265359 / WaveLength;
 	float teta = k.x*Position.x + k.z*Position.z - pulsation*time - phase; 

 	x = ((k.x/km)*(amplitude/(tanh(km*h)))*sin(teta));
 	z = ((k.z/km)*(amplitude/(tanh(km*h)))*sin(teta));
 	y = (amplitude)*cos(teta);

 	z = Position.z -z;
 	x = Position.x - x;


	return vec3(Position.x,y,z);
}


void main()
{
	vec3 originPos = aPos;
	vec3 pos = Trochoidale(aPos);
	vec3 normal = aNormal;

	float dx = 0.1;
	vec3 pos_dx = pos;
	vec3 pos_dz = pos;
	pos_dx.x += dx;
	pos_dz.z += dx;
	pos_dx = Trochoidale(pos_dx);
	pos_dz = Trochoidale(pos_dz);

	vec3 tangentX = normalize(pos_dx - pos);
	vec3 tangentZ = normalize(pos_dz - pos);


	vec3 tangent = normalize(tangentZ + tangentX  );

	vec3 bitangent = cross(normal, tangent);
	vec3 adjustedNormal = normalize(cross(tangent, bitangent));

	Normal = vec3(u_ProjectionMatrix  * u_ObjectMatrix * vec4(adjustedNormal, 0.0));
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(pos, 1.0);
}