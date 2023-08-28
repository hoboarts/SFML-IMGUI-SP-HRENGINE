// UNDER CONSTRUCTION

#ifndef HORSE_HPP_
#define HORSE_HPP_

#include <string>

class Horse
{
public:
	Horse();
	~Horse() = default;

	bool operator < (const Horse& other) const {
		return Potential > other.Potential;
	}

	//bool* ptr = &Selected;
	const int calculatePotential();
	std::string getName() { return this->Name; }
	int getAge() { return this->Age; }
	int getHealth() { return this->Health; }
	int getLuck() { return this->Luck; }
	int getEndurance() { return this->Endurance; }
	int getAgility() { return this->Agility; }
	int getStrength() { return this->Strength; }
	int getPotential() { return this->Potential; }

private:
	std::string Name;
	__int8 Age; // 2 - 30 ( + > 14 < - )
	__int8 Health; // 1 - 10  // FORMULA ~ HEALTH * AGE >14< * LUCK * End * Agi * Str
	__int8 Luck; // 1 - 10
	__int8 Endurance; // 1 - 10
	__int8 Agility; // 1 - 10
	__int8 Strength; // 1 - 10
	int Potential;
	//bool Selected;
};

#endif //HORSE_HPP_