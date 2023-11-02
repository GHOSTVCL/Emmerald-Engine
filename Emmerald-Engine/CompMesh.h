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

    void ShowCompUI() override;

public:
    MeshData* mesh;
    Application* app;
    std::string path;
private:


};

#endif // !__COMPONENT_MESH__