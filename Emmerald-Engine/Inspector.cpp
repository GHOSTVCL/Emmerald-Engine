#include "Inspector.h"
#include "ImGui/imgui.h"
#include "GameObject.h"

Inspector::Inspector() : inspectedObject(nullptr)
{
	name = "Inspector";
}

Inspector::~Inspector()
{
}

void Inspector::Start()
{
}

void Inspector::Draw()
{
	if (isEnabled)
	{
		ImGui::Begin(name.c_str(), &isEnabled);
		if (inspectedObject != nullptr)
		{

			ImGui::Checkbox("Set as Transparent", &inspectedObject->transparent);

			for (auto comp : inspectedObject->components)
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
		ImGui::End();
	}
}