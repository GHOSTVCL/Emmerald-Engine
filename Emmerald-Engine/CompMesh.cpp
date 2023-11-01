#include "CompMesh.h"

CompMesh::CompMesh(GameObject* _go) : Component(_go), meshes(NULL) {

	this->name = "Mesh_Component";
	this->type = COMP_TYPE::MESH;

}

CompMesh::~CompMesh()
{
	delete(&meshes);
}

void CompMesh::SetMesh(std::vector <MeshData> _meshes)
{

	this->meshes = _meshes;

}

