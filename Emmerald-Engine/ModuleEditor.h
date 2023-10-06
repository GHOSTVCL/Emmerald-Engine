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
	

};

#endif //MODULE_EDITOR