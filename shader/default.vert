#version 330 core
#define NR_POINT_LIGHTS 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 PointLightTangentPosition[NR_POINT_LIGHTS];
    vec3 SpotLightTangentPosition;
    vec3 SpotLightTangentDirection;
    vec3 DirLightTangentDirection;
} vs_out;

uniform mat4 projection;
uniform mat4 viewProj;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 position_spotlight;
uniform vec3 direction_spotlight;
uniform vec3 position_pointLights[NR_POINT_LIGHTS];
uniform vec3 direction_dirLight;


void main()
{
    gl_Position = viewProj * model * vec4(position, 1.0f);
    vs_out.TexCoords = texCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 B = normalize(normalMatrix * bitangent);
    vec3 N = normalize(normalMatrix * normal);    
    
    mat3 TBN = transpose(mat3(T, B, N));  
    vs_out.TangentViewPos  = TBN * viewPos;
    vs_out.TangentFragPos  = TBN * vec3(model * vec4(position, 1.0));

    vs_out.SpotLightTangentPosition = TBN * position_spotlight;
    vs_out.SpotLightTangentDirection = TBN * direction_spotlight;
    vs_out.DirLightTangentDirection = TBN * direction_dirLight;
    for(int i=0;i<NR_POINT_LIGHTS; i++){
        vs_out.PointLightTangentPosition[i] = TBN * position_pointLights[i];
    }
}

