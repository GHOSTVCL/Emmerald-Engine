#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "MathGeoLib/include/Math/Quat.h"
#include "Camera.h"
#include "Transform.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	Position = float3(0.0f, 10.0f, 5.0f);
	Reference = float3(0.0f, 0.0f, 0.0f);
	ViewMatrix = IdentityMatrix;
	Position = float3(10, 5, 10);
	float mousez = 60.0f;

	LookAt(float3(0, 0, 0));
	CalculateViewMatrix();
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

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos(0,0,0);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) // Right-click is held down
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			newPos -= Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			newPos += Z * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			newPos -= X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			newPos += X * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
	{
		Position = float3(10, 5, 10);
		LookAt(float3(0, 0, 0));
	}

	if(App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;


	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.5f * dt;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			float3 rotationAxis(0.0f, 1.0f, 0.0f);
			Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

			X = rotationQuat * X;
			Y = rotationQuat * Y;
			Z = rotationQuat * Z;
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

			Y = rotationQuat * Y;
			Z = rotationQuat * Z;

			if(Y.y < 0.0f)
			{
				Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = Z.Cross(X);
			}
		}

		Position = Reference + Z ;
		
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.5f * dt;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				float3 rotationAxis(0.0f, 1.0f, 0.0f);
				Quat rotationQuat = Quat::RotateAxisAngle(rotationAxis, DeltaX);

				X = rotationQuat * X;
				Y = rotationQuat * Y;
				Z = rotationQuat * Z;
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Quat rotationQuat = Quat::RotateAxisAngle(X, DeltaY);

				Y = rotationQuat * Y;
				Z = rotationQuat * Z;

				if (Y.y < 0.0f)
				{
					Z = float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = Z.Cross(X);
				}
			}
			Position = Reference + Z * Position.Length();
			LookAt(float3(0, 0, 0));
		}
		
	}

	LookAt(Reference);

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3&Position, const float3&Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;

	Z = (Position - Reference).Normalized();
	X = (float3(0.0f, 1.0f, 0.0f).Cross(Z)).Normalized();
	Y = Z.Cross(X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const float3&Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.M;
}

float ModuleCamera3D::Scrollzoom()
{
	mousez -= (float)App->input->GetMouseZ();
	if (mousez < 1.0f)
		mousez = 1.0f;
	if (mousez > 45.0f)
		mousez = 45.0f;

	return mousez;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot(Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
}
//ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
//{
//
//}
//
//ModuleCamera3D::~ModuleCamera3D()
//{}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::Start()
//{
//	LOG("Setting up the camera");
//	bool ret = true;
//
//	sceneCam = new CCamera();
//	sceneCam->FrustumCam.pos = float3(0, 2, -10);
//
//	return ret;
//}
//
//// -----------------------------------------------------------------
//bool ModuleCamera3D::CleanUp()
//{
//	LOG("Cleaning camera");
//
//	delete sceneCam;
//	return true;
//}
//
//// -----------------------------------------------------------------
//update_status ModuleCamera3D::Update(float dt)
//{
//	int wheel = -App->input->GetMouseZ();
//	float speed = 8.0f * dt;
//
//	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
//		speed = 8.0f * 2 * dt;
//
//	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) sceneCam->FrustumCam.pos.y += speed;
//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) sceneCam->FrustumCam.pos.y -= speed;
//
//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * speed;
//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.front * speed;
//
//
//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) sceneCam->FrustumCam.pos -= sceneCam->FrustumCam.WorldRight() * speed;
//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.WorldRight() * speed;
//
//	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) Rotation();
//
//	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
//	{
//		if (App->hierarchy->objSelected != nullptr) {
//			float3 target = App->hierarchy->objSelected->transform->position;
//			sceneCam->LookAt(target);
//
//			float TargetDist = sceneCam->FrustumCam.pos.Distance(target);
//
//			Rotation();
//
//			sceneCam->FrustumCam.pos = target + (sceneCam->FrustumCam.front * -TargetDist);
//		}
//	}
//
//	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
//	{
//		if (App->hierarchy->objSelected != nullptr) {
//			float3 target = App->hierarchy->objSelected->transform->position;
//
//			sceneCam->LookAt(target);
//		}
//	}
//
//	if (wheel != 0) sceneCam->FrustumCam.pos += sceneCam->FrustumCam.front * 10 * -wheel;
//
//
//	return UPDATE_CONTINUE;
//}
//
//void ModuleCamera3D::Rotation()
//{
//	int dx = -App->input->GetMouseXMotion();
//	int dy = -App->input->GetMouseYMotion();
//
//	Quat dir;
//	sceneCam->FrustumCam.WorldMatrix().Decompose(float3(), dir, float3());
//
//	if (dy != 0) {
//		float DeltaY = (float)dy * mousez;
//
//		Quat Y = Quat::identity;
//		Y.SetFromAxisAngle(float3(1.0f, 0.0f, 0.0f), DeltaY * DEGTORAD);
//
//		dir = dir * Y;
//	}
//
//	if (dx != 0) {
//		float DeltaX = (float)dx * mousez;
//
//		Quat X = Quat::identity;
//		X.SetFromAxisAngle(float3(0.0f, 1.0f, 0.0f), DeltaX * DEGTORAD);
//
//		dir = X * dir;
//	}
//
//	float4x4 matrix = sceneCam->FrustumCam.WorldMatrix();
//	matrix.SetRotatePart(dir.Normalized());
//	sceneCam->FrustumCam.SetWorldMatrix(matrix.Float3x4Part());
//}
//
