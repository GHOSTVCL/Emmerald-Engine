#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "MeshImporter.h"
#include "MathGeoLib/include/Math/float3x3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Primitive.h"
#include "Glew/include/glew.h"
//todo: REMOVE this before 1st delivery!!
#include "glmath.h"
#include "GameObject.h"
#include "TextureImporter.h"
#include "CompMesh.h"
#include <vector>
#include <map>
#define MAX_LIGHTS 8
#define CHECKERS_WIDTH 64
#define CHECKERS_HEIGHT 64

class CCamera;
class ModuleCamera3;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	CompMesh* RayIntersects(LineSegment& line);

	bool CleanUp();

	void OnResize(int width, int height);

	void SetDepthTest(bool depth);
	void SetCullFace(bool cull);
	void SetLightning(bool setlights);
	void SetWireframe(bool wireframe);
	void OnZoom();

	//void AddDebug(/*float3* points*/);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	CPlane Grid;

	GLuint checkersTexture;
	GLuint texWidth, texHeight;
	GLubyte checkerImage[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

	//You won't need this after using Frustum
	mat4x4 ProjectionMatrix;
	std::vector<MeshData*> ourMeshes;
	std::vector<CompMesh*> compMeshes;
	std::map<float, CompMesh*> trihitsdistmap;

	int GOtotal;

	GameObject* GameCamera;
	GameObject* MainCamera;
	
	CCamera* mainCam;
	
	bool txt2d = false;
	
};

