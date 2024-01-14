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
		if (App->scene->selectedGO->components.size() != 0)
			for each (Component * comp in App->scene->selectedGO->components)
			{
				comp->ShowCompUI();
			}

			ImGui::Separator();
			ImGui::Spacing();

		if (ImGui::BeginCombo("Add Component", "Select Component", ImGuiComboFlags_PopupAlignLeft))
		{
			std::string items[3] = { "Camera","BillBoard","Particle System" };
			for (int i = 0; i < 3; i++)
			{
				if (ImGui::Selectable(items[i].c_str()))
				{
					switch (i)
					{
					case 0:
						App->scene->selectedGO->AddComponent(COMP_TYPE::CAMERA);
						break;
					case 1:
						App->scene->selectedGO->AddComponent(COMP_TYPE::BILLBOARD);
						break;
					case 2:
						App->scene->selectedGO->AddComponent(COMP_TYPE::PARTICLES);
						break;
					default:
						break;
					}
				}
			}
			ImGui::EndCombo();

		}

	}
	ImGui::End();
}