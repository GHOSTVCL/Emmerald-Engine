#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include <string>


class GameObject;

enum COMP_TYPE
{
	NONE,
	TRANSFORM,
	MESH,
	TEXTURE,
	CAMERA,
	BILLBOARD,
	PARTICLES
};

class Component
{
public:

	Component(GameObject* _go);

	virtual ~Component();

	virtual void OnEnable();
	virtual void Update();
	virtual void OnDisable();

	virtual void ShowCompUI();

public:

	COMP_TYPE type;
	GameObject* comp_owner;
	bool active;
	std::string name;


};

#endif