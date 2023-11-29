#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__
#include "Component.h"
#include "Module.h"

class Camera;

class CompCamera : public Component
{
public:
	CompCamera(GameObject* _go);

	~CompCamera();

	void OnEnable();
	void Update();
	void OnDisable();

	void ShowCompUI();

	Camera* camera_;

public:

	int width_, height_;

};

#endif // !__COMPONENT_CAMERA