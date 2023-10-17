#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include "ImGui/imgui.h"
#include "ModuleMesh.h"
#include "ModuleTexture.h"

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
{}

// Called before render is available

bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

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

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Grid.axis = true;
	BindVBO();

	


	return ret;
}

void ModuleRenderer3D::BindVBO()
{
	for (int i = 0; i < App->importer->ourMeshes.size(); i++) {

		glGenBuffers(1, &App->importer->ourMeshes[i].VBO);
		glBindBuffer(GL_ARRAY_BUFFER, App->importer->ourMeshes[i].VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * App->importer->ourMeshes[i].num_vertex * 3, App->importer->ourMeshes[i].vertex, GL_STATIC_DRAW);

		glGenBuffers(1, &App->importer->ourMeshes[i].EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->importer->ourMeshes[i].EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * App->importer->ourMeshes[i].num_index, App->importer->ourMeshes[i].index, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->editor->AddFPS(App->GetDT());

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	Grid.Render();
	//Draw test here
	for (int i = 0; i < App->importer->ourMeshes.size(); i++) {
		
		glBindBuffer(GL_ARRAY_BUFFER, App->importer->ourMeshes[i].VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, App->importer->ourMeshes[i].EBO);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, App->importer->ourMeshes[i].num_index, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, App->importer->ourMeshes[i].id_vertex);
		glVertexPointer(3, GL_FLOAT, sizeof(float) * 5, NULL);
		glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	}

	App->editor->DrawEditor();

	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	for (int i = 0; i < App->importer->ourMeshes.size(); i++) {

		glDeleteBuffers(1, &App->importer->ourMeshes[i].VBO);
		glDeleteBuffers(1, &App->importer->ourMeshes[i].EBO);
	}
	
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//todo: USE MATHGEOLIB here BEFORE 1st delivery! (TIP: Use MathGeoLib/Geometry/Frustum.h, view and projection matrices are managed internally.)
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(ProjectionMatrix.M);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::RendererMenu()
{
	if (ImGui::BeginMenu("Renderer")) {

		if (ImGui::Checkbox("Depth Test", &dtest))
			(dtest) ? glDisable(GL_DEPTH_TEST) : glEnable(GL_DEPTH_TEST);

		if (ImGui::Checkbox("Cull Face", &cface))
			(cface) ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);

		if (ImGui::Checkbox("Lighting", &lighting))
			(lighting) ? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);

		if (ImGui::Checkbox("Color Material", &cmaterial))
			(cmaterial) ? glDisable(GL_COLOR_MATERIAL) : glEnable(GL_COLOR_MATERIAL);

		if (ImGui::Checkbox("Texture 2D", &txt2d))
			(txt2d) ? glDisable(GL_TEXTURE_2D) : glEnable(GL_TEXTURE_2D);

		if (ImGui::Checkbox("Wireframe", &wframe))
			(wframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		ImGui::EndMenu();
	}
}
