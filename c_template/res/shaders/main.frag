#version 330

uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

in vec4 color;
in vec3 normal;
in vec2 tex_coords;
in vec4 frag_position;

out vec4 out_color;

#define R 200.0f

void main(void)
{
  vec2 coord_uv = frag_position.xy / resolution;
  vec2 mouse_uv = mouse / resolution;

  float t = 1.0f - min(length(tex_coords - mouse_uv), R) / R;

  out_color = vec4(
    (sin(t * (tex_coords.x + time)) + 1.0f) / 2.0f,
    (cos(t * (tex_coords.y + time)) + 1.0f) / 2.0f,
    (cos(t * (tex_coords.x + coord_uv.y + time)) + 1.0f) / 2.0f,
    1.0f
  );
}