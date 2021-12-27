#version 330

in vec3 aPosition;
in vec4 aColor;
in vec2 aTexCoord;

out vec4 Color;
out vec2 TexCoord;

uniform mat4 transform;

void main() {
	Color = aColor;
	TexCoord = aTexCoord;
	gl_Position = transform * vec4(aPosition, 1.0f);
}