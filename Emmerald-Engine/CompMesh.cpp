#include "CompMesh.h"
#include "ImGui/imgui.h"

CompMesh::CompMesh(GameObject* _go) : Component(_go), mesh() {

	this->name = "Mesh_Component";
	this->type = COMP_TYPE::MESH;
	mesh = nullptr;
}

CompMesh::~CompMesh()
{
	delete(&mesh);
}

void CompMesh::SetMesh(MeshData* _mesh)
{

	this->mesh = _mesh;

}

void CompMesh::ShowCompUI()
{
	if (ImGui::CollapsingHeader("Mesh Data: ", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (mesh != nullptr)
		{
			ImGui::Checkbox("Print Checkers ", &mesh->printcheckers);
			ImGui::Checkbox("Print Vertex Normals ", &mesh->printVertexNormals);
			ImGui::Checkbox("Print Face Normals ", &mesh->printFaceNormals);

			//ImGui::Image((ImTextureID)_mesh->textureID, ImVec2(128, 128));
			std::string tempvertices = std::to_string(mesh->ourVertex.size());
			std::string tempindices = std::to_string(mesh->indices.size());
			ImGui::Text("Vertices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), tempvertices.c_str());
			ImGui::Text("Indices: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), tempindices.c_str());
			ImGui::Spacing();
			ImGui::Text("Path: ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), path.c_str());
			ImGui::Spacing();

		}
	}


}

