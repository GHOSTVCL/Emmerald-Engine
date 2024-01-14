#include "CompParticles.h"

#include "Application.h"
#include "ModuleRenderer3D.h"
#include "CompTransform.h"
#include "CompBillBoarding.h"
#include "ParticleEmitter.h"
#include "Timer.h"
#include "ImGui/imgui.h"
#include "Globals.h"
#include "Module.h"

CompParticles::CompParticles(GameObject* _go) :Component(_go)
{
	this->type = COMP_TYPE::PARTICLES;
	this->name = "Particle_System";

	AddParticleEmitter();
	App->renderer3D->particleSystems.push_back(this);

}

CompParticles::~CompParticles()
{


	for (int i = 0; i < emitters.size(); i++)
	{
		RELEASE(emitters[i]);
	}

	emitters.clear();

	for (int i = 0; i < App->renderer3D->particleSystems.size(); i++)
	{
		CompParticles* temp = App->renderer3D->particleSystems[i];
		if (temp == this)
		{
			App->renderer3D->particleSystems.erase(App->renderer3D->particleSystems.begin() + i);
		}

	}

}

void CompParticles::Update()
{
	//DO WE REALLY NEED A VECTOR OF EMITTERS IN A PARTICLE SYSTEM?
	for each (ParticleEmitter * parrticleemitter in emitters)
	{
		parrticleemitter->AttachEmitterOnGameObject(comp_owner->GetComponent<CompTransform>());
		if (App->game_State == GameState::STOP)
		{
			parrticleemitter->Update(App->dt_game);
		}
	}

}

void CompParticles::ShowCompUI()
{
	if (ImGui::CollapsingHeader("Particle System", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Separator();
		ImGui::Text("Particle Settings:");
		ImGui::Separator();

		//CHANGE THIS IN ORDER TO HANDLE MULTIPLE EMITTERS IN ONE PARTICLE SYSTEM
		ImGui::DragFloat3("Velocity", emitters[0]->propertiesOfTheParticle.velocity.ptr(), 0.1f, 0.0f, 1000.0f);
		ImGui::DragFloat3("Acceleration", emitters[0]->propertiesOfTheParticle.acceleration.ptr(), 0.1f, 0.0f, 1000.0f);

		ImGui::ColorEdit4("Start Color ", emitters[0]->propertiesOfTheParticle.startColor.ptr());
		ImGui::ColorEdit4("End Color ", emitters[0]->propertiesOfTheParticle.endColor.ptr());

		ImGui::DragFloat("Life Time ", &emitters[0]->propertiesOfTheParticle.MaxLifetime, 0.1f, 0.1f, 1000.0f);

		ImGui::DragFloat("Start Size ", &emitters[0]->propertiesOfTheParticle.startsize, 0.1f, 0.1f, 50.0f);
		ImGui::DragFloat("End Size ", &emitters[0]->propertiesOfTheParticle.endsize, 0.1f, 0.1f, 50.0f);

	}

}

void CompParticles::Draw()
{
	if (comp_owner->GetComponent<CompBillBoarding>() == nullptr)
	{
		comp_owner->AddComponent(COMP_TYPE::BILLBOARD);
	}


	for each (ParticleEmitter * parrticleemitter in emitters)
	{
		parrticleemitter->Draw(comp_owner->GetComponent<CompBillBoarding>()->GetBBRotation());
	}
}

void CompParticles::Play()
{

}

void CompParticles::Stop()
{

}

void CompParticles::AddParticleEmitter()
{
	emitters.push_back(new ParticleEmitter(TYPES_OF_PARTICLES::SMOKE));
}
