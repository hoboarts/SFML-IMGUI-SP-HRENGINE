#include <HorseGeneration.hpp>

#include <algorithm>
#include <iostream>

HorseGeneration::HorseGeneration(int size)
{
    for (int i = 0; i < size; ++i)
    {
        Horse horse;
        this->Stable.push_back(horse);
    }
}

void HorseGeneration::showHorses()
{
    for (int i = 0; i < Stable.size(); i++)
    {
        std::cout << i + 1 << ". " << this->Stable[i].getName() << " | P : "
            << this->Stable[i].getPotential() << " | AGE: " << this->Stable[i].getAge()
            << " | H: " << this->Stable[i].getHealth() << " | L: " << this->Stable[i].getLuck()
            << " | E: " << this->Stable[i].getEndurance() << " | A: " << this->Stable[i].getAgility()
            << " | S: " << this->Stable[i].getStrength() << std::endl;
        if (i == Stable.size() - 1)
        {
            std::cout << "-----------------------------------------------------------------------------" << std::endl;
        }
    }
}

void HorseGeneration::sortStable()
{
    std::sort(this->Stable.begin(), this->Stable.end());
}

std::vector<Horse> HorseGeneration::getStable()
{
    return this->Stable;
}

void HorseGeneration::setStable(std::vector<Horse> stable)
{
    this->Stable = stable;
}

Horse HorseGeneration::getHorse(int horseNo)
{
    return this->Stable[horseNo];
}

Horse HorseGeneration::replaceHorse(int horseNo)
{
    const Horse temp = this->Stable[horseNo];
    this->Stable.erase(this->Stable.begin() + horseNo);
    this->Stable.shrink_to_fit();
    return temp;
}

void HorseGeneration::addHorse(Horse horse)
{
    this->Stable.push_back(horse);
}

void HorseGeneration::removeHorse(int horseNo)
{
    this->Stable.erase(this->Stable.begin() + horseNo);
}

void HorseGeneration::clearStable()
{
    this->Stable.clear();
}

void HorseGeneration::swapHorses(const int horseNo, const int horseNo2)
{
    const Horse tempH = this->Stable[horseNo];
    const Horse tempH2 = this->Stable[horseNo2];
    this->Stable[horseNo] = tempH2;
    this->Stable[horseNo2] = tempH;
}
