#ifndef __COMPONENT_TEXTURE_
#define __COMPONENT_TEXTURE_
#include "Component.h"
#include "ModuleTexture.h"

class CompTexture :
    public Component
{
public:

    CompTexture(GameObject* _go);

    ~CompTexture();

    void SetTexture(Texture* _texture);

    Texture* GetTexture() { return texture; }

    void ShowCompUI() override;


public:
    Texture* texture;
    Application* app;
    std::string path;

private:


};

#endif // !__COMPONENT_TEXTURE__