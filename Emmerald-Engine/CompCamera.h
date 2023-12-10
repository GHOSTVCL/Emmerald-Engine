#pragma once
#include "Globals.h"
#include "Component.h"


class GameObject;
class Component;

class CCamera : public Component
{
public:

	CCamera();
	CCamera(GameObject* owner);
	~CCamera();

	void SetCam();
	void GenBuffer();

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void LookAt(const float3& target);

	void TransformCam();

	void ShowCompUI() override;
	void Update() override;
	bool ContainsAaBox(MeshData* refBox);
	float3 TranslateePoint(const float3& point, const float4x4& matrix);
	bool FrustrumContainsBB(AABB& globalBB);
	Frustum FrustumCam;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;

	int FOV = 60.0f;

	unsigned int cameraBuffer;
	GLuint frameBuffer;

private:

	unsigned int renderObjBuffer;
};

