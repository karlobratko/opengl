#version 330 core

in vec3 position;
in vec2 texCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 _texCoordinates;

void main() {
  _texCoordinates = texCoordinates;
  gl_Position = projection * view * model * vec4(position, 1.0f);
}