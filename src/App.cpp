#include <App.hpp>
#include <MenuState.hpp>
#include "SfmlImguiDemo.hpp"
#include "RacingTopDown.hpp"
#include "HorseRace.hpp"
#include <Icon.hpp>
//#include <HorseGeneration.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Shader.hpp>

//#include <StringArraySort.hpp>//MOD
//#include <NameDictionary.hpp>//MOD

//Needed to access resources on OS X
#ifdef __APPLE__
#include <ResourcePath.hpp>
#endif

using namespace std::placeholders;

namespace
{
    const float timePerFrame = 1.f / 60.f;

    sf::Clock frameClock;
    float timeSinceLastUpdate = 0.f;
    const std::string sfmlImGuiVer = ("(SFML ver." + std::to_string(SFML_VERSION_MAJOR) + "." + std::to_string(SFML_VERSION_MINOR) + "." + std::to_string(SFML_VERSION_PATCH) + ", Dear ImGui ver." + IMGUI_VERSION + " (" + std::to_string(IMGUI_VERSION_NUM) + "))");
    const std::string windowTitle("HR " + sfmlImGuiVer);
}

App::App()
    : m_videoSettings(),
    m_renderWindow(m_videoSettings.VideoMode, windowTitle, m_videoSettings.WindowStyle),
    m_stateStack({ m_renderWindow, *this })
    //m_pendingDifficulty(Difficulty::Easy)
{
    registerStates();

    //ImGui::SFML::Init(m_renderWindow);//BOGEYMAN

#ifndef P_DEBUG_
    m_stateStack.pushState(States::ID::Menu);
    //m_stateStack.pushState(States::ID::Title);
#else
    //m_stateStack.pushState(States::ID::Racing);
    m_stateStack.pushState(States::ID::HorseRace);
    //m_stateStack.pushState(States::ID::Main);
    //m_stateStack.pushState(States::ID::Menu);
#endif //P_DEBUG_

    //loadSettings();
    //m_scores.load();

    m_renderWindow.setVerticalSyncEnabled(m_videoSettings.VSync);
    m_renderWindow.setIcon(icon_width, icon_height, icon_arr);

    //store available modes and remove unusable
    m_videoSettings.AvailableVideoModes = sf::VideoMode::getFullscreenModes();
    m_videoSettings.AvailableVideoModes.erase(std::remove_if(m_videoSettings.AvailableVideoModes.begin(), m_videoSettings.AvailableVideoModes.end(),
        [](const sf::VideoMode& vm)
        {
            return (!vm.isValid() || vm.bitsPerPixel != 32);
        }), m_videoSettings.AvailableVideoModes.end());
    std::reverse(m_videoSettings.AvailableVideoModes.begin(), m_videoSettings.AvailableVideoModes.end());

    update = std::bind(&App::updateApp, this, _1);
}

void App::run()
{
    if (!sf::Shader::isAvailable())
    {
        Logger::Log("Shaders reported as unavailable.", Logger::Type::Error, Logger::Output::File);
        return;
    }

    m_renderWindow.setMouseCursorVisible(false);

    frameClock.restart();
    while (m_renderWindow.isOpen())
    {
        float elapsedTime = frameClock.restart().asSeconds();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;

            handleEvents();
            handleMessages();
            update(timePerFrame);
        }
        draw();
    }
}

void App::pause()
{
    update = std::bind(&App::pauseApp, this, _1);
}

void App::resume()
{
    update = std::bind(&App::updateApp, this, _1);
    frameClock.restart();
    timeSinceLastUpdate = 0.f;
}

const App::VideoSettings& App::getVideoSettings() const
{
    return m_videoSettings;
}

void App::applyVideoSettings(const VideoSettings& settings)
{
    if (m_videoSettings == settings) return;

    auto availableModes = m_videoSettings.AvailableVideoModes;

    m_renderWindow.create(settings.VideoMode, windowTitle, settings.WindowStyle);
    m_renderWindow.setVerticalSyncEnabled(settings.VSync);
    m_renderWindow.setMouseCursorVisible(false);
    //TODO test validity and restore old settings if possible
    m_videoSettings = settings;
    m_videoSettings.AvailableVideoModes = availableModes;
    m_stateStack.updateView();

    m_renderWindow.setIcon(icon_width, icon_height, icon_arr);
}

