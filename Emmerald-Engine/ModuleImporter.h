#ifndef MODULE_IMPORTER
#define MODULE_IMPORTER

#include "Module.h"
#include "Globals.h"

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);

	~ModuleImporter();

	bool Init() override;
	bool CleanUp();


	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;
	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

};






#endif //MODULE_IMPORTER
