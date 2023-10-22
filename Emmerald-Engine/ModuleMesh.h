#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleMesh : public Module
{
public:
	ModuleMesh(Application* app, bool start_enabled = true);

	void LoadMesh(const char* file_path);

	~ModuleMesh();
	update_status Update();
	bool CleanUp();
	struct TextCoords {
		float x = 0, y = 0;
	};
	struct Normals {
		float x = 0, y = 0, z = 0;
	};

	struct MeshData {
		unsigned int id_index = 0; // index in VRAM
		unsigned int num_index = 0;
		unsigned int* index = nullptr;
		unsigned int id_vertex = 0; // unique vertex in VRAM
		unsigned int num_vertex = 0;
		float* vertex = nullptr;
		std::vector<TextCoords> textCoords;
		std::vector<Normals> normals;
		unsigned int VBO = 0, EBO = 0;
	};
	std::vector<MeshData> ourMeshes;


};






#endif //MODULE_IMPORTER
