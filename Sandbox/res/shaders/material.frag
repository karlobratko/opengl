#version 330 core

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 _normal;
in vec3 _fragmentPosition;

out vec4 color;

uniform vec3 cameraPosition;
uniform Material material;
uniform Light light;

void main()
{
  vec3 normal = normalize(_normal);
  vec3 lightDirection = normalize(light.position - _fragmentPosition);
  vec3 cameraDirection = normalize(cameraPosition - _fragmentPosition);
  vec3 reflectedLightDirection = reflect(-lightDirection, normal);

  vec3 ambient = light.ambient * material.ambient;

  float diffuseImpact = max(dot(lightDirection, normal), 0.0f);
  vec3 diffuse = light.diffuse * (diffuseImpact * material.diffuse);

  float specularImpact = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0f), material.shininess);
  vec3 specular = light.specular * (specularImpact * material.specular);

  vec3 result = (ambient + diffuse + specular);
  color = vec4(result, 1.0f);
}