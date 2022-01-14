#version 330 core

in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 _normal;
out vec2 _texCoords;
out vec3 _fragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
  _normal = mat3(transpose(inverse(model))) * normal;
  _texCoords = texCoords;
  _fragmentPosition = vec3(model * vec4(position, 1.0f));

  gl_Position = projection * view * model * vec4(position, 1.0f);
}