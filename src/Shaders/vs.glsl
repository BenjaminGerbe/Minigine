#version 450
out vec4 FragColor;

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

void main()
{
    FragColor = vec4(0.1,0.5,.3, 1.0f);
}