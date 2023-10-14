#include "ModuleImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
}

update_status Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::Init()
{
	bool ret = true;


	return ret;
}


bool ModuleImporter::CleanUp()
{
	return true;
}