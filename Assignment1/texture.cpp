#include "texture.h"

Texture::Texture(GLenum _textureTarget, std::string _textureFileName) {
	textureTarget = _textureTarget;
	textureFileName = _textureFileName;
}

bool Texture::load() {
	image = SOIL_load_image(
		textureFileName.c_str(),  // file name
		&imageWidth,              // width of the image
		&imageHeight,             // height of the image
		0,                        // number of channels
		SOIL_LOAD_RGBA);

	if (image != NULL) {
		glGenTextures(1, &textureObject);
		glBindTexture(textureTarget, textureObject);
		glTexImage2D(textureTarget, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);


		glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(textureTarget, 0);
		return true;
	}

	return false;
}

void Texture::bind(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObject);
}

