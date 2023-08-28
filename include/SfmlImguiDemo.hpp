// STATE DEMO IMGUI

#ifndef SFML_IMGUI_DEMO_HPP_
#define SFML_IMGUI_DEMO_HPP_

#include <State.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

class MessageBus;
class SfmlImguiDemo final : public State
{
public:

    SfmlImguiDemo(StateStack& stateStack, Context context);
    ~SfmlImguiDemo();// = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const Message&) override;

private:
    MessageBus& m_messageBus;
    void drawImguiDemo(float dt);
};

#endif //SFML_IMGUI_DEMO_HPP_