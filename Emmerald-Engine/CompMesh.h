#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "Component.h"
#include "ModuleMesh.h"
#include <vector>

class CompMesh :
    public Component
{
public:

    CompMesh(GameObject* _go);

    ~CompMesh();

    void SetMesh(std::vector <MeshData> _meshes);

    std::vector <MeshData> GetMesh() { return meshes; }


public:
    std::vector <MeshData> meshes;
    Application* app;

private:


};

#endif // !__COMPONENT_MESH__