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
}

bool ModuleScene::Start()
{
	tex1 = TextureImporter::ImportTexture("Assets/Particles/Spark1.png", nullptr);
	tex2 = TextureImporter::ImportTexture("Assets/Particles/Spark2.png", nullptr);
	tex3 = TextureImporter::ImportTexture("Assets/Particles/Spark3.png", nullptr);
	tex4 = TextureImporter::ImportTexture("Assets/Particles/Spark4.png", nullptr);
	tex5 = TextureImporter::ImportTexture("Assets/Particles/Spark5.png", nullptr);
	tex6 = TextureImporter::ImportTexture("Assets/Particles/Spark6.png", nullptr);

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
					fireWorkVector[i]->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.velocity = float3(1.0f, 1.0f, 1.0f);
					fireWorkVector[i]->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.acceleration = float3(50.0f, 50.0f, 50.0f);
					fireWorkVector[i]->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.MaxLifetime = 10.0f;
					fireWorkVector[i]->GetComponent<CompParticles>()->hasExploded = true;
					fireWorkVector[i]->LifeTime = 2.5f;

				}
				else {
					fireWorkVector[i]->active = false;
					fireWorkVector[i]->GetComponent<CompParticles>()->active = false;
					continue;
				}
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

	float posX = Random::RandomFloat() * 10;
	float posZ = Random::RandomFloat() * 10;
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
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex3;
		break;
	case 5:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex3;
		break;
	default:
		fw->GetComponent<CompParticles>()->emitters[0]->text = tex3;
		break;
	}
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.velocity = float3(0.0f, 10.0f, 0.0f);
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.acceleration = float3(1.0f, 1.0f, 1.0f);
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.MaxLifetime = 5.0f;
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.startsize = 10.0f;
	fw->GetComponent<CompParticles>()->emitters[0]->propertiesOfTheParticle.endsize = 10.0f;

	fw->LifeTime = 5.0f;
	float GOspeed = Random::RandomFloat() * 1;

	fw->speed = float3(0.0f, GOspeed, 0.0f);

	fireWorkVector.push_back(fw);
	
}