#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "Component.h"
#include "ModuleMesh.h"

class CompMesh :
    public Component
{
public:

    CompMesh(GameObject* _go);

    ~CompMesh();

    void SetMesh(MeshData* _mesh);

    MeshData* GetMesh() { return mesh; }


public:
    MeshData* mesh;
    Application* app;

private:


};

#endif // !__COMPONENT_MESH__