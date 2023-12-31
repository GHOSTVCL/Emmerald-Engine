#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "Window";

}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::SetFullscreen(bool ffullscreen)
{

	SDL_SetWindowFullscreen(window, ffullscreen);


}

void ModuleWindow::SetResizable(bool resize)
{
	SDL_bool temp;
	if (resize == true) {
		temp = SDL_TRUE;
	}
	else {
		temp = SDL_FALSE;
	}
	SDL_SetWindowResizable(window, temp);
}

void ModuleWindow::SetBorder(bool setborder)
{
	SDL_bool temp;
	if (setborder == true) {
		temp = SDL_TRUE;
	}
	else {
		temp = SDL_FALSE;
	}
	SDL_SetWindowBordered(window, temp);
}

void ModuleWindow::SetBrightness(float brightness)
{

	SDL_SetWindowBrightness(window, brightness);

}

void ModuleWindow::SetScreenSize(int w, int h)
{

	SDL_SetWindowSize(window, w, h);

}

void ModuleWindow::GetScreenSize(int* wi, int* he)
{
	SDL_GetWindowSize(window, wi, he);
}

bool ModuleWindow::SaveConfig(JsonParser& node) const
{

	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "width", App->editor->winWidth);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "height", App->editor->winHeight);
	node.SetNewJsonNumber(node.ValueToObject(node.GetRootValue()), "brightness", App->editor->setbrightness);

	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "fullscreen", App->editor->fullscreen);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "borderless", App->editor->bordered);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "resizable", App->editor->resizeable);

	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "depthEnabled", App->editor->depthEnabled);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "cullEnabled", App->editor->cullEnabled);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "lightsEnabled", App->editor->lightsEnabled);
	node.SetNewJsonBool(node.ValueToObject(node.GetRootValue()), "wireframeEnabled", App->editor->wireframeEnabled);

	return true;
}

bool ModuleWindow::LoadConfig(JsonParser& node)
{
	App->editor->winWidth = (int)node.JsonValToNumber("width");
	App->editor->winHeight = (int)node.JsonValToNumber("height");
	App->editor->setbrightness = (float)node.JsonValToNumber("brightness");

	App->editor->fullscreen = node.JsonValToBool("fullscreen");
	App->editor->bordered = node.JsonValToBool("borderless");
	App->editor->resizeable = node.JsonValToBool("resizable");

	App->editor->depthEnabled = node.JsonValToBool("depthEnabled");
	App->editor->cullEnabled = node.JsonValToBool("cullEnabled");
	App->editor->lightsEnabled = node.JsonValToBool("lightsEnabled");
	App->editor->wireframeEnabled = node.JsonValToBool("wireframeEnabled");
	
	
	SetScreenSize(App->editor->winWidth, App->editor->winHeight);
	SetBrightness(App->editor->setbrightness);

	SetFullscreen(App->editor->fullscreen);
	SetBorder(App->editor->bordered);
	SetResizable(App->editor->resizeable);

	App->renderer3D->SetDepthTest(App->editor->depthEnabled);
	App->renderer3D->SetCullFace(App->editor->cullEnabled);
	App->renderer3D->SetLightning(App->editor->lightsEnabled);
	App->renderer3D->SetWireframe(App->editor->wireframeEnabled);

	return true;
}