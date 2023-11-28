#include "WinInspector.h"
#include "ImGui/imgui.h"
#include "GameObject.h"
#include "Application.h"

void Inspector::Draw()
{
	std::string name = "Inspector";
	bool isEnabled = true;
	ImGui::Begin(name.c_str(), &isEnabled);
	if (App->scene->selectedGO != nullptr)
	{
		if(App->scene->selectedGO->components.size()!=0)
		for each (Component * comp in App->scene->selectedGO->components)
		{
			comp->ShowCompUI();
		}

		ImGui::Separator();
		ImGui::Spacing();

	}
	ImGui::End();
	
}