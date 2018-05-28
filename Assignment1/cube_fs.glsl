#version 420

in vec2 textureCoord;
in vec4 mWorldPos;
in vec3 normal;

out vec4 colour;

uniform vec3 cameraPos;
uniform vec3 fogColour;
uniform float fogStart;
uniform float fogEnd;
uniform vec4 mainColour;

void main(void)
{
float ambientIntensity = 0.7f;
	float specularIntensity = 2.0f;
	float diffuseIntensity = 1.0f;

	//Set colour, apply fog
	float d = distance(mWorldPos.xyz, cameraPos);
	float lerp = (d - fogStart)/ fogEnd;
	lerp = clamp(lerp, 0.0f, 1.0f);
	vec3 usedColour = mix(mainColour.rgb, fogColour, lerp);


	//Do lighting stuff
	vec3 lightPos = vec3(50,0,0);
	vec3 lightDir = normalize(lightPos - mWorldPos.xyz);
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - mWorldPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + cameraPos);  

    float diff = max(dot(lightDir, norm), 0.0) * diffuseIntensity;
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0) * specularIntensity;

	vec3 ambient = ambientIntensity * usedColour;
	vec3 diffuse = diff * usedColour;
	vec3 specular = vec3(specularIntensity) * spec;

	//Apply lighting stuff
	colour =  vec4(ambient + diffuse + specular, 1.0);
}
