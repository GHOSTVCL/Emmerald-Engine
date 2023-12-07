#include "WinScene.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "CompMesh.h"

void WindowScene::Draw()
{
	std::string name = "SceneWindow";
	bool isEnabled = true;
	if (ImGui::Begin(name.c_str(), &isEnabled))
	{
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (size.x != App->camera->scenecam.framebuffer.GetWidthFrame() || size.y != App->camera->scenecam.framebuffer.GetHeightFrame())
		{
			App->camera->scenecam.SetAspectRatio(size.x, size.y);
		}

		ImGui::Image((ImTextureID)App->camera->scenecam.framebuffer.GetTextureBuffer(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		//if (ImGui::IsWindowHovered())
		//{

		//	if (ImGui::IsMouseClicked(0))
		//	{
		//		//ScreenPos 
		//		ImVec2 mouseposinscreen = ImGui::GetMousePos();
		//		ImVec2 winscenepos = ImGui::GetWindowPos();
		//		//Position in win scene
		//		ImVec2 mouseposinwinscene;
		//		mouseposinwinscene.x = (mouseposinscreen.x - winscenepos.x);
		//		mouseposinwinscene.y = mouseposinscreen.y - (winscenepos.y + ImGui::GetFrameHeight());

		//		//normalize mouse pos in win scene
		//		ImVec2 mousewinscenenorm;
		//		mousewinscenenorm.x = mouseposinwinscene.x / ImGui::GetWindowSize().x;
		//		mousewinscenenorm.y = mouseposinwinscene.y / (ImGui::GetWindowSize().y - ImGui::GetFrameHeight());

		//		//Transform each point into the center of win scene
		//		mousewinscenenorm.x = mousewinscenenorm.x * 2.0f - 1.0f;
		//		mousewinscenenorm.y = mousewinscenenorm.y * -2.0f + 1.0f;
		//		LOG("Position : %f,%f", mousewinscenenorm.x, mousewinscenenorm.y);

		//		LineSegment picking = App->camera->scenecam.CameraFrustrum.UnProjectLineSegment(mousewinscenenorm.x, mousewinscenenorm.y);

		//		CompMesh* meshhit = App->renderer3D->RayIntersects(picking);

		//		if (meshhit != nullptr)
		//		{
		//			App->scene->selectedGO = meshhit->comp_owner;
		//		}
		//		else
		//		{
		//			App->scene->selectedGO = nullptr;
		//		}
		//		App->renderer3D->trihitsdistmap.clear();

		//	}
		//}
	}
	
	ImGui::End();

}
