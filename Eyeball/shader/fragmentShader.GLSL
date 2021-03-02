#version 330 core
out vec4 FragColor;

struct Material{
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
    float shininess;
};

struct Light{
    vec3 color;
    vec3 position;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

//uniform sampler2D objectTexture;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = material.ambient * light.color;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = material.specular * spec * light.color;  
        
    vec3 result = (ambient + diffuse + specular) * material.color;
    FragColor = vec4(result, 1.0);
}