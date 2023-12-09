#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "ModuleScene.h"
#include "ModuleHierarchy.h"
#include "ModuleResources.h"

#include<vector>

enum class Logs
{
	ERROR_LOG,
	WARNING,
	NORMAL,
	MAX,

};

struct logReport
{
	logReport(Logs type, std::string msg);

	std::string message;
	Logs type;

};

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEditor* editor;
	ModuleScene* scene;
	ModuleHierarchy* hierarchy;
	ModuleResources* resources;

	float fps = 1.0f / 60.0f;

	std::vector <logReport> logreports;

private:

	Timer	ms_timer;
	float	dt;
	std::vector<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();
	bool quitapp = false;
	float GetDT();

	static Application* GetInstance();
	void AddLog(Logs type, std::string msg);
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};


extern Application* App;