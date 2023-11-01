#include "ModuleScene.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

bool ModuleScene::Start()
{
	return false;
}

update_status ModuleScene::Update()
{
	int i = 3;
	return update_status();
}

bool ModuleScene::CleanUp()
{
	return false;
}
