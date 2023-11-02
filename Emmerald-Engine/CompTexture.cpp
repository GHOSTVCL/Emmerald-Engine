#include "CompTexture.h"
#include "ImGui/imgui.h"

CompTexture::CompTexture(GameObject* _go) : Component(_go), texture(nullptr) {

	this->name = "Texture_Component";
	this->type = COMP_TYPE::TEXTURE;

}

CompTexture::~CompTexture()
{
	delete(texture);
}

void CompTexture::SetTexture(Texture* _texture)
{

	this->texture = _texture;

}

void CompTexture::ShowCompUI()
{
	if (ImGui::CollapsingHeader("Texture Data: ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (texture != nullptr)
		{
			
			ImGui::Image((void*)(intptr_t)texture->textID, ImVec2(128, 128));
			std::string width = std::to_string(texture->width);
			std::string height = std::to_string(texture->height);
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), width.c_str());
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), height.c_str());
			ImGui::Spacing();
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), path.c_str());
			ImGui::Spacing();

		}
	}


}
