#pragma once
#include "Component.h"
#include "MathGeoLib/include/Math/MathAll.h"

enum BILLBOARDTYPE
{
    NO_ALIGN,
    SCREENALIGN,
    WORLDALIGN,
    AXISALIGN,
};

class Application;

class CompBillBoarding :
    public Component
{
public:

    CompBillBoarding(GameObject* go);

    ~CompBillBoarding();

    void Update() override;

    void ShowCompUI() override;

    Quat ScreenAlignBBoard();

    Quat WorldAlignBBoard();

    void AxisAlignBBoard();

    Quat GetBBRotation();

private:


    BILLBOARDTYPE typeofBBoard;

    std::string BBtype[4] = { "Screen Align","World Align","Axis Align","No Align" };

    bool staticYAxisBillboard;

    float3 zBBoardAxis;
    float3 yBBoardAxis;
    float3 xBBoardAxis;

    Quat rotation;

};

