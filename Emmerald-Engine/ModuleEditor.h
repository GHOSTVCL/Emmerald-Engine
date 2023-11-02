#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"
#include <vector>
#include <string.h>


class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);

	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	void DisplayMainMenuBar();
	void DisplayConfigMenu();
	void DisplayHelp();
	bool CleanUp() override;
	void AddFPS(const float aFPS);

	std::vector <float> mFPSLog;
	bool fullscreen = false;
	bool resizeable = true;
	bool bordered = true;
	bool depthEnabled = false;
	bool cullEnabled = false;
	bool lightsEnabled = false;
	bool colorMaterialEnabled = false;
	bool textureMappingEnabled = false;
	bool wireframeEnabled = false;
	

	float setbrightness;
	int winWidth;

	int winHeight;
};

#endif //MODULE_EDITOR