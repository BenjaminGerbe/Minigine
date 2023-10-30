#version 450
out vec4 FragColor;

//source : https://www.shadertoy.com/view/7sBGzR

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ObjectMatrix;
};

uniform float time_;


in vec2 TexCoord;
in vec3 origin;
in vec3 end;
in vec3 direction;
in vec3 pos;



#define MAJOR_INTERVAL 0.0625
#define INTERVAL_DIVISIONS 8.0

#define MAJOR_INTENSITY 0.5
#define MINOR_INTENSITY 0.3

#define PI 3.141592653589793238f

float depth =0.0;

float grid(float x) {
  // TODO make line width zoom-independent
  float major = smoothstep(0.999, 1.0, cos(x * (2. * PI) / 0.05)) * 1.0;
 // float minor = smoothstep(0.99, 1.0, cos(x * (2. * PI) / 0.01)) * 1.0;
  return major;
  //return max(major, minor);
}

float computeDepth(vec3 imp) {
    vec4 clip_space_pos = u_ProjectionMatrix * u_ViewMatrix * vec4(imp.xyz, 1.0);
    return ((clip_space_pos.z / clip_space_pos.w) + 1.0) * 0.5;
}


void main()
{
	
	float t = -origin.z / (end.z - origin.z);
	vec3 impact =  origin + t*(end-origin);
	float dist = mix(1.0,0.0,t*5.0);
	depth = computeDepth(impact);
	vec2 uv = mod(impact.xy,1.0);
	vec3 color = vec3(0.5,0.5,0.5);
	float size = 0.0002;
	if(uv.x <= 0.0+size || uv.x >= 1.0-size ){
		color = vec3(0.83,0.15,0.21);
	}

	if(uv.y <= 0.0+size || uv.y >= 1.0-size ){
		color = vec3(0.24,0.18,0.88);
	}

    float xCol = grid(uv.x);
    float yCol = grid(uv.y);
    float col = max(xCol, yCol)*(1.0);
    FragColor = vec4(col.x*(color*dist.x),col.x*(dist.x));
	gl_FragDepth = depth;
}