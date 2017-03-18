#version 330 core
in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;

out vec4 color;

uniform vec3 viewPos;	

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    	
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;  

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};   
uniform Material material;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    // Check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity=clamp((light.outerCutOff-theta)/(light.outerCutOff-light.cutOff),0.0,1.0);

        float distance    = length(light.position - FragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
        attenuation*=intensity;

        // Ambient
        vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
        
        // Diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoords));  
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
        
        //ambient  *= attenuation; 
        diffuse  *= attenuation;
        specular *= attenuation; 

        vec3 result = ambient + diffuse + specular;
        color = vec4(result, 1.0);

}