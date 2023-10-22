#pragma once
#include "Module.h"
#include "Glew/include/glew.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"
#include "SDL/include/SDL_opengl.h"
#include <string>



class ModuleTexture : public Module
{
public:

	ModuleTexture(Application* app, bool start_enabled = true);
	~ModuleTexture() {}
	bool Init();
	bool CleanUp();

	bool GenTexture(GLuint* imgData, GLuint width, GLuint height);

	bool LoadTexture(std::string path);

	void FreeTexture();

	std::string texturePath;
	GLuint texID;
	GLuint texWidth, texHeight;

};