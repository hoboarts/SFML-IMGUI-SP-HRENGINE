
#include <State.hpp>
#include <StateStack.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

State::Context::Context(sf::RenderWindow& rw, App& app)
    : renderWindow(rw), appInstance(app) {}


//-----------------------------------------------------

State::State(StateStack& stateStack, Context context)
    : m_stateStack(stateStack),
    m_context(context)
{

}

void State::setContext(Context c)
{
    m_context.defaultView = c.defaultView;
}

//protected
void State::requestStackPush(States::ID id)
{
    m_stateStack.pushState(id);
}

void State::requestStackPop()
{
    m_stateStack.popState();
}

void State::requestStackClear()
{
    m_stateStack.clearStates();
}

State::Context State::getContext() const
{
    return m_context;
}