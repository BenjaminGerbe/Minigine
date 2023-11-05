#version 430
out vec4 FragColor;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

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
uniform vec3 color;

in vec3 Normal;


uniform vec3 u_SpecularColor;
uniform vec3 u_DiffuseColor;
uniform vec3 u_AmbianteColor;
uniform float u_Roughness;
uniform int u_Metalness;
uniform float u_Reflectance;
uniform mat4 u_WorldMatrix;


void main(void)
{
   
    vec3 N = normalize(Normal);

    vec3 L = normalize(dir.direction);
    float NdotL = clamp(dot(N,-L),0.0,1.0);
    
    float bias = 0.005*tan(acos(NdotL));
    bias = clamp(bias, 0.0,0.01);
 
    float Roughness = clamp(u_Roughness,0.001,1.0);
    Roughness = Roughness*Roughness;
    float Shininess = 2.0 / (Roughness * Roughness)-2.0;
    float normalisation = ((Shininess + 8.0) / 8.0);
    vec3 V = -(vec3(inverse(u_ProjectionMatrix * u_ViewMatrix) * vec4(0, 0, 1.0, 1.0)));
    vec3 H = normalize(V + L);
    float NdotH = clamp(dot(N, H), 0, 1);
    float NdotV = clamp(dot(N, V), 0, 1);

 
    vec3 R = reflect(V, normalize(Normal)); 

    vec3 ambiante = u_AmbianteColor;
    vec3 diffuse = (1.0 - u_Metalness)*u_DiffuseColor*NdotL;
    diffuse = mix(diffuse,diffuse,clamp(u_Reflectance*u_Reflectance,0,1));

    vec3 SpecularColor = mix(vec3(1,1,1),u_SpecularColor,u_Metalness);
    vec3 specular = SpecularColor * normalisation * pow(NdotH,Shininess);


    vec3 F0 = mix(vec3(0.16 * u_Reflectance * u_Reflectance), vec3(ambiante), u_Metalness);
    vec3 Fresnel = F0 + (1.0 - F0) * (1.0 - pow(NdotV, 5.0));
    vec3 FresnelNL = 1.0 - F0 + (1.0 - F0) * (1.0 - pow(NdotL, 5.0));

     vec3 Coefs = F0;
   

    if(u_Metalness>0){
        ambiante= mix(ambiante,ambiante,clamp(u_Reflectance*u_Reflectance,0,1));
    }
    
    FragColor = vec4(ambiante+ (diffuse*FresnelNL)+normalisation*(specular*Coefs) ,1.0)*vec4(dir.color,1.0);
    //FragColor = vec4(NdotL.xxx,1.0);
 
}