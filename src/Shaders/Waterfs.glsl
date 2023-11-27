#version 450
out vec4 FragColor;
in vec3 Normal;

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

void main()
{	
	vec3 col = vec3(0.32,0.57,1);
	vec3 L = normalize(dir.direction);
	vec3 N = Normal;
	float Diffuse = max(dot( normalize(N),-(L)),.8f);
    FragColor = vec4(Diffuse.x*col, Diffuse.x);
}