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

uniform vec3 u_SpecularColor;
uniform vec3 u_DiffuseColor;
uniform vec3 u_AmbianteColor;
uniform float u_Roughness;
uniform int u_Metalness;
uniform float u_Reflectance;
uniform mat4 u_WorldMatrix;
out vec3 Normal;

void main(void)
{	
	Normal = vec3(u_ProjectionMatrix*u_ObjectMatrix*vec4(aNormal,0.0));
    gl_Position = u_ProjectionMatrix*u_ViewMatrix*vec4(aPos, 1.0);
}
