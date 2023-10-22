#include "Application.h"
#include "ModuleTexture.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment(lib, "DevIL/libx86/DevIL.lib")
#pragma comment(lib, "DevIL/libx86/ILU.lib")
#pragma comment(lib, "DevIL/libx86/ILUT.lib")

//#include "SDL/include/SDL_opengl.h"

ModuleTexture::ModuleTexture(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//textureID = 0;
}

bool ModuleTexture::Init()
{

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	ilInit();

	return true;
}

bool ModuleTexture::CleanUp()
{
	//glDeleteBuffers(1, &textureID);
	return true;
}


bool ModuleTexture::GenTexture(GLuint* imgData, GLuint width, GLuint height)
{
	//Clean textures if there is another
	FreeTexture();

	texWidth = width;
	texHeight = height;

	glEnable(GL_TEXTURE_2D);

	//Generate and bind a texture buffer
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Pick your texture settings with glTexParameter()

	//GL_TEXTURE_WRAP_S/T: How the texture behaves outside 0,1 range (s = x ; t = y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Resize the texture (MIN->make it smaller ; MAG->make it bigger)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

	//cleaning texture


	return true;
}

bool ModuleTexture::LoadTexture(std::string path)
{
	//Texture loading success
	bool textureLoaded = false;

	//Generate and set current image ID
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	//Load image
	ILboolean success = ilLoadImage(path.c_str());

	//Image loaded successfully
	if (success == IL_TRUE)
	{
		//Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (success == IL_TRUE)
		{
			//Create texture from file pixels
			textureLoaded = GenTexture((GLuint*)ilGetData(), (GLuint)ilGetInteger(IL_IMAGE_WIDTH), (GLuint)ilGetInteger(IL_IMAGE_HEIGHT));
		}

		//Delete file from memory
		ilDeleteImages(1, &imgID);

		//Report error
		if (!textureLoaded)
		{
			printf("Unable to load %s\n", path.c_str());
		}
	}
	return textureLoaded;
}

void ModuleTexture::FreeTexture()
{
	//Delete texture
	if (texID != 0)
	{
		glDeleteTextures(1, &texID);
		texID = 0;
	}

	texWidth = 0;
	texHeight = 0;

}
