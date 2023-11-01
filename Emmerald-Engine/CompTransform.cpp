#include "CompTransform.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

CompTransform::CompTransform(GameObject* _go) :Component(_go)
{
	this->type = COMP_TYPE::TRANSFORM;
	localTransform.SetIdentity();
	globalTransform.SetIdentity();
	//Getting Pos,Rot and Scale from the local transform matrix
	localTransform.Decompose(position, rotation, localScale);
	//Transform quaternion to euler angles
	rotation.Normalize();
	eulerRotation = rotation.ToEulerXYZ();

}

CompTransform::~CompTransform()
{
}

void CompTransform::Update()
{


	//GetGlobalMatrix();
}

void CompTransform::OnUIController()
{

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::DragFloat3("Position", &position[0]);
		ImGui::DragFloat3("Rotation", &eulerRotation[0]);
		ImGui::DragFloat3("Scale", &localScale[0]);

	}

}

float4x4 CompTransform::GetLocalMatrix()
{
	float4x4 aux;
	rotation = Quat::FromEulerXYZ(math::DegToRad(eulerRotation.x), math::DegToRad(eulerRotation.y), math::DegToRad(eulerRotation.z));
	rotation.Normalize();

	aux = float4x4::FromTRS(position, rotation, localScale);


	return aux;
}

float4x4 CompTransform::GetGlobalMatrix()
{
	float4x4 aux;
	if (comp_owner->parent == nullptr)
	{
		aux = GetidentityMatrix() * GetLocalMatrix();

	}
	else
	{
		aux = comp_owner->parent->GetComponent<CompTransform>()->GetGlobalMatrix() * GetLocalMatrix();

	}

	return aux;
}

float4x4 CompTransform::GetidentityMatrix()
{
	float4x4 identity;
	identity.SetIdentity();
	return identity;
}

float3 CompTransform::GetGlobalScale()
{
	float3 aux;

	if (comp_owner->parent == nullptr)
	{

		aux = localScale;
		return aux;

	}
	else
	{
		aux = localScale.Mul(comp_owner->parent->GetComponent<CompTransform>()->GetGlobalScale());

		return aux;
	}


}

void CompTransform::SetNewRotation(Quat rot)
{
	rot = rot.Normalized();
	this->eulerRotation = rot.ToEulerXYZ();

}

void CompTransform::SetNewTransform(float3 pos, float3x3 rot, float3 scale)
{

	this->position = pos;
	this->eulerRotation = rot.ToEulerXYZ();
	this->localScale = scale;

}
