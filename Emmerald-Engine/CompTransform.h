#ifndef __CompTransform_H__
#define __CompTransform_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"


class CompTransform :
    public Component
{
public:
    CompTransform(GameObject* _go);

    ~CompTransform();

    void Update() override;

    void OnUIController() override;

    float4x4 GetLocalMatrix();

    float4x4 GetGlobalMatrix();

    float4x4 GetidentityMatrix();

    float3 GetGlobalScale();

    void SetNewRotation(Quat rot);

    void SetNewTransform(float3 pos, float3x3 rot, float3 scale);

public:
    float4x4 globalTransform;
    float4x4 localTransform;
    float4x4 identity;

    float3 position, localScale, eulerRotation;
    Quat rotation;
};

#endif // !__CompTransform_H__