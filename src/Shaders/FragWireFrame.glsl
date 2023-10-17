#version 450
// source shader : https://www.shadertoy.com/view/wdK3Dy

out vec4 FragColor;

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
};

in vec2 TexCoord;


float grid_intensity = 0.7;



void main()
{
   
    // Output to screen
    FragColor = vec4(.0,0,.0,1.0);
}