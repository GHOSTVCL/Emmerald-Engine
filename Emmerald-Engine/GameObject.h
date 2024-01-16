#ifndef _GAMEOBJECT_H__
#define _GAMEOBJECT_H__

#include "Component.h"
#include <vector>
#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

class Component;
class Application;
class CompTransform;

class GameObject
{
public:

	GameObject(std::string name);


	virtual ~GameObject();

	Component* AddComponent(COMP_TYPE type);

	Component* GetComponent(COMP_TYPE type);

	void Update();

	void AddChild(GameObject* _go);

	void SetParent(GameObject* _go);

	void RemoveChild(GameObject* _go);

	void Delete();

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


public:

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	bool active;
	std::string name;

	float LifeTime;
	float3 speed;

	bool transparent;

};

#endif // !_GAMEOBJECT_H__