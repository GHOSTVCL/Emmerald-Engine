#include "ModuleImporter.h"
#include "Assimp/include/assimp/cimport.h"
#include "Assimp/include/assimp/scene.h"
#include "Assimp/include/assimp/postprocess.h"


#ifdef _DEBUG
#pragma comment (lib, "Assimp/libx86/Debug/assimp-vc142-mtd.lib")
#else
#pragma comment (lib, "Assimp/libx86/Release/assimp-vc142-mtd.lib") 
#endif // _DEBUG

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImporter::~ModuleImporter()
{
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