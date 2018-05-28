#version 420

in vec2 textureCoord;
in vec4 mWorldPos;

out vec4 colour;

uniform vec3 cameraPos;
uniform vec3 fogColour;
uniform float fogStart;
uniform float fogEnd;
uniform vec4 mainColour;
uniform sampler2D sampler;

void main(void)
{
	//Set colour, apply fog
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - fogStart)/ fogEnd;
	lerp = clamp(lerp, 0.0f, 1.0f);

	colour = texture(sampler, textureCoord.st) * mainColour;
	colour = vec4(mix(colour.rgb, fogColour.rgb, lerp), colour.a);
}