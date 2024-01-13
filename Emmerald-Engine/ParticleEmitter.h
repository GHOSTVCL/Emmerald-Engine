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

	void Draw(Shaders* shader, Quat BBrot);

	void InitBuffers();

	void SetData(const Vertex* vertices, const uint numvertices, const GLuint* indices, const uint numindices);

	void EmitParticles(int numOfparticles);

	void SettingUpParticlePool(Particle& particlePoolRef);

	void SetParticlePoolSize(uint size);

	int SearchNotActiveParticle();

	void AttachEmitterOnGameObject(CompTransform* comp_owner_transform);

	void SaveParticle(Particle& particleToSave);

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

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	GLuint texture;

	Texture* text;

	//MathGeoLIB RANDOM
	LCG random;

	friend class CompParticles;

};

#endif // !__PARTICLE_EMITTER_H__