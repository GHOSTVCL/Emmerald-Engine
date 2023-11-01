#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};
struct MeshData {
	std::vector<unsigned int> indices;
	std::vector<Vertex> ourVertex;
	unsigned int VBO = 0, EBO = 0;
};

class ModuleMesh : public Module
{
public:
	ModuleMesh(Application* app, bool start_enabled = true);

	std::vector<MeshData> LoadMesh(const char* file_path);

	~ModuleMesh();
	update_status Update();
	bool CleanUp();

	std::vector<MeshData> ourMeshes;

};






#endif //MODULE_IMPORTER
