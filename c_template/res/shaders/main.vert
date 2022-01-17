#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec4 v_color;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec2 v_tex_coords;

out vec4 color;
out vec3 normal;
out vec2 tex_coords;
out vec4 frag_position;

void main(void) 
{
  gl_Position = vec4(v_position, 1.0f);
  color = v_color;
  normal = v_normal;
  tex_coords = v_tex_coords;
  frag_position = vec4(v_position, 1.0f);
}