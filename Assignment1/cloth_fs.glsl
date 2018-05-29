#version 420

in vec2 textureCoord;
in vec4 mWorldPos;

out vec4 colour;


void main(void)
{
	colour = vec4(textureCoord.x, textureCoord.y, 0.0f, 1.0f);
}