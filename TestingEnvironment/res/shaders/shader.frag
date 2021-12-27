#version 330

in vec4 Color;
in vec2 TexCoord;

out vec4 fragmentColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float TexOpacity;

void main() {
	fragmentColor = mix(texture(texture0, TexCoord),
						texture(texture1, TexCoord), TexOpacity) * Color;
}