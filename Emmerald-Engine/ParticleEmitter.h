#ifndef __PARTICLE_EMITTER_H__
#define __PARTICLE_EMITTER_H__

#include "Particle.h"
#include "MeshImporter.h"
#include "TextureImporter.h"


enum TYPES_OF_PARTICLES
{
	SMOKE,

};

class CompTransform;
class Shaders;
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


private:

	std::string name;

	float3 position;

	std::vector<Particle> particlesInEmitter;

	ParticleProperties propertiesOfTheParticle;

	int lastActiveParticle = 0;

	int numOfParticlesToRespawn = 0;

	int currentparticle;

	//Add Particles efects;

	//Open GL

	uint id_indices = 0;
	uint id_vertices = 0;
	uint textID = 0;

	GLuint texture;

	Texture* text;

	//MathGeoLIB RANDOM
	LCG random;

	friend class CompParticles;

};

#endif // !__PARTICLE_EMITTER_H__