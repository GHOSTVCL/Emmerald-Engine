#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "Globals.h"
#include "Application.h"
#include "MathGeoLib/include/Math/Quat.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleEditor::~ModuleEditor()
{}

// -----------------------------------------------------------------
bool ModuleEditor::Init()
{
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------

void ModuleEditor::DrawEditor() {



}
// -----------------------------------------------------------------


bool ModuleEditor::CleanUp() {

	return true;
}





/*
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO(); (void)io;
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

// Setup Dear ImGui style
ImGui::StyleColorsDark();
//ImGui::StyleColorsLight();

// Setup Platform/Renderer backends
ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
ImGui_ImplOpenGL3_Init(glsl_version);
*/