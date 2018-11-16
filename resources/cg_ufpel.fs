#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 color;
uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float specularStrength = 1.0;
	vec3 ambient = 0.5 * lightColor;
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);
	vec3 diff = max(dot(norm, lightDir), 0.0) * lightColor;
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 tex = (texture(texture_diffuse1, TexCoords)).rgb;
	vec3 result;

	if(color != vec3(0.0)) {
		result = color * (ambient + diff + specular);
	}
	else {
		result = tex * (ambient + diff + specular);
	}
	FragColor = vec4(result, 1.0);
}