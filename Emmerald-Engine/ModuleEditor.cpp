#include "ModuleEditor.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/backends/imgui_impl_sdl2.h"
#include "SDL/include/SDL.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "ModuleHierarchy.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init();

    mFPSLog.reserve(30);
    setbrightness = 1.f;
    winWidth = SCREEN_WIDTH;
    winHeight = SCREEN_HEIGHT;
    return true;
}

void ModuleEditor::DrawEditor()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();


    DisplayMainMenuBar();
    App->hierarchy->Draw();
    App->inspector->Draw();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool ModuleEditor::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

void ModuleEditor::AddFPS(const float aFPS)
{
    if (mFPSLog.size() < 30)
    {
        mFPSLog.push_back(aFPS);
    }
    else
    {
        for (unsigned int i = 0; i < mFPSLog.size(); i++)
        {
            if (i + 1 < mFPSLog.size())
            {
                float iCopy = mFPSLog[i + 1];
                mFPSLog[i] = iCopy;
            }
        }
        mFPSLog[mFPSLog.capacity() - 1] = aFPS;
    }
}

void ModuleEditor::DisplayHelp()
{
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::BeginMenu("About"))
        {
            ImGui::Text("Emmerald Engine");
            ImGui::Text(" ");
            ImGui::Text("An engine made by two students cursing the third year of Design and Development degree at CITM.");
            ImGui::Text(" ");
            ImGui::Text("Engine made by: ");
            ImGui::Text("Xavi Prats Paredes:  ");
            if (ImGui::MenuItem("   Github: https://github.com/Xarderos"))
            {
                ShellExecute(NULL, "open", "https://github.com/Xarderos", NULL, NULL, SW_SHOW);
            }
            ImGui::Text("Joan Giol Pozo: ");
            if (ImGui::MenuItem("   Github:  https://github.com/GHOSTVCL"))
            {
                ShellExecute(NULL, "open", "https://github.com/GHOSTVCL", NULL, NULL, SW_SHOW);
            }
            ImGui::Text(" ");
            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
            ImGui::Text("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
            ImGui::Text("OpenGL version supported %s", glGetString(GL_VERSION));
            if (ImGui::MenuItem("   https://www.opengl.org/"))
            {
                ShellExecute(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOW);
            }
            ImGui::Text("Using Glew %s", glewGetString(GLEW_VERSION));
            if (ImGui::MenuItem("   https://glew.sourceforge.net/"))
            {
                ShellExecute(NULL, "open", "https://glew.sourceforge.net/", NULL, NULL, SW_SHOW);
            }
            SDL_version version;
            SDL_GetVersion(&version);
            ImGui::Text("Using SDL %i.%i.%i", version.major, version.minor, version.patch);
            if (ImGui::MenuItem("   https://www.libsdl.org/"))
            {
                ShellExecute(NULL, "open", "https://www.libsdl.org/", NULL, NULL, SW_SHOW);
            }
            const char* version_gui = ImGui::GetVersion();
            ImGui::Text("Using ImGui %s", version_gui);
            if (ImGui::MenuItem("   https://github.com/ocornut/imgui"))
            {
                ShellExecute(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOW);
            }
            ImGui::Text("");
            ImGui::Text("");
            ImGui::Text("MIT License");
            ImGui::Text("");
            ImGui::Text("Copyright (c) 2023 GHOSTVCL");
            ImGui::Text("");
            ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
            ImGui::Text("of this software and associated documentation files (the \"Software\"), to deal");
            ImGui::Text("in the Software without restriction, including without limitation the rights");
            ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
            ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
            ImGui::Text("furnished to do so, subject to the following conditions:");
            ImGui::Text("The above copyright notice and this permission notice shall be included in all");
            ImGui::Text("copies or substantial portions of the Software.");
            ImGui::Text("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
            ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
            ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
            ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
            ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
            ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
            ImGui::Text("SOFTWARE.");



            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}

void ModuleEditor::DisplayConfigMenu()
{
    if (ImGui::BeginMenu("Configuration"))
    {
        if (ImGui::BeginMenu("Application")) {


            ImGui::PlotHistogram("FPS", mFPSLog.data(), mFPSLog.size());
            ImGui::EndMenu();

        }
        if (ImGui::BeginMenu("Window")) {
            ImGui::SliderFloat("Brightness", &setbrightness, 0.00f, 1.00f);
            ImGui::SliderInt("Width", &winWidth, 0, 1920);
            ImGui::SliderInt("Height", &winHeight, 0, 1080);


            if (ImGui::Checkbox("Fullscreen", &fullscreen)) {
                App->window->SetFullscreen(fullscreen);
            }

            ImGui::SameLine();
            if (ImGui::Checkbox("Resizable", &resizeable)) {
                App->window->SetResizable(resizeable);
            }

            if (ImGui::Checkbox("Borderless", &bordered)) {
                App->window->SetBorder(bordered);
            }

            if (ImGui::Button("APPLY")) {

                App->window->SetBrightness(setbrightness);
                App->window->SetScreenSize(winWidth, winHeight);

            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Renderer")) {
            if (ImGui::Checkbox("Depth Test", &depthEnabled))
            {
                App->renderer3D->SetDepthTest(depthEnabled);
            }
            if (ImGui::Checkbox("Cull Face", &cullEnabled))
            {
                App->renderer3D->SetCullFace(cullEnabled);
            }
            if (ImGui::Checkbox("Lightning", &lightsEnabled))
            {
                App->renderer3D->SetLightning(lightsEnabled);
            }
            if (ImGui::Checkbox("Wireframe", &wireframeEnabled))
            {
                App->renderer3D->SetWireframe(wireframeEnabled);
            }
            ImGui::EndMenu();
        }

        /*  App->renderer3D->RendererMenu();*/

        ImGui::EndMenu();
    }
}

void ModuleEditor::DisplayMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Assets"))
        {
            ImGui::Text("Hello world!");
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
            App->hierarchy->PrimitivesMenu();

            ImGui::EndMenu();
        }
        DisplayConfigMenu();
        DisplayHelp();
        if (ImGui::Button("Quit")) {
            exit(0);
        }
        ImGui::EndMainMenuBar();
    }
}

