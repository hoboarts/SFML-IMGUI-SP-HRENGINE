#ifndef HORSEGENERATION_HPP_
#define HORSEGENERATION_HPP_

#include <Horse.hpp>

#include <vector>

namespace
{
    const int StableSize = 25;
}

class HorseGeneration final
{
public:
    HorseGeneration(int size = StableSize);
    ~HorseGeneration() = default;
    HorseGeneration(const HorseGeneration&) = delete;//NOT TO ALLOW COPY OF CLASS
    const HorseGeneration& operator = (const HorseGeneration&) = delete;// NOT TO ALLOW MOVE CLASS


    void showHorses();
    void sortStable();
    std::vector<Horse> getStable();
    void setStable(std::vector<Horse> stable);
    Horse getHorse(int horseNo);
    Horse replaceHorse(int horseNo);
    void addHorse(Horse horse);
    void removeHorse(int horseNo);
    void clearStable();
    void swapHorses(int horseNo, int horseNo2);

private:
    std::vector<Horse> Stable;
};

#endif //HORSEGENERATION_HPP_