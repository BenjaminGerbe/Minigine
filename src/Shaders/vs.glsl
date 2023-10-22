#version 450
out vec4 FragColor;
in vec3 Normal;

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ObjectMatrix;
};




void main()
{	
	vec3 col = vec3(0.19,0.65,0.32);
	vec3 L = vec3(0.0,-1.0,0.0);
	vec3 N = Normal;
	float Diffuse = max(dot( normalize(N),-L),.1f);
    FragColor = vec4(Diffuse.x*col, 1.0);
}