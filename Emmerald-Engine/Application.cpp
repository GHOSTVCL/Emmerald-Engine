#include "Application.h"
#include "ModuleInput.h"
#include "ImGUI/imgui.h"
Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	editor = new ModuleEditor(this);
	scene = new ModuleScene(this);
	hierarchy = new ModuleHierarchy(this);
	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(scene);
	AddModule(hierarchy);
	// Renderer last!
	AddModule(renderer3D);
	AddModule(editor);

	loadRequested = true;
	saveRequested = false;

}

Application::~Application()
{
	for (std::vector<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}
}

bool Application::Init()
{
	bool ret = true;
	JSON_Value* root = jsonFile.FileToValue("config.json");

	if (jsonFile.GetRootValue() == NULL)
	{
		ret = false;
	}

	JsonParser application = jsonFile.GetChild(root, "App");

	// Call Init() in all modules
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret; ++it)
	{
		(*it)->Start();
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (loadRequested)
	{
		LoadConfig();
	}

	if (saveRequested)
	{
		SaveConfig();
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	if (game_State == GameState::PLAY) dt_game = dt;
	else dt_game = 0;
	PrepareUpdate();
	
	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PreUpdate(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->Update(dt);
	}

	for (std::vector<Module*>::const_iterator it = list_modules.cbegin(); it != list_modules.cend() && ret == UPDATE_CONTINUE; ++it)
	{
		(*it)->PostUpdate(dt);
	}
	if (quitapp) {
		ret = UPDATE_STOP;
	}
	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::vector<Module*>::reverse_iterator it = list_modules.rbegin(); it != list_modules.rend() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}
	return ret;
}

float Application::GetDT()
{
	return dt;
}

float Application::GetGameDT()
{
	return game_dt;
}
void Application::SetGameDT()
{
	game_timer.Start();
	game_dt = ((float)game_timer.Read() / 1000.0f);
}
void Application::StopGameDT()
{
	game_timer.Stop();
	game_dt = 0;
}
void Application::PauseGameDT()
{
	if (game_dt == 0)
		game_dt = ((float)game_timer.Read() / 1000.0f);
	else
		game_dt = 0;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

Application* Application::GetInstance()
{

	if (App == nullptr) {
		App = new Application();
	}
	return App;
}
void Application::AddLog(Logs type, std::string msg)
{

	logReport temp(type, msg);

	logreports.push_back(temp);
}

logReport::logReport(Logs type, std::string msg)
{
	this->message = msg;
	this->type = type;
}

void Application::SetDT(float dt)
{
	this->dt = dt;
}
float Application::DTG()
{
	return dt_game;
}
bool Application::IsRunning()
{
	return game_State == GameState::PLAY;
}
bool Application::IsPaused()
{
	return game_State == GameState::PAUSE;
}
GameState Application::GetState()
{
	return game_State;
}
void Application::SetState(GameState gameState)
{
	this->game_State = gameState;
}

void Application::SaveConfig()
{

	JSON_Value* root = jsonFile.GetRootValue();

	JsonParser application = jsonFile.SetChild(root, "App");

	// Call SaveConfig() in all modules
	std::vector<Module*>::iterator item;

	for (item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		(*item)->SaveConfig(jsonFile.SetChild(root, (*item)->name));
	}

	jsonFile.SerializeFile(root, "config.json");
	saveRequested = false;
}

void Application::LoadConfig()
{

	JSON_Value* root = jsonFile.GetRootValue();

	JsonParser application = jsonFile.GetChild(root, "App");

	std::vector<Module*>::iterator item;

	for (item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		(*item)->LoadConfig(jsonFile.GetChild(root, (*item)->name));
	}

	loadRequested = false;
}