#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 mWorldPos;

out vec4 color;

uniform sampler2D texture_diffuse0;
uniform vec3 modelColour;
uniform vec3 cameraPos;

uniform vec3 fogColour;
uniform float fogStart;
uniform float fogEnd;

void main(){

	//Set colour, apply fog
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - fogStart)/ fogEnd;
	lerp = clamp(lerp, 0.0f, 1.0f);

	vec3 usedColour = mix(modelColour, fogColour, lerp);

	color =  vec4(usedColour, 1.0);
}