// RACING TOP DOWN GAME

#ifndef RACING_TOP_DOWN_HPP_
#define RACING_TOP_DOWN_HPP_

#include <State.hpp>

//#include "imgui.h"
//#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <math.h>

namespace
{
    static float scaleFactor = 3.;

    const int navCheckPointsCount = 4; //checkpoints

    float navCheckPoints[navCheckPointsCount][2] = { 232., 550.,
                      436., 550.0,
                      436., 905.3,
                      232.6, 905.3};

    const float planTurnSpeed = 0.09;
    float speed = 0, angle = 0;
    float maxSpeed = 5.0, maxBackSpeed = 2.0;
    float acc = 0.1, dec = 0.2;
    float turnSpeed = planTurnSpeed;

    float angleMultpl = 180.0;//mod

    float offsetX = 0., offsetY = 0.;
    float collisionRadius = 24 / scaleFactor;// COLLISION RADIUS 22
    const int carsCount = 4;

    sf::Vector2u backgroundSize;

    static bool focusCameraOnCar = true;
    static bool activateDragCameraV2 = true;
    static bool activateBorderScrollCameraV1 = true;
    static bool drawSfmlDebug = true;
    static bool carControl = true;
    float accCam = 0.;

    static bool mousePressedLock = true;
    static int lastMousePosX = 0;
    static int lastMousePosY = 0;
    static int lastMousePosXLocal = 0;
    static int lastMousePosYLocal = 0;

    static const float rectSize = 50.;
    static sf::Vector2f centerVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f leftVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f leftopVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f topVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f rightopVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f rightVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f rightbottomVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f bottomVKPosition = sf::Vector2f(0., 0.);
    static sf::Vector2f leftbottomVKPosition = sf::Vector2f(0., 0.);

    static int pulsar = 1;
    static bool pulsarToggle = true;

    static int deltaTime = 0;
}

class MessageBus;
class RacingTopDown final : public State
{
public:
    struct Car
    {
        float x{}, y{}, speed; double angle; int targetCheckpoint;

        Car() { speed = 0; angle = 0; targetCheckpoint = 0; }

        void move()//ALL CAR PHISICS HERE
        {
            x += sin(angle) * speed;
            y -= cos(angle) * speed;
        }

        void findTarget()
        {
            float tx = navCheckPoints[targetCheckpoint][0];
            float ty = navCheckPoints[targetCheckpoint][1];
            float beta = angle - atan2(tx - x, -ty + y);
            if (sin(beta) < 0) angle += 0.005 * speed; else angle -= 0.005 * speed;
            if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) targetCheckpoint = (targetCheckpoint + 1) % navCheckPointsCount;
        }
    };

    struct Point
    {
        float x{}, y{}; int n;
        Point() { x = 0; y = 0; n = 0; }
    };

    /*
    struct Chrono
    {
        sf::Clock clock;

        std::string showSecondsStr()
        {
            sf::Time t = clock.getElapsedTime();
            int intS = (int)t.asSeconds();
            std::string str = std::to_string(intS);
            return str;
        }

        int& showSecondsInt()
        {
            sf::Time t = clock.getElapsedTime();
            int intS = (int)t.asSeconds();
            int& ptr = intS;
            return ptr;
        }

        float& showSecondsFloat()
        {
            sf::Time t = clock.getElapsedTime();
            float floatS = t.asSeconds();
            float& ptr = floatS;
            return ptr;
        }

        int& showMillisecondsInt()
        {
            sf::Time t = clock.getElapsedTime();
            int intS = (int)t.asMilliseconds();
            int& ptr = intS;
            return ptr;
        }

        void restartChrono()
        {
            clock.restart();
        }
        
    };
    */

    enum class VirtualKeys
    {
        Center,
        Left,
        Leftop,
        Top,
        Rightop,
        Right,
        Rightbottom,
        Bottom,
        Leftbottom
    };

    RacingTopDown(StateStack& stateStack, Context context);
    ~RacingTopDown();// = default;

    bool update(float dt) override;
    void draw() override;
    bool handleEvent(const sf::Event& evt) override;
    void handleMessage(const Message&) override;

private:
    MessageBus& m_messageBus;

    sf::Font m_font;

    sf::Texture m_paralaxTexture;
    sf::Texture m_backgroundTexture;
    sf::Texture m_pointTexture;
    sf::Texture m_carRedTexture;
    sf::Texture m_carYellowTexture;
    sf::Texture m_carBlueTexture;
    sf::Texture m_carGreenTexture;

    sf::Sprite m_paralaxSprite;
    sf::Sprite m_backgroundSprite;
    sf::Sprite m_pointSprite;
    sf::Sprite m_carSprite;

    //Chrono m_chrono;

    std::vector<sf::Texture> m_carsTexturesArr;
    std::vector<Car> m_cars;
    std::vector<Point> m_points;

    void drawRacing();

    template <typename T>
    void drawHudElement(sf::Font& font, const char* subj, T value, int hudVerticalPosition);

    void drawDebugRect(float width = 50., float height = 50., sf::Color fillCr = sf::Color(0, 0, 0, 0), sf::Color outlCr = sf::Color(255, 0, 0, 255), float posX = 0., float posY = 0., bool centerRect = false);

    void imGuiDebugWindow(float dt);
    void cameraControll();
    void dragCamera(VirtualKeys key = VirtualKeys::Center);

    void mouseDeltaCalc(const sf::Event& evt);
    void drawDebugElem();

    void pulsarLogic();
};

#endif //RACING_TOP_DOWN_HPP_