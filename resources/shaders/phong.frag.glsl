#version 330 core

struct Material {
	sampler2D diffuse;
	vec3 diffuse_albedo;
	sampler2D specular;
	vec3 specular_albedo;
	sampler2D emission;
	vec3 emission_albedo;
	float shininess;
}; 

struct Light {
	vec3 position;
  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 oNormal;
in vec2 oTexCoord;
in vec3 oFragPos;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform bool b_Untextured;

void main()
{
	vec3 diffuse_map;
	vec3 specular_map;
	vec3 emission_map;
	
	if (b_Untextured == true)
	{
		diffuse_map = material.diffuse_albedo;
		specular_map = material.specular_albedo;
		emission_map = material.emission_albedo;
	}
	else
	{
		diffuse_map = vec3(texture(material.diffuse, oTexCoord)) * material.diffuse_albedo;
		specular_map = vec3(texture(material.specular, oTexCoord)) * material.specular_albedo;
		emission_map = vec3(texture(material.emission, oTexCoord)) * material.emission_albedo;
	}

	// ambient
	vec3 ambient = light.ambient * diffuse_map;
	
	// diffuse 
	vec3 norm = normalize(oNormal);
	vec3 lightDir = normalize(light.position - oFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * diffuse_map);
	
	// specular
	vec3 viewDir = normalize(viewPos - oFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * specular_map);  

	vec3 result = ambient + diffuse + specular + (emission_map * pow(diff, 0.5));
	
	FragColor = vec4(result, 1.0);
}