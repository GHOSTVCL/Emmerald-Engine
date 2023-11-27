
#include "Component.h"
#include "TextureImporter.h"

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
    std::string path;
private:


};

