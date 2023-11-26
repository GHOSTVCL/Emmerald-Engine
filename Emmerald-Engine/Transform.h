#pragma once
#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ImGUI/imgui.h"
//#include "Mesh.h"

class GameObject;
class Component;
//struct Mesh;

class Transform : public Component
{
public:

	Transform();
	Transform(GameObject* comp_owner);
	~Transform();

	float4x4 lTransform;

	float3 position, scale, rotation;

	void SetTransformMatrix();
	void SetTransform(float4x4 matrix);
	void Inspector();



	float4x4 GetTransformMatrix();

	//void BoxMesh();

	//Mesh* mesh;
};
