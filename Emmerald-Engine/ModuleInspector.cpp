#include "ModuleInspector.h"
#include "ImGui/imgui.h"
#include "GameObject.h"
#include "Application.h"

ModuleInspector::ModuleInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Inspector";
	inspectedObject = new GameObject("inspected");

}

ModuleInspector::~ModuleInspector()
{
}

bool ModuleInspector::Start()
{
	return true;
}

void ModuleInspector::Draw()
{

	inspectedObject = App->scene->selectedGO;
	ImGui::Begin(name.c_str(), &isEnabled);
	if (inspectedObject != nullptr)
	{

		for each (Component * comp in inspectedObject->components)
		{
			comp->ShowCompUI();
		}

		ImGui::Separator();
		ImGui::Spacing();

	}
	ImGui::End();
	
}