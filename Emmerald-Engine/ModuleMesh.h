#pragma once
#include "glmath.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "Glew/include/glew.h"
#include "ModuleTexture.h"
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
	Texture* textid = nullptr;
	bool printcheckers = false;
	bool printFaceNormals = false;
	bool printVertexNormals = false;

};
namespace Importer {

	void LoadMesh(const char* file_path);
	void DeleteMesh(MeshData* mesh2delete);	

};

