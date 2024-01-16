#include "ModuleScene.h"
#include "Application.h"
#include "CompMesh.h"
#include "CompParticles.h"
#include "ParticleEmitter.h"
#include "CompTransform.h"
#include "Random.h"
ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject("Scene");
	root->parent = nullptr;
	selectedGO = root;
	name = "Scene";

	Chimney1 = new GameObject("Smoke Particles 1");
	Chimney2 = new GameObject("Smoke Particles 2");
	Chimney3 = new GameObject("Smoke Particles 3");
	Chimney1->SetParent(root);
	Chimney2->SetParent(root);
	Chimney3->SetParent(root);


}

bool ModuleScene::Start()
{
	tex1 = TextureImporter::ImportTexture("Assets/Particles/Spark1.png", nullptr);
	tex2 = TextureImporter::ImportTexture("Assets/Particles/Spark2.png", nullptr);
	tex3 = TextureImporter::ImportTexture("Assets/Particles/Spark3.png", nullptr);
	tex4 = TextureImporter::ImportTexture("Assets/Particles/Spark4.png", nullptr);
	tex5 = TextureImporter::ImportTexture("Assets/Particles/Spark5.png", nullptr);
	tex6 = TextureImporter::ImportTexture("Assets/Particles/Spark6.png", nullptr);

	Chimney1->AddComponent(PARTICLES);
	Chimney1->GetComponent<CompTransform>()->position = float3(-1.0f, 2.8f, 1);

	Chimney2->AddComponent(PARTICLES);
	Chimney2->GetComponent<CompTransform>()->position = float3(26.6f, 9, 38.7f);
	
	Chimney3->AddComponent(PARTICLES);
	Chimney3->GetComponent<CompTransform>()->position = float3(-31.8f, 6.3f, -34.1f);

	return false;
}

update_status ModuleScene::Update(float dt)
{
	UpdateGo(root);

	if (selectedGO != nullptr) {

		DrawAABB(selectedGO);

	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		LOG("Pressed 1");
		castFirework();
	}
	for (int i = 0; i < fireWorkVector.size(); i++)
	{
		if (fireWorkVector[i]->active == true) {
			if (fireWorkVector[i]->LifeTime <= 0.0f)
			{
				if (!fireWorkVector[i]->GetComponent<CompParticles>()->hasExploded) {

					fireWorkVector[i]->speed = float3::zero;
					fireWorkVector[i]->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.MaxLifetime = 10.0f;
					fireWorkVector[i]->GetComponent<CompParticles>()->hasExploded = true;
					fireWorkVector[i]->LifeTime = 0.4f;

				}
				else {
					fireWorkVector[i]->GetComponent<CompParticles>()->readytoremove = true;
					fireWorkVector[i]->active = false;
					fireWorkVector[i]->GetComponent<CompParticles>()->active = false;

					continue;
				}
			}
			if (fireWorkVector[i]->GetComponent<CompParticles>()->hasExploded) {
				float explosionspeed = 25.0f;
				fireWorkVector[i]->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.velocity = float3(Random::AnyRandomFloat() * explosionspeed, Random::AnyRandomFloat() * explosionspeed, Random::AnyRandomFloat() * explosionspeed);

			}

			float DTspd = App->dt_game;
			fireWorkVector[i]->LifeTime -= DTspd;
			fireWorkVector[i]->GetComponent<CompTransform>()->position += fireWorkVector[i]->speed * DTspd;
		}
	}

	return UPDATE_CONTINUE;
}


update_status ModuleScene::PostUpdate(float dt)
{
	for (int i = 0; i < App->renderer3D->particleSystems.size(); i++) {

		if (App->renderer3D->particleSystems[i]->readytoremove == true) {

			App->renderer3D->particleSystems.erase(App->renderer3D->particleSystems.begin() + i);
			i = 0;

		}
	}
	for (int i = 0; i < fireWorkVector.size(); i++) {
		if (fireWorkVector[i]->active == false) {
			fireWorkVector.erase(fireWorkVector.begin() + i);
			i = 0;
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleScene::UpdateGo(GameObject* goToUpdate)
{

	goToUpdate->Update();

    for (int i = 0; i < goToUpdate->children.size(); i++)
	{
		UpdateGo(goToUpdate->children[i]);
    }

}


void ModuleScene::DrawAABB(GameObject* goToDraw)
{
	if(goToDraw->GetComponent<CompMesh>() != nullptr)
	if (goToDraw->GetComponent<CompMesh>()->mesh != nullptr) {
		goToDraw->GetComponent<CompMesh>()->mesh->printAABB = true;
	}
	for (int i = 0; i < goToDraw->children.size(); i++)
	{
		DrawAABB(goToDraw->children[i]);
	}


}

bool ModuleScene::CleanUp()
{
	return false;
}

void ModuleScene::castFirework() {
	LOG("Casting Firework");

	GameObject* fw = new GameObject("FireWork");
	fw->AddComponent(PARTICLES);
	fw->SetParent(root);
	float posX = Random::AnyRandomFloat() * 10;
	float posZ = Random::AnyRandomFloat() * 10;
	fw->GetComponent<CompTransform>()->position = float3(posX, 0.0f, posZ);

	int randomText = 1 + (rand() % 5);
	switch (randomText) {
	case 1:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex1;
		break;
	case 2:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex2;
		break;
	case 3:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex3;
		break;
	case 4:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex4;
		break;
	case 5:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex5;
		break;
	default:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex3;
		break;
	}
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.velocity = float3(0.0f, 0.1f, 0.0f);
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.acceleration = float3(1.0f, 1.0f, 1.0f);
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.MaxLifetime = 0.45f;
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.startsize = 2.0f;
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.endsize = 2.0f;

	fw->LifeTime = 1.5f;
	float GOspeed = (Random::RandomFloat() + 1) * 10;

	fw->speed = float3(0.0f, GOspeed, 0.0f);

	fireWorkVector.push_back(fw);
	
}