#ifndef _GAMEOBJECT_H__
#define _GAMEOBJECT_H__

#include <vector>
#include <string.h>

class Application;

enum COMP_TYPE
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE
};

class Component
{
public:

	Component(GameObject* _go);

	virtual ~Component();

	virtual void OnEnable();
	virtual void Update();
	virtual void OnDisable();

	virtual void OnUIController();

public:

	COMP_TYPE type;
	GameObject* comp_owner;
	bool active;
	std::string name;


};
class GameObject
{
public:

	GameObject(std::string name);

	virtual ~GameObject();

	void PreUpdate();

	void Update();

	void PostUpdate();

	Component* AddComponent(COMP_TYPE type);

	Component* GetComponent(COMP_TYPE type);

	void AddChild(GameObject* _go);

	void SetParent(GameObject* _go);

	void RemoveChild(GameObject* _go);

	template <class T>
	T* GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (typeid(*components[i]) == typeid(T))
			{
				return (T*)components[i];
			}
		}
		return nullptr;
	}

	bool CheckifGameObjIsParent(GameObject* _go);

private:


protected:

	Application* app = nullptr;

public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	bool active;
	std::string name;

	bool readytobedeleted;

	bool transparent;

};

#endif // !_GAMEOBJECT_H__