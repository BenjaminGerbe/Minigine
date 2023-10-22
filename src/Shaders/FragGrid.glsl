#version 450
out vec4 FragColor;

//source : https://www.shadertoy.com/view/7sBGzR

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ObjectMatrix;
};



in vec2 TexCoord;

#define MAJOR_INTERVAL 0.0625
#define INTERVAL_DIVISIONS 8.0

#define MAJOR_INTENSITY 0.3
#define MINOR_INTENSITY 0.1

#define PI 3.141592653589793238f

float grid(float x) {
  // TODO make line width zoom-independent
  float major = smoothstep(0.99, 1.0, cos(x * (2. * PI) / MAJOR_INTERVAL)) * MAJOR_INTENSITY;
  float minor = smoothstep(0.9, 1.0, cos(x * (2. * PI) * INTERVAL_DIVISIONS / MAJOR_INTERVAL)) * MINOR_INTENSITY;
  return max(major, minor);
}

void main()
{
	vec2 uv = TexCoord;
    
    vec3 col = vec3(0.2);
    
    float xCol = grid(uv.x);
    float yCol = grid(uv.y);
 	col = col + max(xCol, yCol);
	if(col.x < .2){
		col = vec3(0);
	}
	FragColor = vec4(col, col.x);
}