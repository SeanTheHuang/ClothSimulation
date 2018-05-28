#version 420

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_textureCoord;
layout (location = 2) in vec3 a_normal;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 normalMat;

out vec2 textureCoord;
out vec3 normal;
out vec4 mWorldPos;

void main(void)
{

	gl_Position = mvp * vec4(a_position, 1.0f);
	mWorldPos = model * vec4(a_position, 1.0f);
	textureCoord = a_textureCoord;
	normal = (normalize(normalMat * vec4(a_normal,0.0))).xyz;
}