#version 450
out vec4 FragColor;

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}