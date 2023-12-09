#pragma once
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/glew.h"
#include "TextureImporter.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include <vector>
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class GameObject;
struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
};

struct MeshData {

public:
	std::vector<unsigned int> indices;
	std::vector<Vertex> ourVertex;
	unsigned int VBO = 0, EBO = 0;
	void Draw(GLuint checkers, float4x4 matrix);
	void InitBuffers();
	AABB GenLocalAABB();
	AABB GenGlobalBB(GameObject* go);
	Texture* textid = nullptr;
	bool printcheckers = false;
	bool printFaceNormals = false;
	bool printVertexNormals = false;
	bool printAABB = false;
	AABB aABB;

private:

	AABB localAABB;
	OBB oBB;

	

};
namespace Importer {

	void ImportMesh(const char* file_path);
	void DeleteMesh(MeshData* mesh2delete);	
	void LoadMesh(const char* file_path);
	void ProcessNode(const aiScene* scene, aiNode* node, GameObject* GO, const char* file_path);

};

