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
	void DisplayHelp();
	bool CleanUp() override;
	void AddFPS(const float aFPS);

	std::vector <float> mFPSLog;
	bool fullscreen = false;
	bool resizeable = true;
	bool bordered = true;

	bool dtest = false;
	bool cface = false;
	bool lighting = false;
	bool cmaterial = false;
	bool txt2d = false;
	bool wframe = false;

	float setbrightness;
	int winWidth;

	int winHeight;
};

#endif //MODULE_EDITOR