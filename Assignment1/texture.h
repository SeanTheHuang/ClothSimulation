#pragma once

#include <glew.h>
#include <SOIL.h>
#include <string>

class Texture {
private:
	std::string textureFileName;
	GLenum textureTarget;
	GLuint textureObject;
	unsigned char* image;
	int imageWidth;
	int imageHeight;
public:
	Texture(GLenum TextureTarget, std::string textureFileName);
	bool load();
	void bind(GLenum textureUnit);

};

