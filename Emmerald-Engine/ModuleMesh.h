#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/glew.h"
#include <vector>
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64
struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};
struct MeshData {

	std::vector<unsigned int> indices;
	std::vector<Vertex> ourVertex;
	unsigned int VBO = 0, EBO = 0;
	void Draw(GLuint checkers);
	void InitBuffers();
	GLuint textid;

};


class ModuleMesh : public Module
{
public:
	ModuleMesh(Application* app, bool start_enabled = true);

	std::vector<MeshData> LoadMesh(const char* file_path);

	~ModuleMesh();
	update_status Update();
	bool CleanUp();
	void Draw();
	std::vector<MeshData> ourMeshes;

};






#endif //MODULE_IMPORTER
