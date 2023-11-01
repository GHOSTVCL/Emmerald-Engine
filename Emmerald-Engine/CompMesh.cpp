#include "CompMesh.h"

CompMesh::CompMesh(GameObject* _go) : Component(_go), mesh(nullptr) {

	this->name = "Mesh_Component";
	this->type = COMP_TYPE::MESH;

}

CompMesh::~CompMesh()
{
	delete(mesh);
}

void CompMesh::SetMesh(ModuleMesh::MeshData* _mesh)
{

	this->mesh = _mesh;

}

