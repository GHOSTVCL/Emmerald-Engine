#pragma once
#include "Module.h"
#include "GameObject.h"
#include "CompMesh.h"
class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene() {}
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void UpdateGo(GameObject* goToUpdate);
	void DrawMesh(GameObject* goToUpdate);

	bool CleanUp();
	GameObject* selectedGO;
	GameObject* root;

};