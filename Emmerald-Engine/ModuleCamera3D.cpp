#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "CompCamera.h"
#include "CompTransform.h"



ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	delete sceneCam;
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	int wheel = -App->input->GetMouseZ();
	float speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * 2 * dt;

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) sceneCam->FrustumCam.pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) sceneCam->FrustumCam.pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.front * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.WorldRight() * speed;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) Rotation();

	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->scene->selectedGO != nullptr) {

			float3 target = FindTargetRotation(App->scene->selectedGO);

	

			sceneCam->LookAt(target);

			float TargetDist = sceneCam->FrustumCam.pos.Distance(target);

			Rotation();

			sceneCam->FrustumCam.pos = target + (sceneCam->FrustumCam.front * -TargetDist);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (App->hierarchy->objSelected != nullptr) {
			float3 target = App->hierarchy->objSelected->transform_->position;

			sceneCam->LookAt(target);
		}
	}

	if (wheel != 0) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * 10 * -wheel;

	

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Rotation()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	Quat dir;
	sceneCam->FrustumCam.WorldMatrix().Decompose(float3(), dir, float3());

	if (dy != 0) {
		float DeltaY = (float)dy * mouseSens;

		Quat Y = Quat::identity;
		Y.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), DeltaY * DEGTORAD);

		dir = dir * Y;
	}

	if (dx != 0) {
		float DeltaX = (float)dx * mouseSens;

		Quat X = Quat::identity;
		X.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);

		dir = X * dir;
	}

	float4x4 matrix = sceneCam->FrustumCam.WorldMatrix();
	matrix.SetRotatePart(dir.Normalized());
	sceneCam->FrustumCam.SetWorldMatrix(matrix.Float3x4Part());
}


float3 ModuleCamera3D::FindTargetRotation(GameObject* tempGO)
{
	float3 target;

	target = tempGO->GetComponent<CompTransform>()->position;
	if (tempGO->parent != nullptr && tempGO->parent->name != "root") {
		target = FindTargetRotation(tempGO->parent) + target;
	}
	return target;
}

void ModuleCamera3D::CreateGameCamera()
{
	Camera = new GameObject("Camera");
	//GameCamera->name = "Main Camera";
	CCamera* cam_ = new CCamera(Camera);
	App->renderer3D->mainCam = cam_;
	Camera->components.push_back(cam_);
	Camera->GetComponent<CompTransform>()->position = float3(0, 2, -10);
	Camera->SetParent(App->scene->root);

}


