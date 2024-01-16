#include "Random.h"

void Random::Init()
{
	srand(static_cast <unsigned> (time(0)));
}

float Random::RandomFloat()
{
	//Random number from 0.0 to 1.0
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	return r;
}

float Random::AnyRandomFloat() {
	// Random number from -1.0 to 1.0
	return 2.0f * RandomFloat() - 1.0f;
}