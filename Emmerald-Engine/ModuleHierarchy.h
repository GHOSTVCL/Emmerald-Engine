#ifndef __MODULE_HIERARCHY_H__
#define __MODULE_HIERARCHY_H__

#include "Module.h"
#include <string>

class Application;
class GameObject;

class ModuleHierarchy : public Module
{
public:

    ModuleHierarchy(Application* app, bool start_enabled = true);

    ~ModuleHierarchy();

    bool Start();

    void Draw();

    void PrimitivesMenu();

    void ShowGameObjects(GameObject* go);

    GameObject* objSelected;

private:

    GameObject* goToDrop;
    

    int emptycounter = 0;

    bool isEnabled;
    std::string name;
};

#endif // !__MODULE_HIERARCHY_H__