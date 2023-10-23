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

void main()
{	
	vec3 col = vec3(0.19,0.65,0.32);
	vec3 L = normalize(dir.direction);
	vec3 N = Normal;
	float Diffuse = max(dot( normalize(N),-(L)),.1f);
    FragColor = vec4(Diffuse.x*dir.color*col*dir.intensity, 1.0);
}