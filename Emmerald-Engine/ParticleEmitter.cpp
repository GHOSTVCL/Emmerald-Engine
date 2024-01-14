#include "ParticleEmitter.h"

#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "TextureImporter.h"
#include "CompTransform.h"


ParticleEmitter::ParticleEmitter(TYPES_OF_PARTICLES typeofpart)
{

	this->name = "smokeparticle";
	//TODO UPGRADE
	//Compute how many particles are being emited each second and multiply by lifetime of each particle and set that as poolsize

	SetParticlePoolSize(2000);

	ilInit();
	text = TextureImporter::ImportTexture("Assets/Textures/smoke.png", NULL);
	//INIT BUFFERS

	InitBuffers();

	switch (typeofpart)
	{

	case (SMOKE):

		propertiesOfTheParticle.position = { 0.0f,1.0f,0.0f };
		propertiesOfTheParticle.startsize = 0.5f;
		propertiesOfTheParticle.endsize = 0.2f;
		propertiesOfTheParticle.MaxLifetime = 14.0f;
		propertiesOfTheParticle.velocity = { 0.01f,0.01f,0.0f };
		propertiesOfTheParticle.acceleration = { 0.1f,0.1f,0.0f };
		propertiesOfTheParticle.startColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		propertiesOfTheParticle.endColor = float4(0.6f, 0.6f, 0.6f, 0.0f);

		break;
	default:

		propertiesOfTheParticle.position = { random.Float() * 5,3.0f,random.Float() };
		propertiesOfTheParticle.startsize = random.Float() * 5;
		propertiesOfTheParticle.endsize = 0.1f;
		propertiesOfTheParticle.MaxLifetime = 5.0f;
		propertiesOfTheParticle.velocity = { 0.0f,0.01f,0.0f };
		propertiesOfTheParticle.acceleration = { 0.0f,0.01f,0.0f };
		propertiesOfTheParticle.startColor = float4(0.0f, 0.0f, 1.0f, 1.0f);
		propertiesOfTheParticle.endColor = float4(0.0f, 0.0f, 1.0f, 1.0f);

	}

	currentparticle = particlesInEmitter.size() - 1;

	EmitParticles(10);

}

void ParticleEmitter::InitBuffers()
{
	uint indices[]
	{
		0, 1, 2,
		2, 3, 0,
	};

	float vertices[]
	{
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //0
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //1
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, //2
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, //3

	};

	//Fill buffers with vertices
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, (GLuint*)&(id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 5, vertices, GL_STATIC_DRAW);

	//Fill buffers with indices
	glGenBuffers(1, (GLuint*)&(id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices, GL_STATIC_DRAW);

	glDisableClientState(GL_VERTEX_ARRAY);

}

ParticleEmitter::~ParticleEmitter()
{
	
	particlesInEmitter.clear();

}

void ParticleEmitter::Update(float dt)
{
	for (int i = 0; i < numOfParticlesToRespawn; i++)
	{
		lastActiveParticle = SearchNotActiveParticle();

		//TODO REWORK THIS IN ORDER TO CHANGE HOW RESPAWN PARTICLES WORK
		SettingUpParticlePool(particlesInEmitter[lastActiveParticle]); 
		numOfParticlesToRespawn--;
	}

	EmitParticles(1);

	for (int i = 0; i < particlesInEmitter.size(); i++)
	{
		//No Active means it is available to be remplace
		if (particlesInEmitter[i].Active == false)
		{
			continue;
		}
		//No remainingLifetime means it is dead therefore, is not active
		if (particlesInEmitter[i].remainingLifetime < 0.0f)
		{
			numOfParticlesToRespawn++;
			particlesInEmitter[i].Active = false;
			continue;
		}
		else
		{

			//Compute all the calculus needed to move the particles

			//Remaining life minus dt
			particlesInEmitter[i].remainingLifetime -= dt;

			
			if (particlesInEmitter[i].remainingLifetime > 0.0f)
			{
				// velocity = acceleration * dt
				particlesInEmitter[i].velocity += particlesInEmitter[i].acceleration * dt ;

				// pos += velocity * dt
				particlesInEmitter[i].position += particlesInEmitter[i].velocity * dt;

				
				
			}
		}
	}
}

void ParticleEmitter::Draw(Quat BBrot)
{
	//Vertices
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id_vertices);

	glVertexPointer(3, GL_FLOAT, sizeof(float) * 5, NULL);
	glTexCoordPointer(2, GL_FLOAT, sizeof(float) * 5, (void*)(sizeof(float) * 3));
	//bind and use other buffers


	if (text) {

		glBindTexture(GL_TEXTURE_2D, textID);

	}

	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_indices);


	for (Particle& particleInPool : particlesInEmitter)
	{
		if (particleInPool.Active == false)
		{
			continue;
		}

		Quat BBRotAroundZ = BBrot * Quat::RotateAxisAngle({ 0.0f,0.0f,1.0f }, DegToRad(particleInPool.rotation));

		float percentageOfLife = particleInPool.remainingLifetime / particleInPool.maxLifetime;

		float4 currentcolor = math::Lerp(particleInPool.endColor, particleInPool.startColor, percentageOfLife);

		float currentsize = math::Lerp(particleInPool.endSize, particleInPool.startSize, percentageOfLife);

		//Gather pos & rotation &scale
		float4x4 transform = float4x4::FromTRS(particleInPool.position, BBRotAroundZ, { currentsize,currentsize ,1.0f }).Transposed();


		if (!text) {
			glColor4f(currentcolor.x, currentcolor.y, currentcolor.z, currentcolor.w);
		}

		glPushMatrix();

		glMultMatrixf(transform.ptr());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		glPopMatrix();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	//cleaning texture
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	
}

