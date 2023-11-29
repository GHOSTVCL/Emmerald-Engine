#include "CompCamera.h"
#include "Application.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "ModuleWindow.h"


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
	App->camera->AddCamera();
	App->window->GetScreenSize(&width_, &height_);
	camera_->SetUpFrameBuffer(width_,height_);

}

void CompCamera::Update()
{

}

void CompCamera::OnDisable()
{

}

void CompCamera::ShowCompUI()
{

}
