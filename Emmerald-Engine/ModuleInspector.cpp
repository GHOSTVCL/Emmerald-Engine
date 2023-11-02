#include "ModuleInspector.h"
#include "ImGui/imgui.h"
#include "GameObject.h"
#include "Application.h"

ModuleInspector::ModuleInspector(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Inspector";
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
	if (isEnabled)
	{
		ImGui::BeginMenu(name.c_str(), &isEnabled);
		if (inspectedObject != nullptr)
		{

			ImGui::Checkbox("Set as Transparent", &inspectedObject->transparent);

			for each (Component * comp in inspectedObject->components)
			{
				comp->OnUIController();
			}

			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::BeginCombo("Add Component", "Select Component", ImGuiComboFlags_PopupAlignLeft))
			{
				for (int i = 0; i < 3; i++)
				{
					if (ImGui::Selectable(items[i].c_str()))
					{
						switch (i)
						{
						case 0:
							inspectedObject->AddComponent(COMP_TYPE::TRANSFORM);
							break;
						case 1:
							inspectedObject->AddComponent(COMP_TYPE::MESH);
							break;
						case 2:
							inspectedObject->AddComponent(COMP_TYPE::TEXTURE);
							break;
						default:
							break;
						}
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::EndMenu();
	}
}