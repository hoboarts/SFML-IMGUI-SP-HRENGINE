// HORSE RACE QUIZ

#ifndef HORSE_RACE_HPP_
#define HORSE_RACE_HPP_

#include <State.hpp>

#include <HorseGeneration.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

class MessageBus;
class HorseRace final : public State
{
public:
    struct History final
    {
        
        std::vector<std::string> textVec;
        
        void addLine(std::string text)
        {
            textVec.push_back(text);
        }
        
        History()
            : textVec()
        {}
    };

    HorseRace(StateStack& stateStack, Context context);
    ~HorseRace();// = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const Message&) override;

private:
    MessageBus& m_messageBus;
    HorseGeneration m_generatedStable;
    HorseGeneration m_chosenForRace;
    History m_history;
    void drawHorseRace(float dt);
    void writeHistory(int rC, HorseGeneration* stable);
    bool selArr[25];
};

#endif //HORSE_RACE_HPP_