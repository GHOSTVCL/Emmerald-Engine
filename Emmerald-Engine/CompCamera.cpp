#include "CompCamera.h"
#include "Application.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "ModuleWindow.h"
#include "CompTransform.h"
#include "ImGUI/imgui.h"


CompCamera::CompCamera(GameObject* _go) : Component(_go)
{
	this->name = "Camera_Component";
	this->type = COMP_TYPE::CAMERA;
	
}

CompCamera::~CompCamera()
{
	App->camera->DestroyCamera(camera_);
}

void CompCamera::OnEnable()
{
	camera_ = App->camera->AddCamera();
	App->window->GetScreenSize(&width_, &height_);
	camera_->SetUpFrameBuffer(width_,height_);
	camera_->SetAsGameCamera();

}

void CompCamera::Update()
{

}

void CompCamera::OnDisable()
{
	App->camera->DestroyCamera(camera_);
}

void CompCamera::ShowCompUI()
{
	
}
