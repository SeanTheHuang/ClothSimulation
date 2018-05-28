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
	float ambientIntensity = 0.2f;

	//Set colour, apply fog
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - fogStart)/ fogEnd;
	lerp = clamp(lerp, 0.0f, 1.0f);

	vec3 usedColour = texture(texture_diffuse0, TexCoords).rgb * modelColour;
	usedColour = mix(usedColour, fogColour, lerp);


	//Do lighting stuff
	vec3 lightPos = vec3(50, 0, 20);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 norm = normalize(Normal);

    float diff = max(dot(lightDir, norm), 0.0);

	// Toon shader stuff
	if (diff > 0.95f)
		diff = 1;
	else if (diff > 0.5f)
		diff = 0.5f;
	else if (diff > 0.25f)
		diff = 0.25f;
	else
		diff = 0;

	vec3 ambient = ambientIntensity * usedColour;
	vec3 diffuse = diff * usedColour;

	//Apply lighting stuff
	color =  vec4(ambient + diffuse, 1.0);
}