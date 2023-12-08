#include "ModuleScene.h"
#include "Application.h"
#include "CompMesh.h"
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

update_status ModuleScene::Update(float dt)
{
	UpdateGo(root);
	return UPDATE_CONTINUE;
}


update_status ModuleScene::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleScene::UpdateGo(GameObject* goToUpdate)
{

	goToUpdate->Update();

    for (int i = 0; i < goToUpdate->children.size(); i++)
	{
		UpdateGo(goToUpdate->children[i]);
    }

}


bool ModuleScene::CleanUp()
{
	return false;
}
