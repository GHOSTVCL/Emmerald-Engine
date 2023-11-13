#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__
#include "Component.h"
#include "ModuleMesh.h"
#include "Application.h"
#include "Module.h"
#include <vector>
class CompMesh : public Component
{
public:

    CompMesh(GameObject* _go);

    ~CompMesh();

    void SetMesh(MeshData* _mesh);

    MeshData* GetMesh() { return mesh; }

    void ShowCompUI() override;

public:
    MeshData* mesh;
    std::vector<MeshData*> _ourMeshes;
    Application* app;
    std::string path;
private:


};

#endif // !__COMPONENT_MESH__