#include "ModuleHierarchy.h"
#include "ImGUI/imgui.h"
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"
#include "Globals.h"

ModuleHierarchy::ModuleHierarchy(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Hierarchy";
}

ModuleHierarchy::~ModuleHierarchy()
{
}

bool ModuleHierarchy::Start()
{
	bool ret = true;
	return ret;
}

void ModuleHierarchy::Draw()
{
	ImVec2 windowSize = ImGui::GetWindowSize();
	//Window
	if (isEnabled)
	{

		if (ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			ImGui::SetWindowPos(ImVec2(0, 18));
			ImGui::SetWindowSize(ImVec2(windowSize.x * 0.7f, windowSize.y * 2.5f));
			ShowGameObjects(App->scene->root);

			PrimitivesMenu();
		}
		ImGui::End();
	}
}

void ModuleHierarchy::PrimitivesMenu()
{

	if (ImGui::BeginMenu("Empty GameObjects"))
	{
		if (ImGui::MenuItem("Empty"))
		{
			GameObject* empty = new GameObject("Empty" + std::to_string(emptycounter));
			empty->SetParent(App->scene->root);
			emptycounter++;

		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Primitives"))
	{
		if (ImGui::MenuItem("Cube"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Cube.fbx");
		}
		if (ImGui::MenuItem("Sphere"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Sphere.fbx");
		}
		if (ImGui::MenuItem("Monkey"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Monkey.fbx");
		}
		if (ImGui::MenuItem("Plane"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Plane.fbx");
		}
		if (ImGui::MenuItem("Pyramid"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Pyramid.fbx");
		}
		if (ImGui::MenuItem("Cylinder"))
		{
			App->importer->LoadMesh("../Assets/Models/Primitives/Cylinder.fbx");
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Delete GameObjects"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			if (App->scene->selectedGO != nullptr)
			{
				App->scene->selectedGO->readytobedeleted = true;
				//app->AddLog(Logs::WARNING, "Still On Development");
			}
			else
			{
				/*(Logs::WARNING, "Select the gameobject that you would like to delete.");*/
			}

		}
		ImGui::EndMenu();
	}

}

void ModuleHierarchy::ShowGameObjects(GameObject* go)
{

	ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_None;
	nodeFlags += ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnArrow;

	if (go->children.size() == 0)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
	}

	if (go == App->scene->selectedGO)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	};

	bool nodeOpen = ImGui::TreeNodeEx(go->name.c_str(), nodeFlags);
	if (go->children.size() == 0)nodeOpen = false;

	if (ImGui::IsItemClicked(0))
	{
		if (go->parent != nullptr)
		{
			App->scene->selectedGO = go;
		}
	}

	if (go->parent != nullptr)
	{
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("GameObject", go, sizeof(GameObject*));

			goToDrop = go;

			ImGui::Text("Where do you want to drop this in the hierarchy ? ");

			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
		{
			//Not working as intended
			GameObject* temp = (GameObject*)payload->Data;

			goToDrop->SetParent(go);

			goToDrop = nullptr;
		}

		ImGui::EndDragDropTarget();
	}

	if (nodeOpen && go->children.size() != 0)
	{

		for (int i = 0; i < go->children.size(); i++)
		{
			ShowGameObjects(go->children[i]);
		}

		if (go->children.size() != 0)
		{
			ImGui::TreePop();
		}
	}


}
