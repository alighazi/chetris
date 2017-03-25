#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 viewProj;

struct PointLight {    
    vec3 position;
};

#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

out vec2 TexCoords;
out vec3 Normal;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;
out vec3 TangetnPointLightPos[NR_POINT_LIGHTS];

void main()
{
    gl_Position =  viewProj * model * vec4(position,1.0f);
	TexCoords = texCoord;
	Normal = normal;

	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
	vec3 B = normalize(vec3(model * vec4(biTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
	mat3 TBN = transpose(mat3(T, B, N));

    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * vec3(model * vec4(position, 0.0));
	for(int i=0; i<NR_POINT_LIGHTS; i++){
		TangetnPointLightPos[i] =  TBN * vec3(model * vec4(pointLights[i].position	, 0.0));
	}
}