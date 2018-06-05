#version 420

in vec2 textureCoord;
in vec4 mWorldPos;

out vec4 colour;
uniform sampler2D sampler;

void main(void)
{
	colour = texture(sampler, textureCoord.st);
}