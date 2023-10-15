#include "ModuleImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	const char* file_path = "../Assets/Models/BakerHouse.fbx";

	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate|aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) {

			Vdata temp;
			temp.num_vertex = scene->mMeshes[i]->mNumVertices;
			temp.vertex = new float[temp.num_vertex * 3];
			memcpy(temp.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * temp.num_vertex * 3);
			

			LOG("New mesh with %d vertices", temp.num_vertex);

			if (scene->mMeshes[i]->HasFaces())
			{
				temp.num_index = scene->mMeshes[i]->mNumFaces * 3;
				
				temp.index = new unsigned int[temp.num_index]; // assume each face is a triangle

				
				for (uint y = 0; y < scene->mMeshes[i]->mNumFaces; y++)
				{
					if (scene->mMeshes[i]->mFaces[y].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {

						memcpy(&temp.index[y * 3], scene->mMeshes[i]->mFaces[y].mIndices, 3 * sizeof(unsigned int));

					}
				}
			}
			ourMeshes.push_back(temp);

		}
		aiReleaseImport(scene);

	}
	else
		LOG("Error loading scene % s", file_path);

}

ModuleImporter::~ModuleImporter()
{
}

update_status Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleImporter::CleanUp()
{
	return true;
}