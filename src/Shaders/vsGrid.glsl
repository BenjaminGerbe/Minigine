#version 450

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aFragCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = aFragCoord;
    gl_Position = u_ProjectionMatrix*u_ViewMatrix*vec4(aPos, 1.0);
}