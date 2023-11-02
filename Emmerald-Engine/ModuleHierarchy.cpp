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
	//Window
	if (isEnabled)
	{
		if (ImGui::Begin(name.c_str(), &isEnabled))
		{

			ShowGameObjects(app->scene->root);

			PrimitivesMenu();
		}
		ImGui::End();
	}
}

void ModuleHierarchy::PrimitivesMenu()
{
	if (!ImGui::IsItemHovered())
	{
		if (ImGui::BeginPopupContextWindow("GameObjects", ImGuiPopupFlags_NoOpenOverExistingPopup | ImGuiPopupFlags_MouseButtonDefault_))
		{
			if (ImGui::BeginMenu("Empty GameObjects"))
			{
				if (ImGui::MenuItem("Empty"))
				{
					GameObject* empty = new GameObject("Empty" + std::to_string(emptycounter));
					empty->SetParent(app->scene->root);
					emptycounter++;

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Primitives"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					app->importer->LoadMesh("Assets/Primitives/Cube.fbx");
				}
				if (ImGui::MenuItem("Sphere"))
				{
					app->importer->LoadMesh("Assets/Primitives/Sphere.fbx");
				}
				if (ImGui::MenuItem("Monkey"))
				{
					app->importer->LoadMesh("Assets/Primitives/Monkey.fbx");
				}
				if (ImGui::MenuItem("Plane"))
				{
					app->importer->LoadMesh("Assets/Primitives/Plane.fbx");
				}
				if (ImGui::MenuItem("Pyramid"))
				{
					app->importer->LoadMesh("Assets/Primitives/Pyramid.fbx");
				}
				if (ImGui::MenuItem("Cylinder"))
				{
					app->importer->LoadMesh("Assets/Primitives/Cylinder.fbx");
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Delete GameObjects"))
			{
				if (ImGui::MenuItem("Delete"))
				{
					if (app->scene->selectedGO != nullptr)
					{
						app->scene->selectedGO->readytobedeleted = true;
						//app->AddLog(Logs::WARNING, "Still On Development");
					}
					else
					{
						/*(Logs::WARNING, "Select the gameobject that you would like to delete.");*/
					}

				}
				ImGui::EndMenu();
			}


			ImGui::EndPopup();

		}
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

	if (go == app->scene->selectedGO)
	{
		nodeFlags |= ImGuiTreeNodeFlags_Selected;
	};

	bool nodeOpen = ImGui::TreeNodeEx(go->name.c_str(), nodeFlags);
	if (go->children.size() == 0)nodeOpen = false;

	if (ImGui::IsItemClicked(0))
	{
		if (go->parent != nullptr)
		{
			app->scene->selectedGO = go;
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
