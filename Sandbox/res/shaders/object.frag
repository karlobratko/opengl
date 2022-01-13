#version 330 core

in vec3 _normal;
in vec3 _fragmentPosition;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
  vec3 normal = normalize(_normal);
  vec3 lightDirection = normalize(lightPosition - _fragmentPosition);
  vec3 cameraDirection = normalize(cameraPosition - _fragmentPosition);
  vec3 reflectedLightDirection = reflect(-lightDirection, normal);

  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  float diffuseImpact = max(dot(lightDirection, normal), 0.0f);
  vec3 diffuse = diffuseImpact * lightColor;

  float specularStrength = 0.5f;
  float specularImpact = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0f), 64);
  vec3 specular = specularStrength * specularImpact * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}