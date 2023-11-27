#include "ModuleScene.h"
#include "Application.h"
ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject("Scene");
	root->parent = nullptr;
	selectedGO = root;

}

bool ModuleScene::Start()
{
	return false;
}

update_status ModuleScene::Update()
{

	return update_status();
}

bool ModuleScene::CleanUp()
{
	return false;
}
