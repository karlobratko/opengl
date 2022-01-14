#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 _normal;
in vec2 _texCoords;
in vec3 _fragmentPosition;

out vec4 color;

uniform Material material;
uniform Light light;
uniform vec3 cameraPosition;

void main()
{
  vec3 normal = normalize(_normal);
  vec3 lightDirection = normalize(light.position - _fragmentPosition);
  vec3 cameraDirection = normalize(cameraPosition - _fragmentPosition);
  vec3 reflectedLightDirection = reflect(-lightDirection, normal);

  vec3 ambient = light.ambient * texture(material.diffuse, _texCoords).rgb;

  float diffuseImpact = max(dot(lightDirection, normal), 0.0f);
  vec3 diffuse = light.diffuse * diffuseImpact * texture(material.diffuse, _texCoords).rgb;

  float specularImpact = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * specularImpact * texture(material.specular, _texCoords).rgb;

  vec3 result = (ambient + diffuse + specular);
  color = vec4(result, 1.0f);
}