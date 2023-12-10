#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include "ImGui/imgui.h"
#include "MeshImporter.h"
#include "CompMesh.h"
#include "CompCamera.h"
#include "CompTransform.h"
#include "ModuleHierarchy.h"
#include "ModuleCamera3D.h"
#include "WinScene.h"

#include "Globals.h"
#include "CompTransform.h"
#include "MeshImporter.h"
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#ifdef _DEBUG
#pragma comment (lib, "MathGeoLib/libx86/Debug/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#else
#pragma comment (lib, "MathGeoLib/libx86/Release/MathGeoLib.lib") /* link Microsoft OpenGL lib   */
#endif // _DEBUG


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
}

// Called before render is available

bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	GOtotal = 0;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glewInit();
	}

	MainCamera = new GameObject("Main Camera");
	//GameCamera->name = "Main Camera";
	CCamera* mcam = new CCamera(MainCamera);
	mainCam = mcam;
	MainCamera->components.push_back(mcam);
	MainCamera->GetComponent<CompTransform>()->position = float3(0, 2, -10);
	MainCamera->SetParent(App->scene->root);
	GameCamera = new GameObject("Scene Camera");

	//GameCamera->name = "Main Camera";
	CCamera* cam = new CCamera(GameCamera);
	App->camera->sceneCam = cam;
	GameCamera->components.push_back(cam);
	GameCamera->GetComponent<CompTransform>()->position = float3(0, 2, -10);

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	for (int i = 0; i < CHECKERS_WIDTH; i++) {
		for (int j = 0; j < CHECKERS_HEIGHT; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkersTexture);
	glBindTexture(GL_TEXTURE_2D, checkersTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	Grid.axis = true;
	ilInit();

	Importer::LoadMesh("Assets/Models/BakerHouse.fbx");
	streetToLoad = Importer::LoadMesh("Assets/Models/scene.DAE");
	streetToLoad->GetComponent<CompTransform>()->eulerRotation.x = -90;
	streetToLoad->GetComponent<CompTransform>()->GetGlobalMatrix();
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(App->camera->sceneCam->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->sceneCam->GetViewMatrix());

	glBindFramebuffer(GL_FRAMEBUFFER, App->camera->sceneCam->frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->sceneCam->FrustumCam.pos.x, App->camera->sceneCam->FrustumCam.pos.y, App->camera->sceneCam->FrustumCam.pos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->editor->AddFPS(App->GetDT());

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	//Draw test here
	CPlane plane;
	plane.axis = true;
	plane.Render();

	for (int i = 0; i < compMeshes.size(); i++)
	{
		compMeshes[i]->Draw();
	}
	if (mainCam != nullptr) {
		CCamera* tempCam = mainCam;
		if (tempCam != nullptr) {
			float3 corners[8];
			tempCam->FrustumCam.GetCornerPoints(corners);
			glLineWidth(5.0f);
			DrawBox(corners, float3(1, .2, .4));
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (mainCam != nullptr)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(mainCam->GetProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(mainCam->GetViewMatrix());

		glBindFramebuffer(GL_FRAMEBUFFER, mainCam->frameBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		CPlane plane;
		plane.axis = true;
		plane.Render();

		for (int i = 0; i < compMeshes.size(); i++)
		{
			compMeshes[i]->Draw();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	App->editor->DrawEditor();


	/*for (int i = 0; i < ourMeshes.size(); i++) {

		ourMeshes.at(i)->Draw(checkersTexture);

	}*/
	compMeshes.clear();


	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	delete GameCamera;
	delete MainCamera;

	for (int i = 0; i < ourMeshes.size(); i++) {

		glDeleteBuffers(1, &ourMeshes[i]->VBO);
		glDeleteBuffers(1, &ourMeshes[i]->EBO);
	}
	
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->sceneCam->GetProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void ModuleRenderer3D::SetDepthTest(bool depth)
{
	if (depth)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
}

void ModuleRenderer3D::SetCullFace(bool cull)
{
	if (cull)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}
}

void ModuleRenderer3D::SetLightning(bool lights)
{
	if (lights)
	{
		glDisable(GL_LIGHTING);
	}
	else
	{
		glEnable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetWireframe(bool wireframe)
{
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ModuleRenderer3D::DrawBox(float3* corners, float3 color)
{
	int indices[24] = { 0,2,2,6,6,4,4,0,0,1,1,3,3,2,4,5,6,7,5,7,3,7,1,5 };
	glBegin(GL_LINES);
	glColor3fv(color.ptr());
	for (size_t i = 0; i < 24; i++)
	{
		glVertex3fv(corners[indices[i]].ptr());
	}
	glColor3f(255.f, 255.f, 255.f);
	glEnd();
	glLineWidth(1.0f);


}

CompMesh* ModuleRenderer3D::RayIntersects(LineSegment& line)
{
	std::vector<CompMesh*> meshIntersectedbyAABB;
	for (uint i = 0; i < compMeshes.size(); i++)
	{
		AABB tempabb = compMeshes[i]->mesh->aABB;

		if (line.Intersects(tempabb))
		{
			meshIntersectedbyAABB.push_back(compMeshes[i]);
		}
	}

	if (meshIntersectedbyAABB.size() == 0)
	{
		App->scene->selectedGO = nullptr;
		return nullptr;
	}

	for (int j = 0; j < meshIntersectedbyAABB.size(); j++)
	{
		//New variable to save the ray in local coordinates
		LineSegment rayinlocalspace = line;

		float4x4 meshglobalmatrix = meshIntersectedbyAABB[j]->comp_owner->GetComponent<CompTransform>()->GetGlobalMatrix();

		float4x4 meshlocalmatrix = meshglobalmatrix.Inverted();

		//multiply ray's position by the inverted global matrix of the mesh in order to convert ray in global position to mesh's local space
		rayinlocalspace.Transform(meshlocalmatrix);

		MeshData* tempmesh = meshIntersectedbyAABB[j]->GetMesh();

		for (uint k = 0; k < tempmesh->indices.size(); k += 3)
		{
			float intersectlength = 0;
			//Creating vertices from the vertices of the mesh at the index of the indices
			float3 vertex1 = tempmesh->ourVertex[tempmesh->indices[k]].Position;
			float3 vertex2 = tempmesh->ourVertex[tempmesh->indices[k+1]].Position;
			float3 vertex3 = tempmesh->ourVertex[tempmesh->indices[k+2]].Position;

			//Create the triangle using the 3 vertices previously created
			Triangle triIntersects(vertex1, vertex2, vertex3);


			if (rayinlocalspace.Intersects(triIntersects, &intersectlength, nullptr))
			{
				trihitsdistmap[intersectlength] = meshIntersectedbyAABB[j];
			}
		}
	}
	meshIntersectedbyAABB.clear();

	if (trihitsdistmap.size() == 0)
	{
		return nullptr;
	}
	else
	{
		return trihitsdistmap.begin()->second;
	}

}
