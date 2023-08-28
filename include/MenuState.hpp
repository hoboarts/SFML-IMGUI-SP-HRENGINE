//initial menu state

#ifndef MENU_STATE_HPP_
#define MENU_STATE_HPP_

#include <State.hpp>
//#include <UIContainer.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MessageBus;
class MenuState final : public State
{
public:
    MenuState(StateStack& stateStack, Context context);
    ~MenuState() = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const Message&) override;

private:
    MessageBus& m_messageBus;
    sf::Sprite m_menuSprite;
    sf::Sprite m_cursorSprite;
    std::vector<sf::Text> m_texts;

    //ui::Container m_uiContainer;

    void buildMenu(const sf::Font&);
    void startGame();
};

#endif //MENU_STATE_HPP_