void ParticleEmitter::SetParticlePoolSize(uint size)
{
	particlesInEmitter.resize(size);
	/*for (int i = 0; i< particlesInEmitter.size();i++)
	{
		SettingUpParticlePool(particlesInEmitter[i]);
	}*/
}

int ParticleEmitter::SearchNotActiveParticle()
{

	// Search the last particle that was used or a particle that has not been used yet 
	//and returns its index in particlesInEmitter Pool

	int maxParticles = particlesInEmitter.size();
	//First we check from the index of the last active particle

	for (int i = lastActiveParticle; i < maxParticles; ++i)
	{
		if (particlesInEmitter[i].remainingLifetime < 0.0f && particlesInEmitter[i].Active == false) {
			lastActiveParticle = i;
			return i;
		}
	}

	//If the first for doesn't return anything we check from the index num 0 till last active
	for (int i = 0; i < lastActiveParticle; ++i)
	{
		if (particlesInEmitter[i].remainingLifetime < 0.0f && particlesInEmitter[i].Active == false)
		{
			lastActiveParticle = i;
			return i;
		}
	}


	lastActiveParticle = 0;
	return 0; // If All particles are Active return the index of the first one and override it
}

void ParticleEmitter::EmitParticles(int numOfparticles)
{
	//SET UP PARTICLE

	for (int i = 0; i < numOfparticles;i++)
	{
		if (currentparticle <= 0)
		{
			currentparticle = particlesInEmitter.size() - 1;
		}

		SettingUpParticlePool(particlesInEmitter[currentparticle]);

		currentparticle--;

	}

}

void ParticleEmitter::SettingUpParticlePool(Particle& particlePoolRef)
{

	particlePoolRef.position = propertiesOfTheParticle.position + this->position;
	particlePoolRef.position.z += random.Float() -0.5;
	particlePoolRef.Active = true;

	particlePoolRef.acceleration = propertiesOfTheParticle.acceleration;
	particlePoolRef.velocity = propertiesOfTheParticle.velocity;
	particlePoolRef.velocity.x += propertiesOfTheParticle.acceleration.x * random.Float() * 1.2f;
	particlePoolRef.velocity.y += propertiesOfTheParticle.acceleration.y * random.Float() * 1.2f;

	particlePoolRef.startColor = propertiesOfTheParticle.startColor;
	particlePoolRef.endColor = propertiesOfTheParticle.endColor;

	particlePoolRef.maxLifetime = propertiesOfTheParticle.MaxLifetime;
	particlePoolRef.remainingLifetime = propertiesOfTheParticle.MaxLifetime;

	particlePoolRef.startSize = propertiesOfTheParticle.startsize;
	particlePoolRef.endSize = propertiesOfTheParticle.endsize;

}



void ParticleEmitter::AttachEmitterOnGameObject(CompTransform* comp_owner_transform)
{
	//Pass gameobject position to emitter pos
	this->position = comp_owner_transform->GetGlobalMatrix().TranslatePart();
}


