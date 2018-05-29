#version 420

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_textureCoord;

uniform mat4 vp;

out vec2 textureCoord;
out vec4 mWorldPos;

void main(void)
{

	gl_Position = vp * vec4(a_position, 1.0f);
	mWorldPos = vec4(a_position, 1.0f);
	textureCoord = a_textureCoord;
}