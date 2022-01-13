#version 330 core

in vec3 position;
in vec3 normal;

out vec3 _normal;
out vec3 _fragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
  _normal = mat3(transpose(inverse(model))) * normal;
  _fragmentPosition = vec3(model * vec4(position, 1.0f));

  gl_Position = projection * view * model * vec4(position, 1.0f);
}