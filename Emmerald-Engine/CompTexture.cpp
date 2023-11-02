#include "CompTexture.h"

CompTexture::CompTexture(GameObject* _go) : Component(_go), texture(nullptr) {

	this->name = "Texture_Component";
	this->type = COMP_TYPE::TEXTURE;

}

CompTexture::~CompTexture()
{
	delete(texture);
}

void CompTexture::SetTexture(Texture* _texture)
{

	this->texture = _texture;

}

void CompTexture::ShowCompUI()
{



}
