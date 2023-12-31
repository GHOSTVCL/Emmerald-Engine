#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "JsonParser.h"
#include "parson.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);

	void SetFullscreen(bool ffullscreen);
	void SetResizable(bool resize);
	void SetBorder(bool setborder);
	void SetBrightness(float brightness);
	void SetScreenSize(int w, int h);
	void GetScreenSize(int* wi, int* he);

	bool SaveConfig(JsonParser& node) const;
	bool LoadConfig(JsonParser& node);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
};

#endif // __ModuleWindow_H__