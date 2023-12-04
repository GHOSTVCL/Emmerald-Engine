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
	camera_->CameraFrustrum.pos = comp_owner->GetComponent<CompTransform>()->position;
	camera_->CameraFrustrum.front = comp_owner->GetComponent<CompTransform>()->GetGlobalMatrix().RotatePart().Col(2);
	camera_->CameraFrustrum.up = comp_owner->GetComponent<CompTransform>()->GetGlobalMatrix().RotatePart().Col(1);
}

void CompCamera::OnDisable()
{
	App->camera->DestroyCamera(camera_);
}

void CompCamera::ShowCompUI()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{

		if (ImGui::SliderFloat("FOV", &camera_->FieldOfView, 55.0f, 110.0f, "%.2f"))
		{
			camera_->CameraFrustrum.verticalFov = math::DegToRad(camera_->FieldOfView);
			camera_->CameraFrustrum.horizontalFov = 2.0f * atanf(tanf(camera_->CameraFrustrum.verticalFov / 2.0f) * camera_->aspectratio);
		}

		ImGui::DragFloat("Near plane Distance ", &camera_->CameraFrustrum.nearPlaneDistance, 0.1f, 0.01);

		ImGui::DragFloat("Far plane Distance ", &camera_->CameraFrustrum.farPlaneDistance, 0.5f, 0.03);

		if (ImGui::Button("Set as Game Camera "))
		{
			std::string phrase = comp_owner->name + " Camera set as Game Camera";
			this->camera_->SetAsGameCamera();
		}

	}
}
