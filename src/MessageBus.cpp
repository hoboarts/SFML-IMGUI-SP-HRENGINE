
#include <MessageBus.hpp>
#include <Log.hpp>

MessageBus::MessageBus()
    : m_polling(false) {}

Message MessageBus::poll()
{
    m_polling = true;
    Message m = m_messages.front();
    m_messages.pop();

    return m;
}

void MessageBus::send(const Message& m)
{
    (m_polling) ? m_deferredMessages.push(m) : m_messages.push(m);
}

bool MessageBus::empty()
{
    auto empty = m_messages.empty();
    if (empty)
    {
        m_deferredMessages.swap(m_messages);
        m_polling = false;
    }

    return empty;
}