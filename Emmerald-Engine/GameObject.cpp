#include "GameObject.h"
#include "Application.h"


#include "CompMesh.h"
#include "CompTexture.h"
#include "CompTransform.h"
#include <string>

GameObject::GameObject(std::string name)
{
	if (name == "")
	{
		name = "GameObject";
	}
	this->name = name;

	this->AddComponent(COMP_TYPE::TRANSFORM);
	this->AddComponent(COMP_TYPE::MESH);
	this->AddComponent(COMP_TYPE::TEXTURE);

}

GameObject::~GameObject()
{


}


Component* GameObject::AddComponent(COMP_TYPE type)
{

	Component* comp = nullptr;
	switch (type)
	{
	case NONE:
		break;
	case MESH:
		comp = new CompMesh(this);
		comp->type = type;
		break;
	case TRANSFORM:
		comp = new CompTransform(this);
		comp->type = type;
		break;
	case TEXTURE:
		comp = new CompTexture(this);
		comp->type = type;
		break;
	}
	if (comp != nullptr)
	{
		components.push_back(comp);
	}
	return comp;

}

Component* GameObject::GetComponent(COMP_TYPE _type)
{
	Component* comp = nullptr;

	for (int i = 0; i < components.size(); i++)
	{
		if (this->components[i]->type == _type)
		{
			comp = components[i];
			return comp;
		}
	}
	return comp;
}

void GameObject::AddChild(GameObject* _go)
{
	if (_go == nullptr) return;
	if (this->parent == _go) return;

	_go->parent = this;

	children.push_back(_go);


}

void GameObject::SetParent(GameObject* _go)
{
	if (_go->CheckifGameObjIsParent(this))
	{
		return;
	}

	if (this->parent != nullptr)
	{
		this->parent->RemoveChild(this);
	}
	_go->AddChild(this);

}

void GameObject::RemoveChild(GameObject* _go)
{
	if (this->children.size() == 0) return;

	for (int i = 0; i < this->children.size(); i++)
	{
		if (this->children[i] == _go)
		{
			this->children.erase(this->children.begin() + i);
		}
	}
}

void GameObject::Delete()
{
	
	if (parent != nullptr) {

		for (int i = 0; i < parent->children.size(); i++)
		{
			if (parent->children[i] == this) {
				
				parent->children.erase(parent->children.begin() + i);
				break;

			}
		}
	}

	
	for (int i = 0; i < components.size(); i++) {

		delete components[i];
		components[i] = nullptr;
	}

	
	for (int i = 0; i < children.size(); i++) {

		children[i]->Delete();
		children[i] = nullptr;
	}

	parent = nullptr;

	
	children.clear();
	components.clear();


}

bool GameObject::CheckifGameObjIsParent(GameObject* _goToDrop)
{
	//Parent of go in begindragdrop
	GameObject* temp = this->parent;

	//When temp is root stop searching
	while (temp != nullptr)
	{
		//if goToDrop is this gameobject parent not set Parent
		if (temp == _goToDrop)
		{
			return true;
		}
		else
		{
			//Update temp in order to see if someone else in the tree should not be set parent
			temp = temp->parent;
		}
	}
	//if goToDrop is not this gameobject parent then, Set Parent
	return false;
}