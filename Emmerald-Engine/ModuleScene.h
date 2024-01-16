#pragma once
#include "Module.h"
#include "GameObject.h"
#include "CompMesh.h"
#include "TextureImporter.h"
#include <vector>

class ModuleScene : public Module
{
public:

	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene() {}
	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	void UpdateGo(GameObject* goToUpdate);
	void DrawAABB(GameObject* goToDraw);
	bool CleanUp();
	void castFirework();
	GameObject* selectedGO;
	GameObject* root;
	GameObject* Chimney1;
	GameObject* Chimney2;
	GameObject* Chimney3;

	std::vector<GameObject*> fireWorkVector;
	Texture* tex1;
	Texture* tex2;
	Texture* tex3;
	Texture* tex4;
	Texture* tex5;
	Texture* tex6;

};
