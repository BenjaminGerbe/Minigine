#version 450


uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

layout (location = 1) in vec3 aPos;

void main()
{
    gl_Position = u_ProjectionMatrix*u_ViewMatrix*vec4(aPos, 1.0);
}