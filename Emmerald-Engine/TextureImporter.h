#pragma once
#include "Glew/include/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "SDL/include/SDL_opengl.h"
#include <string>
#include "Globals.h"

struct Texture
{
	Texture(GLuint id, uint _width, uint _height)
	{
		textID = id;
		width = _width;
		height = _height;
	}

	GLuint textID;
	uint width;
	uint height;
};

namespace TextureImporter {
	
	Texture* ImportTexture(std::string textfile);

}