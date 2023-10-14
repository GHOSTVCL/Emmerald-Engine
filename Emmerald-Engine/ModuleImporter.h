#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Module.h"
#include "Globals.h"
#include <vector>

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);

	~ModuleImporter();
	update_status Update();
	bool CleanUp();
	struct Vdata {
		unsigned int id_index = 0; // index in VRAM
		unsigned int num_index = 0;
		unsigned int* index = nullptr;
		unsigned int id_vertex = 0; // unique vertex in VRAM
		unsigned int num_vertex = 0;
		float* vertex = nullptr;
	};
	std::vector<Vdata> ourMeshes;


};






#endif //MODULE_IMPORTER
