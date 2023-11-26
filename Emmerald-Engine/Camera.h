#pragma once
#include "Globals.h"
#include "Component.h"
#include "ImGUI/imgui.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Component;

class CCamera : public Component
{
public:

	CCamera();
	CCamera(GameObject* comp_owner);
	~CCamera();

	void SetCam();
	void GenBuffer();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void LookAt(const float3& target);

	void TransformCam();

	void Inspector();

	bool ContainsAaBox(MeshData* refBox);

	Frustum FrustumCam;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	int printCount;

	int FOV = 60.0f;

	unsigned int cameraBuffer;
	unsigned int frameBuffer;

private:

	unsigned int renderObjBuffer;
};
