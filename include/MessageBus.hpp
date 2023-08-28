//message bus to allow inter-component communication

#ifndef MESSAGE_BUS_HPP_
#define MESSAGE_BUS_HPP_

#include <StateIds.hpp>
//#include <PhysicsComponent.hpp>
//#include <PlanktonController.hpp>

#include <SFML/Config.hpp>

#include <queue>

class Message final
{
public:
    enum Type
    {
        Audio = 1,
        Physics,
        Drawable,
        Entity,
        UI,
        Player,
        Plankton,
        ComponentSystem
    }type;

    struct AudioEvent
    {
        sf::Uint64 entityId;
    };

    struct PhysicsEvent
    {
        enum Event
        {
            Collision,
            ConstraintDestroyed,
            Trigger,
            Teleported
        }event;

        sf::Uint64 entityId[2];
        //PhysicsComponent::Constraint* constraint;
    };

    struct DrawableEvent
    {
        float lightX;
        float lightY;
        float lightIntensity;
    };

    struct EntityEvent
    {
        bool maxCollisionsReached;
    };

    struct UIEvent
    {
        enum
        {
            RequestVolumeChange,
            RequestAudioMute,
            RequestAudioUnmute,
            MenuOpened,
            MenuClosed,
            RequestState,
            RequestDifficultyChange,
            RequestControlsClassic,
            RequestControlsArcade,
            RequestControllerEnable,
            RequestControllerDisable,
            RequestColourblindEnable,
            RequestColourblindDisable,
            ResizedWindow,
            MouseClicked
        }type;
        float value;
        States::ID stateId;
        Difficulty difficulty;
        float mouseX, mouseY;
    };

    struct PlayerEvent
    {
        enum
        {
            Died,
            PartAdded,
            PartRemoved,
            HealthAdded,
            HealthLost,
            Spawned,
            BeganEating
        }action;
        float value;
    };

    struct PlanktonEvent
    {
        enum
        {
            Spawned,
            Died
        }action;
        //PlanktonController::Type type;
        bool touchingPlayer;
    };

    struct ComponentEvent
    {
        enum
        {
            Deleted
        }action;
        sf::Uint64 entityId;
        //Component* ptr;
    };

    union
    {
        AudioEvent audio;
        PhysicsEvent physics;
        DrawableEvent drawable;
        EntityEvent entity;
        UIEvent ui;
        PlayerEvent player;
        PlanktonEvent plankton;
        ComponentEvent component;
    };
};

class MessageBus final
{
public:
    MessageBus();
    ~MessageBus() = default;
    MessageBus(const MessageBus&) = delete;
    const MessageBus& operator = (const MessageBus&) = delete;

    //read and despatch all messages on the message stack
    Message poll();
    //places a message on the message stack
    void send(const Message& msg);

    bool empty();

private:
    std::queue<Message> m_messages;
    std::queue<Message> m_deferredMessages;
    bool m_polling;
};

#endif