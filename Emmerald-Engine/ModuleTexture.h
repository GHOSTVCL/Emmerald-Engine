#pragma once
#include "Module.h"
#include "Glew/include/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "SDL/include/SDL_opengl.h"
#include <string>


struct Texture
{
	Texture(GLuint* id, uint* _width, uint* _height)
	{
		textID = *id;
		width = *_width;
		height = *_height;
	}

	GLuint textID;
	uint width;
	uint height;
};
class ModuleTexture : public Module
{
public:
	
	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture() {}
	bool Start();
	bool CleanUp();

	bool GenTexture(GLuint* imgData, GLuint width, GLuint height);

	static Texture* LoadTexture(std::string textfile);

	std::string texturePath;


};