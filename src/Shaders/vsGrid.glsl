// source : https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
#version 450

uniform Matrices {
	mat4 u_ViewMatrix;
	mat4 u_ProjectionMatrix;
	mat4 u_ObjectMatrix;
};

uniform float time_;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aFragCoord;



out vec2 TexCoord;
out vec3 origin;
out vec3 end;
out vec3 direction;
out vec3 pos;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
	TexCoord = aFragCoord;
	origin = UnprojectPoint(aPos.x, aPos.y, 0.0, u_ViewMatrix, u_ProjectionMatrix).xyz;
	end = UnprojectPoint(aPos.x, aPos.y, 1.0, u_ViewMatrix, u_ProjectionMatrix).xyz; 
	pos = vec3(u_ProjectionMatrix*u_ViewMatrix*vec4(aPos,1.0));
	gl_Position =vec4(aPos, 1.0);
}