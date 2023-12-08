#include "WinGame.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "CompMesh.h"
#include "ModuleCamera3D.h"
#include "ModuleCamera3D.h"
#include "CompCamera.h"

void WindowGame::Draw()
{
	std::string name = "Game";
	bool isEnabled = true;
	if (ImGui::Begin(name.c_str(), &isEnabled))
	{
		ImVec2 WindowSize;
		WindowSize = ImGui::GetContentRegionAvail();

		//Prevent image stretching by setting new aspect ratio
		float aspectRatio = WindowSize.x / WindowSize.y;
		App->camera->sceneCam->FrustumCam.verticalFov = App->camera->sceneCam->FOV * DEGTORAD;
		App->camera->sceneCam->FrustumCam.horizontalFov = 2.0f * atanf(tanf(App->camera->sceneCam->FrustumCam.verticalFov / 2.0f) * aspectRatio);

		ImGui::Image((ImTextureID)App->camera->sceneCam->cameraBuffer, WindowSize, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
}
