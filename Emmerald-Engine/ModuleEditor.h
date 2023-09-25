#ifndef MODULE_EDITOR
#define MODULE_EDITOR

#include "Module.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Init() override;
	void DrawEditor();
	bool Cleanup() override;

};

#endif //MODULE_EDITOR