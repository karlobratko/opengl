#version 330 core

in vec3 Normal;
in vec3 FragmentPosition;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
  float ambientStrength = 0.1f;
  vec3 ambient = ambientStrength * lightColor;

  vec3 normal = normalize(Normal);
  vec3 lightDirection = normalize(lightPosition - FragmentPosition);
  float diffuseImpact = max(dot(lightDirection, normal), 0.0f);
  vec3 diffuse = diffuseImpact * lightColor;

  float specularStrength = 0.5f;
  vec3 cameraDirection = normalize(cameraPosition - FragmentPosition);
  vec3 reflectedLightDirection = reflect(-lightDirection, normal);
  float specularImpact = pow(max(dot(cameraDirection, reflectedLightDirection), 0.0f), 32);
  vec3 specular = specularStrength * specularImpact * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;
  color = vec4(result, 1.0f);
}