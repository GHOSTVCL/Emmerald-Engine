#include "ModuleMesh.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleMesh::ModuleMesh(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

void ModuleMesh::LoadMesh(const char* file_path)
{

	const aiScene* scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) {

			MeshData temp;
			temp.num_vertex = scene->mMeshes[i]->mNumVertices;
			temp.vertex = new float[temp.num_vertex * 3];
			memcpy(temp.vertex, scene->mMeshes[i]->mVertices, sizeof(float) * temp.num_vertex * 3);

			LOG("New mesh with %d vertices", temp.num_vertex);
			
			for (unsigned int o = 0; o < scene->mMeshes[i]->mNumVertices; o++)
			{
				Normals temp2;
				vec2 vec;

				if (scene->mMeshes[i]->HasNormals())
				{
					temp2.x = scene->mMeshes[i]->mNormals[o].x;
					temp2.y = scene->mMeshes[i]->mNormals[o].y;
					temp2.z = scene->mMeshes[i]->mNormals[o].z;
					
				}
				if (scene->mMeshes[i]->HasTextureCoords(0))
				{
					vec.x = scene->mMeshes[i]->mTextureCoords[0][o].x;
					vec.y = scene->mMeshes[i]->mTextureCoords[0][o].y;
					
				}
				else
				{
					vec.x = 0.0f;
					vec.y = 0.0f;
				}
				temp.normals.push_back(temp2);
				temp.textCoords = vec;
			}


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

ModuleMesh::~ModuleMesh()
{
}

update_status Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleMesh::CleanUp()
{
	return true;
}