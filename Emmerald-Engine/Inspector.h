#ifndef __INSPECTOR_H__
#define __INSPECTOR_H__

#include <string>

class GameObject;

class Inspector
{
public:

    Inspector();

    ~Inspector();

    void Start();

    void Draw();

public:

    GameObject* inspectedObject;

private:
    bool isEnabled;
    std::string name;
    std::string items[3] = { "Transform","Mesh","Texture" };
};

#endif // !__INSPECTOR_H__
