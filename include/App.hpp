#ifndef APP_HPP_
#define APP_HPP_

//#include <Utils.hpp>
#include <Log.hpp>

#include <StateStack.hpp>
#include <Resource.hpp>
//#include <Score.hpp>
#include <MessageBus.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>

class App final
{
public:
    struct VideoSettings final
    {
        sf::Int8 WindowStyle;// WHY Int32? int16, int8, uint8, int works fine
        sf::VideoMode VideoMode;
        bool VSync;
        std::vector<sf::VideoMode> AvailableVideoModes;

        VideoSettings()
            : WindowStyle(sf::Style::Close /*sf::Style::Fullscreen*/),
            VideoMode(1600, 920),
            VSync(true) {}

        bool operator == (const VideoSettings& vs)
        {
            if (&vs == this) return true;
            return
                (vs.VideoMode == this->VideoMode
                    && vs.VSync == this->VSync
                    && vs.WindowStyle == this->WindowStyle);
        }
    };

    App();
    ~App() = default;
    App(const App&) = delete;//NOT TO ALLOW COPY OF CLASS
    const App& operator = (const App&) = delete;// NOT TO ALLOW MOVE CLASS

    void run();
    void pause();
    void resume();

    sf::Font& getFont(const std::string& path);
    sf::Texture& getTexture(const std::string& path);
    MessageBus& getMessageBus();

    const VideoSettings& getVideoSettings() const;
    void applyVideoSettings(const VideoSettings&);

private:

    struct SettingsFile final
    {
        int ident;
        int version;

        sf::VideoMode videoMode;
        sf::Int32 windowStyle;
        //AudioSettings audioSettings;
        //GameSettings gameSettings;
    };

    VideoSettings m_videoSettings;
    sf::RenderWindow m_renderWindow;

    StateStack m_stateStack;

    FontResource m_fontResource;
    TextureResource m_textureResource;

    MessageBus m_messageBus;

    void handleEvents();
    void handleMessages();
    std::function<void(float)> update;
    void updateApp(float dt);
    void pauseApp(float dt);
    void draw();

    void registerStates();
};

#endif //APP_HPP_