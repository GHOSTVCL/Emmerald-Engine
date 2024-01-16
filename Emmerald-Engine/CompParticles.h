#ifndef __COMP_PARTICLESYSTEM_H__
#define __COMP_PARTICLESYSTEM_H__

#include "Component.h"
#include <vector>

class ParticleEmitter;
class Application;

class CompParticles :
	public Component
{

public:

	CompParticles(GameObject* _go);
	~CompParticles();

	void Update() override;

	void ShowCompUI() override;

	void Draw();

	void Play();
	void Stop();

	void AddParticleEmitter();

	std::vector<ParticleEmitter*> emitters;
	bool hasExploded;
	float maxDuration = 0.0f;
private:

	bool systemActive;
	//emitter vector

	bool looping = false;


	//Timer insert

};

#endif // !__COMP_PARTICLESYSTEM_H__