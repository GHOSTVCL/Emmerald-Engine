#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "Camera.h"

//todo: REMOVE this before 1st delivery!!
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt) override;
	void SceneCameraHandleInput();
	void CamMovementInput();
	void CamRotationInput();
	bool CleanUp();

	/*bool SaveSettings(pugi::xml_node& config);*/

	Camera* AddCamera();

	void DestroyCamera(Camera* camToDestroy);


private:

	void CalculateViewMatrix();

public:
	
	//You won't need this after using Frustum
	float3 X, Y, Z, Position, Reference;
	float mousez = 60.0f;

	std::vector<Camera*> gamecams;

	Camera scenecam;

	Camera* cameratobedrawn = nullptr;

	Camera* gamecamactive = nullptr;

private:

	mat4x4 ViewMatrix;
	//Frustum mMainCamera; Some help here :)
};