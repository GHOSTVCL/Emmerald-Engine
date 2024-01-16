#pragma once


#include "Particle.h"
#include "MeshImporter.h"
#include "TextureImporter.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

enum TYPES_OF_PARTICLES
{
	SMOKE,

};

class CompTransform;
class Application;
struct Vertex;

class ParticleEmitter
{
public:
	ParticleEmitter(TYPES_OF_PARTICLES typeofpart);

	~ParticleEmitter();

	void Update(float dt);

	void Draw(Quat BBrot);

	void InitBuffers();

	void SetData(const Vertex* vertices, const uint numvertices, const GLuint* indices, const uint numindices);

	void EmitParticles(int numOfparticles);

	void SettingUpParticlePool(Particle& particlePoolRef);

	void SetParticlePoolSize(uint size);

	int SearchNotActiveParticle();

	void AttachEmitterOnGameObject(CompTransform* comp_owner_transform);

	uint id_indices = 0;
	uint id_vertices = 0;
	GLuint texture;

	Texture* text;

	std::vector<Particle> particlesInEmitter;

	ParticleProperties propertiesOfTheParticle;

private:

	std::string name;

	float3 position;



	int lastActiveParticle = 0;

	int numOfParticlesToRespawn = 0;

	int currentparticle;

	//Add Particles efects;

	//Open GL



	//MathGeoLIB RANDOM
	LCG random;

	friend class CompParticles;

};