sf::Font& App::getFont(const std::string& path)
{
    return m_fontResource.get(path);
}

sf::Texture& App::getTexture(const std::string& path)
{
    return m_textureResource.get(path);
}

MessageBus& App::getMessageBus()
{
    return m_messageBus;
}

//private
void App::handleEvents()
{
    sf::Event evt;
    while (m_renderWindow.pollEvent(evt))
    {
        switch (evt.type)
        {
        case sf::Event::LostFocus:
            pause();
            continue;
        case sf::Event::GainedFocus:
            resume();
            continue;
        case sf::Event::Closed:
            m_renderWindow.close();
            return;
        default: break;
        }

#ifdef P_DEBUG_// FAST EXIT APP ON DEBUG
        if (evt.type == sf::Event::KeyPressed
            && evt.key.code == sf::Keyboard::Escape)
        {
            m_renderWindow.close();
        }
#endif //P_DEBUG_
        
        /* if (evt.type == sf::Event::KeyPressed
            && evt.key.code == sf::Keyboard::F5)
        {
            saveScreenshot();
        } */
        
        //ImGui::SFML::ProcessEvent(m_renderWindow, evt);//BOGEYMAN
        m_stateStack.handleEvent(evt);
    }
}

void App::handleMessages()
{
    while (!m_messageBus.empty())
    {
        auto msg = m_messageBus.poll();

        switch (msg.type)
        {
        case Message::Type::Player:
            /* if (msg.player.action == Message::PlayerEvent::Spawned)
            {
                m_gameSettings.difficulty = m_pendingDifficulty;
            }
            break; */
        case Message::Type::UI:
            switch (msg.ui.type)
            {
            /*case Message::UIEvent::RequestDifficultyChange:
                m_pendingDifficulty = msg.ui.difficulty;
                break;
            case Message::UIEvent::RequestAudioMute:
                m_audioSettings.muted = true;
                break;
            case Message::UIEvent::RequestAudioUnmute:
                m_audioSettings.muted = false;
                break;
            case Message::UIEvent::RequestVolumeChange:
                m_audioSettings.volume = msg.ui.value;
                break;
            case Message::UIEvent::RequestControlsArcade:
                m_gameSettings.controlType = ControlType::Arcade;
                break;
            case Message::UIEvent::RequestControlsClassic:
                m_gameSettings.controlType = ControlType::Classic;
                break;
            case Message::UIEvent::RequestControllerEnable:
                m_gameSettings.controllerEnabled = true;
                break;
            case Message::UIEvent::RequestControllerDisable:
                m_gameSettings.controllerEnabled = false;
                break;
            case Message::UIEvent::RequestColourblindEnable:
                m_gameSettings.colourblindMode = true;
                break;
            case Message::UIEvent::RequestColourblindDisable:
                m_gameSettings.colourblindMode = false;
                break;*/
            default: break;
            }
            break;
        default: break;
        }

        m_stateStack.handleMessage(msg);
    }
}

void App::updateApp(float dt)
{
    //dt -     const float timePerFrame = 1.f / 60.f;
    m_stateStack.update(dt);
}

void App::pauseApp(float dt)
{
    //do nothing, we're paused!
}

void App::draw()
{
    m_renderWindow.clear(sf::Color::Black);
    m_stateStack.draw();
    
    //BOGEYMAN
    /*
    ImGui::SFML::Update(m_renderWindow, frameClock.restart());
    ImGui::Begin("ImGui from APP");
    ImGui::Text("ImGui TEXT from APP");
    ImGui::End();
    ImGui::SFML::Render(m_renderWindow);
    */

    m_renderWindow.display();
}

void App::registerStates()
{
    m_stateStack.registerState<MenuState>(States::ID::Menu);
    m_stateStack.registerState<SfmlImguiDemo>(States::ID::Main);
    m_stateStack.registerState<RacingTopDown>(States::ID::Racing);
    m_stateStack.registerState<HorseRace>(States::ID::HorseRace);
    //m_stateStack.registerState<TitleState>(States::ID::Title);
}