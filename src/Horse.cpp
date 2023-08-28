#include <Horse.hpp>
#include <Utils.hpp>
#include <NameGenerator.hpp>

using namespace Utils::Random;

namespace
{
	NameGenerator testGen;
}

Horse::Horse()
	: Name(testGen.generateName()),
	Age(value(1, 30)),
	Health(value(1, 10)),
	Luck(value(1, 10)),
	Endurance(value(1, 10)),
	Agility(value(1, 10)),
	Strength(value(1, 10)),
	Potential(Horse::calculatePotential())
	//Selected(false)
{
}

const int Horse::calculatePotential()
{
	int potential = 0;
	if (this->Age > 14)
	{
		potential += 14 - (this->Age - 14);
	}
	else
	{
		potential += this->Age;
	}

	if (potential <= 0)
	{
		potential = 1;
	}
	
	potential = potential * this->Health * this->Luck * this->Endurance * this->Agility * this->Strength;
	assert(potential > 0);

	return potential;
}