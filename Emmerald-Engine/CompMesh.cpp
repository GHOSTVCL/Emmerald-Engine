#include "CompMesh.h"

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

