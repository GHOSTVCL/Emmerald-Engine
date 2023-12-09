#include "WinConsole.h"
#include "ImGUI/imgui.h"
#include "Application.h"

std::string error, warning;
Application* app;
int count = 0;
bool scrolltobottom = false;
bool autoscroll = false;

void WindowConsole::Draw()
{
	app = App->GetInstance();

	std::string name = "Console";
	bool isEnabled = true;
	if (ImGui::Begin(name.c_str(), &isEnabled))
	{
		if (ImGui::Button("Clear"))
		{
			app->logreports.clear();
		}
		ImGui::SameLine();
		if (ImGui::Button("Scroll To Bottom"))
		{
			scrolltobottom = true;
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("AutoScroll", &autoscroll))
		{

		}
		if (ImGui::BeginChild("Output"))
		{
			for (uint i = 0; i < app->logreports.size(); i++)
			{
				if (app->logreports[i].type == Logs::WARNING)
				{
					std::string  warning_complete = warning + app->logreports.data()[i].message;
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), warning_complete.c_str());
				}
				else if (app->logreports[i].type == Logs::ERROR_LOG)
				{
					std::string  error_complete = error + app->logreports.data()[i].message;
					ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), error_complete.c_str());
				}
				else
				{
					ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), app->logreports.data()[i].message.c_str());
				}

			}
			if (scrolltobottom)
			{
				ImGui::SetScrollHereY(1.0f);
				scrolltobottom = false;
			}
			if (autoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			{
				ImGui::SetScrollHereY(1.0f);
			}

		}
		ImGui::EndChild();

	}
	ImGui::End();
}


