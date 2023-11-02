#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

#include "Module.h"
#include <string>

class GameObject;

class ModuleInspector : public Module
{
public:

    ModuleInspector(Application* app, bool start_enabled = true);

    ~ModuleInspector();

    bool Start();

    void Draw();

public:

    GameObject* inspectedObject;

private:
    bool isEnabled;
    std::string name;
    std::string items[3] = { "Transform","Mesh","Texture" };
};

#endif // !__INSPECTOR_H__

