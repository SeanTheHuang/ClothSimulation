#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 mWorldPos;

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 normalMat;

void main(){
	gl_Position = mvp * vec4(position, 1.0f);

	mWorldPos = model * vec4(position, 1.0f);
	FragPos = gl_Position.xyz;
	Normal = (normalize(normalMat * vec4(normal,0.0))).xyz;
}