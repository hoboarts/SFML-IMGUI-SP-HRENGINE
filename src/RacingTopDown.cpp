#include "RacingTopDown.hpp"
#include <App.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

RacingTopDown::RacingTopDown(StateStack& stateStack, Context context)
    : State(stateStack, context),
    m_messageBus(context.appInstance.getMessageBus()),
    m_font(),
    m_paralaxTexture(),
    m_backgroundTexture(),
    m_pointTexture(),
    m_carRedTexture(),
    m_carYellowTexture(),
    m_carBlueTexture(),
    m_carGreenTexture(),
    m_paralaxSprite(),
    m_backgroundSprite(),
    m_pointSprite(),
    m_carSprite(),
    m_carsTexturesArr(),
    m_cars()
    //m_chrono()
{
    ImGui::SFML::Init(getContext().renderWindow);
    auto& font = context.appInstance.getFont("assets/fonts/sansation.ttf");
    m_font = font;
    
    m_paralaxTexture.loadFromFile("assets/images/background_parallax.png");
    m_paralaxTexture.setRepeated(true);

    m_backgroundTexture.loadFromFile("assets/images/background_new2.png");
    backgroundSize = m_backgroundTexture.getSize();

    m_pointTexture.loadFromFile("assets/images/pointn.png");

    m_carRedTexture.loadFromFile("assets/images/car_red.png");
    m_carYellowTexture.loadFromFile("assets/images/car_yellow.png");
    m_carBlueTexture.loadFromFile("assets/images/car_blue.png");
    m_carGreenTexture.loadFromFile("assets/images/car_green.png");

    //m_paralaxTexture.setSmooth(true);
    //m_backgroundTexture.setSmooth(true);
    //m_pointTexture.setSmooth(true);
    //m_carRedTexture.setSmooth(true);
    //m_carYellowTexture.setSmooth(true);
    //m_carBlueTexture.setSmooth(true);
    //m_carGreenTexture.setSmooth(true);

    m_carsTexturesArr.push_back(m_carRedTexture);
    m_carsTexturesArr.push_back(m_carYellowTexture);
    m_carsTexturesArr.push_back(m_carBlueTexture);
    m_carsTexturesArr.push_back(m_carGreenTexture);

    const sf::Vector2u displayRes = sf::Vector2u(getContext().appInstance.getVideoSettings().VideoMode.width, getContext().appInstance.getVideoSettings().VideoMode.height);
    m_paralaxSprite.setTextureRect(sf::IntRect(0, 0, backgroundSize.x - displayRes.x / scaleFactor, backgroundSize.y - displayRes.y / scaleFactor));
    m_paralaxSprite.scale(scaleFactor, scaleFactor);
    m_backgroundSprite.scale(scaleFactor, scaleFactor);

    m_pointSprite.setOrigin(6, 6);
    m_pointSprite.scale(scaleFactor, scaleFactor);

    m_carSprite.setOrigin(8, 8);// COLLISION RADIUS 18,24 - front wheel, 18,29 - fwd wheel, 18,34 - rear wheel
    m_carSprite.scale(scaleFactor, scaleFactor);

    for (int i = 0; i < carsCount; i++)
    {
        Car car;
        m_cars.push_back(car);
    }

    for (int i = 0; i < navCheckPointsCount; i++)
    {
        Point dpoint;
        dpoint.x = navCheckPoints[i][0];
        dpoint.y = navCheckPoints[i][1];
        m_points.push_back(dpoint);
    }

    for (int i = 0; i < carsCount; i++)//SET ALL CARS ON START POSITION
    {
        m_cars[i].x = 120 + i * 65;//POSITIONS CARS NAV ON START LANE
        m_cars[i].y = 890;
        m_cars[i].speed = 3;//7+i; //'AI' CAR SPEED
    }
}

RacingTopDown::~RacingTopDown()
{
    ImGui::SFML::Shutdown();
}

//public

bool RacingTopDown::update(float dt)
{
    deltaTime += 2;
    
    
    if (deltaTime > 255)
    {
        deltaTime = 0;
        if (pulsarToggle)
        {
            pulsarToggle = false;
        }
        else
        {
            pulsarToggle = true;
        }
    }
    
	return true;
}

void RacingTopDown::draw()
{
    //SHIT
    /*
    auto& rw = getContext().renderWindow;
    rw.setView(getContext().renderWindow.getDefaultView());
    //rw.draw(m_scene);

    rw.setView(getContext().defaultView);
    //rw.draw(m_vignette, sf::BlendMultiply);
    */

	drawRacing();
    imGuiDebugWindow(1.f / 60.f);
}

bool RacingTopDown::handleEvent(const sf::Event& evt)
{
	ImGui::SFML::ProcessEvent(getContext().renderWindow,evt);
    mouseDeltaCalc(evt);
	
    return false; //consume events
}

void RacingTopDown::handleMessage(const Message& msg)
{}

//private

void RacingTopDown::drawRacing()
{
    auto& rw = getContext().renderWindow;

    bool Up = 0, Right = 0, Down = 0, Left = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        Up = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        Right = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        Down = 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        Left = 1;
    }

    //-------------------------------------------------------
    //player car[0] movement
    if (carControl)
    {
        if (Up && speed < maxSpeed)//acceleration
        {
            if (speed < 0)
            {
                speed += dec;//from rear to front moving
            }
            else
            {
                speed += acc;
            }
        }

        if (speed > 18)//try phisic mod
        {
            if (Right)
            {
                angleMultpl = 200.0;
            }
            else if (Left) {
                angleMultpl = 200.0;
            }
        }
        else
        {
            if (angleMultpl > 180.0)
            {
                angleMultpl -= 2.0;
            }
        }

        if (angleMultpl > 180.0)
        {
            if (speed > 10)
            {
                speed -= 0.23;
            }
        }

        if (Down && speed > -maxBackSpeed)//brake
        {
            if (speed > 0)
            {
                speed -= 0.3;//brake - front move
            }
            else
            {
                speed -= acc;//brake - rear move
            }
        }

        if (!Up && !Down)//inertia
        {
            if (speed - dec > 0)
            {
                speed -= dec;//auto brake front
            }
            else if (speed + dec < 0)
            {
                speed += dec;//auto brake rear
            }
            else
            {
                speed = 0;// prevent idle move-stop
            }
        }

        //MOD
        if (speed >= 4)
        {
            turnSpeed -= 0.001;
            if (turnSpeed <= 0.02)
            {
                turnSpeed = 0.02;
            }
        }
        else
        {
            if (turnSpeed < planTurnSpeed)
            {
                turnSpeed += 0.005;
            }
        }

        if (Right && speed != 0)
        {
            angle += turnSpeed * speed / maxSpeed;
        }

        if (Left && speed != 0)
        {
            angle -= turnSpeed * speed / maxSpeed;
        }

        //DEBUG---------------------
        /*
        if (Right)
        {
            angle += 0.1;
        }
        if (Left)
        {
            angle -= 0.1;
        }
        */
        //----------------------------------------------

        if (angle > 6.4 || angle < -6.4)
        {
            angle = 0;//MOD
        }

        m_cars[0].speed = speed;// car[0] - player car
        m_cars[0].angle = angle;
    }
    else
    {
        if (!focusCameraOnCar)
        {
            if (Left)
            {
                offsetX = offsetX - 2.;
            }
            if (Right)
            {
                offsetX = offsetX + 2.;
            }
            if (Up)
            {
                offsetY = offsetY - 2.;
            }
            if (Down)
            {
                offsetY = offsetY + 2.;
            }
        }
    }
    //----------------------------------------------

    for (int i = 0; i < carsCount; i++)
    {
        m_cars[i].move();
    }

    for (int i = 1; i < carsCount; i++)
    {
        m_cars[i].findTarget();
    }

    //----------------------------------------------
    //collision
    for (int i = 0; i < carsCount; i++)
    {
        for (int j = 0; j < carsCount; j++)
        {
            int dx = 0, dy = 0;
            while (dx * dx + dy * dy < 4 * collisionRadius * collisionRadius)
            {
                m_cars[i].x += dx / 10.0;
                m_cars[i].x += dy / 10.0;
                m_cars[j].x -= dx / 10.0;
                m_cars[j].y -= dy / 10.0;
                dx = m_cars[i].x - m_cars[j].x;
                dy = m_cars[i].y - m_cars[j].y;
                if (!dx && !dy) break;
            }
        }
    }

    rw.clear(sf::Color::White);

    cameraControll();

    m_paralaxSprite.setPosition((- offsetX * 0.3) * scaleFactor, ( - offsetY * 0.3) * scaleFactor);
    m_paralaxSprite.setTexture(m_paralaxTexture, false);
    rw.draw(m_paralaxSprite);//mod

    m_backgroundSprite.setPosition(-offsetX * scaleFactor, -offsetY * scaleFactor);
    m_backgroundSprite.setTexture(m_backgroundTexture, true);
    rw.draw(m_backgroundSprite);

    /*
    sf::Color colors[11] = {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Magenta,
        sf::Color::Blue,
        sf::Color::White,
        sf::Color(13,88,65,255),
        sf::Color(136,44,213,255),
        sf::Color(255,107,176,255),
        sf::Color(255,252,0,255),
        sf::Color(0,250,255,255),
        sf::Color(190,4,203,255)
    };
    */

    for (int i = 0; i < carsCount; i++)
    {
        m_carSprite.setPosition((m_cars[i].x - offsetX)* scaleFactor, (m_cars[i].y - offsetY)* scaleFactor);
        m_carSprite.setRotation(m_cars[i].angle * angleMultpl / 3.141593);//180 - angleMultpl
        //m_carSprite.setColor(colors[i]);
        m_carSprite.setTexture(m_carsTexturesArr[i], true);
        rw.draw(m_carSprite);
    }

    for (int i = 0; i < navCheckPointsCount; i++)
    {
        m_pointSprite.setPosition((m_points[i].x - offsetX) * scaleFactor, (m_points[i].y - offsetY) * scaleFactor);//mod
        m_pointSprite.setTexture(m_pointTexture, true);
        rw.draw(m_pointSprite);//mod
    }

    if (drawSfmlDebug)
    {
    drawHudElement(m_font, "Speed ", m_cars[0].speed, 20);//hudText
    drawHudElement(m_font, "Angle ", m_cars[0].angle, 50);//statusText
    drawHudElement(m_font, "X coord ", m_cars[0].x, 80);//debugOne
    drawHudElement(m_font, "Y coord ", m_cars[0].y, 110);//debugTwo
    drawHudElement(m_font, "Turn speed ", turnSpeed, 140);//debugThree
    drawHudElement(m_font, "Angle ", angleMultpl, 170);//debugFour
    drawHudElement(m_font, "Time ", deltaTime, 200);//debugFour

    drawDebugElem();//DEBUG
    }
}

template <typename T>//THIS IS DIZDEC!
void RacingTopDown::drawHudElement(sf::Font& font,const char *subj,T value, int hudVerticalPosition)
{
    auto& rw = getContext().renderWindow;
    sf::Text elem;
    elem.setFont(font);
    elem.setCharacterSize(28);
    elem.setFillColor(sf::Color::White);
    elem.setOutlineColor(sf::Color::Black);
    elem.setOutlineThickness(2.0f);

    elem.setPosition(20, hudVerticalPosition);

    elem.setString(subj + std::to_string(value));

    rw.draw(elem);
}

void RacingTopDown::drawDebugRect(float width, float height, sf::Color fillCr, sf::Color outlCr, float posX, float posY, bool centerRect)
{
    auto& rw = getContext().renderWindow;
    sf::RectangleShape rectShape(sf::Vector2f(width,height));//50x50
    rectShape.setFillColor(fillCr);
    rectShape.setOutlineColor(outlCr);
    rectShape.setOutlineThickness(2.);
    if(centerRect)
    {
        rectShape.setOrigin(width / 2, height / 2);
    }
    rectShape.setPosition(posX, posY);
    rw.draw(rectShape);
}

void RacingTopDown::imGuiDebugWindow(float dt)
{
    ImGui::SFML::Update(getContext().renderWindow, static_cast<sf::Time>(sf::seconds(dt)));

    ImGui::Begin("DEBUG");

    static int clicked = 0;
    if (ImGui::Button("Change to imgui scene"))
        clicked++;
    if (clicked & 1)
    {
        requestStackPop();
        requestStackPush(States::ID::Main);
    }

    ImGui::Text("");
    
    ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.5f, 1.0f), "IMGUI");
    ImGui::SameLine();
    ImGui::SeparatorText("mouse positions");
    ImGui::Text("ImGui mouse pos: (x: %g, y: %g)", ImGui::GetIO().MousePos.x + offsetX, ImGui::GetIO().MousePos.y + offsetY);
    ImGui::Text("ImGui mouse pos no offset: (x: %g, y: %g)", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::Text("ImGui mouse delta: (x: %g, y: %g)", ImGui::GetIO().MouseDelta.x, ImGui::GetIO().MouseDelta.y);
    ImGui::Text("Accamulate cam %f", accCam);

    ImGui::Text("");

    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "SFML");
    ImGui::SameLine();
    ImGui::SeparatorText("mouse positions");
    ImGui::Text("Sfml mouse pos: (x: %g, y: %g)", sf::Mouse::getPosition(getContext().renderWindow).x + offsetX, sf::Mouse::getPosition(getContext().renderWindow).y + offsetY);
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Sfml REAL mouse pos with scale: (x: %g, y: %g) <<<", (sf::Mouse::getPosition(getContext().renderWindow).x / scaleFactor) + offsetX, (sf::Mouse::getPosition(getContext().renderWindow).y / scaleFactor) + offsetY);
    
    //ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "Sfml REAL WITH VIEWPORT mouse pos with scale: (x: %g, y: %g) <", getContext().defaultView.getSize().x, getContext().defaultView.getSize().y);
    
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f),"Sfml mouse pos no offset: (x: %d, y: %d)", sf::Mouse::getPosition(getContext().renderWindow).x, sf::Mouse::getPosition(getContext().renderWindow).y);
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "GLOBAL");
    ImGui::SameLine();
    ImGui::Text("Sfml mouse pos: (x: %g, y: %g)", sf::Mouse::getPosition().x + offsetX, sf::Mouse::getPosition().y + offsetY);
    ImGui::TextColored(ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "GLOBAL");
    ImGui::SameLine();
    ImGui::Text("Sfml mouse pos no offset: (x: %d, y: %d)", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
    ImGui::Text("Last mouse pos: (x: %d, y: %d)", lastMousePosX, lastMousePosY);
    ImGui::Text("Offset: (x: %f, y: %f)", offsetX, offsetY);
    ImGui::Text("CAMERA STOP: (x: %g y: %g)", backgroundSize.x * scaleFactor - getContext().appInstance.getVideoSettings().VideoMode.width, backgroundSize.y * scaleFactor - getContext().appInstance.getVideoSettings().VideoMode.height);
    ImGui::Text("CAMERA CENTER: (x: %g y: %g)", (getContext().appInstance.getVideoSettings().VideoMode.width / 2) / scaleFactor, (getContext().appInstance.getVideoSettings().VideoMode.height / 2) / scaleFactor);
    ImGui::Text("Display res (x: %d, y: %d)", getContext().appInstance.getVideoSettings().VideoMode.width, getContext().appInstance.getVideoSettings().VideoMode.height);
    ImGui::Text("Paralax (x: %d, y: %d)", getContext().appInstance.getVideoSettings().VideoMode.width * 2, getContext().appInstance.getVideoSettings().VideoMode.height * 2);
    ImGui::Text("BACKGROUND: (x: %d y: %d)", backgroundSize.x, backgroundSize.y);
    ImGui::Text("LIMIT: (x: %g y: %g)", backgroundSize.x * scaleFactor - getContext().appInstance.getVideoSettings().VideoMode.width, backgroundSize.y * scaleFactor - getContext().appInstance.getVideoSettings().VideoMode.height);

    sf::Vector2f paralaxScale = m_paralaxSprite.getScale();
    sf::Vector2f backgrndScale = m_backgroundSprite.getScale();
    sf::Vector2f carScale = m_carSprite.getScale();
    sf::Vector2f navpointScale = m_pointSprite.getScale();
    ImGui::Text("Scales (paralax: x: %g y: %g, backgrnd: x: %g y: %g, car: x: %g y: %g, navpoint: x: %g y: %g)", paralaxScale.x, paralaxScale.y, backgrndScale.x, backgrndScale.y, carScale.x, carScale.y, navpointScale.x, navpointScale.y);

    ImGui::Text("ABIR: %f)", getContext().defaultView.getRotation());

    ImGui::Text("");
    ImGui::SeparatorText("Time");
    ImGui::Text("Seconds: %d", deltaTime);//m_chrono.showSecondsInt()
    //ImGui::Text("Milliseconds: %d", m_chrono.showMillisecondsInt());
    ImGui::Text("Pulsar ~256: %d", pulsar);
    pulsarLogic();

    ImGui::Checkbox("Focus camera on car", &focusCameraOnCar);
    ImGui::SameLine();
    ImGui::Checkbox("Activate camera drag v2", &activateDragCameraV2);
    ImGui::Checkbox("Activate camera border scroll v1", &activateBorderScrollCameraV1);
    ImGui::SameLine();
    ImGui::Checkbox("Draw SFML DEBUG", &drawSfmlDebug);
    ImGui::Checkbox("Car control ON", &carControl);

    ImGui::End();

    ImGui::SFML::Render(getContext().renderWindow);
}

void RacingTopDown::cameraControll()
{
    float accCamAsc = 0.1;
    float maxAccCam = 10.;
    int borderEdgeScrollActivator = 50;
    const sf::Vector2u displayRes = sf::Vector2u(getContext().appInstance.getVideoSettings().VideoMode.width, getContext().appInstance.getVideoSettings().VideoMode.height);
    //-----------------------------------------------------------------------------------------------
    // CAMERA CENTER WITH FOCUS
    if (m_cars[0].x > (displayRes.x / 2) / scaleFactor && focusCameraOnCar)
    {
        offsetX = m_cars[0].x - (displayRes.x / 2) / scaleFactor; //CENTER CAR CAMERA
    }
    if (m_cars[0].y > (displayRes.y / 2) / scaleFactor && focusCameraOnCar)
    {
        offsetY = m_cars[0].y - (displayRes.y / 2) / scaleFactor; //CENTER CAR CAMERA
    }
    //-----------------------------------------------------------------------------------------------
    // CAMERA BORDER SCROLL WITH MOUSE NO FOCUS
    if (!focusCameraOnCar && activateBorderScrollCameraV1)
    {
        if (sf::Mouse::getPosition(getContext().renderWindow).x < borderEdgeScrollActivator)
        {
            offsetX = offsetX - accCam; // BORDER SCROLL LEFT
            if (sf::Mouse::getPosition(getContext().renderWindow).y < borderEdgeScrollActivator)
            {
                offsetY = offsetY - accCam; // BORDER SCROLL LEFT TOP
            }
            if (sf::Mouse::getPosition(getContext().renderWindow).y > (displayRes.y - borderEdgeScrollActivator))
            {
                offsetY = offsetY + accCam; // BORDER SCROLL LEFT BOTTOM
            }
            if (accCam < maxAccCam)
            {
                accCam = accCam + accCamAsc;
            }
        }
        else if (sf::Mouse::getPosition(getContext().renderWindow).x > (displayRes.x - borderEdgeScrollActivator))
        {
            offsetX = offsetX + accCam; // BORDER SCROLL RIGHT
            if (sf::Mouse::getPosition(getContext().renderWindow).y < borderEdgeScrollActivator)
            {
                offsetY = offsetY - accCam; // BORDER SCROLL RIGHT TOP
            }
            if (sf::Mouse::getPosition(getContext().renderWindow).y > (displayRes.y - borderEdgeScrollActivator))
            {
                offsetY = offsetY + accCam; // BORDER SCROLL RIGHT BOTTOM
            }
            if (accCam < maxAccCam)
            {
                accCam = accCam + accCamAsc;
            }
        }
        else if (sf::Mouse::getPosition(getContext().renderWindow).y < borderEdgeScrollActivator)
        {
            offsetY = offsetY - accCam; // BORDER SCROLL TOP
            if (accCam < maxAccCam)
            {
                accCam = accCam + accCamAsc;
            }
        }
        else if (sf::Mouse::getPosition(getContext().renderWindow).y > (displayRes.y - borderEdgeScrollActivator))
        {
            offsetY = offsetY + accCam; // BORDER SCROLL BOTTOM
            if (accCam < maxAccCam)
            {
                accCam = accCam + accCamAsc;
            }
        }
        else if (accCam > 0. && mousePressedLock)
        {
            accCam = 0.;
        }
    }
    //---------------------------------------------------------------------------------------------------------
    // CAMERA STOP
    if (offsetX > backgroundSize.x - displayRes.x / scaleFactor && focusCameraOnCar)
    {
        offsetX = backgroundSize.x - displayRes.x / scaleFactor;// MOD CAMERA BORDER STOP 2240, 1880
    }
    
    if (offsetY > backgroundSize.y - displayRes.y / scaleFactor && focusCameraOnCar)
    {
        offsetY = backgroundSize.y - displayRes.y / scaleFactor;// MOD CAMERA BORDER STOP 3168, 3028
    }
    //---------------------------------------------------------------------------------------------------------
    // CAMERA STOP NO FOCUS
    if (offsetX <= -0. && !focusCameraOnCar)
    {
        offsetX = 0.;//MOD CAMERA STOP <0
    }
    if (offsetY <= -0. && !focusCameraOnCar)
    {
        offsetY = 0.;
    }
    if (offsetX >= backgroundSize.x * scaleFactor - displayRes.x && !focusCameraOnCar)
    {
        offsetX = backgroundSize.x * scaleFactor - displayRes.x;
    }
    if (offsetY > backgroundSize.y * scaleFactor - displayRes.y && !focusCameraOnCar)
    {
        offsetY = backgroundSize.y * scaleFactor - displayRes.y;// MOD CAMERA BORDER STOP 3168, 3028
    }
    //---------------------------------------------------------------------------------------------------------
    //CAMERA DRAG NEW
    if (!focusCameraOnCar && !mousePressedLock && activateDragCameraV2)
    {
        float widthFromCenterToLeftBorder = lastMousePosXLocal - rectSize + (rectSize / 2);
        float widthFromTopToLeftBorder = lastMousePosXLocal - (rectSize / 2);
        float heightFromTopToLeftCenterBorder = lastMousePosYLocal - (rectSize / 2);
        //float widthFromCenterToRightBorder = displayRes[0] - lastMousePosXLocal;

        sf::Vector2i currentMousePos{sf::Mouse::getPosition(getContext().renderWindow).x, sf::Mouse::getPosition(getContext().renderWindow).y};

        if (
            currentMousePos.x >= centerVKPosition.x 
            && currentMousePos.y >= centerVKPosition.y 
            && currentMousePos.x <= centerVKPosition.x + rectSize 
            && currentMousePos.y <= centerVKPosition.y + rectSize
            )
        {
            dragCamera(VirtualKeys::Center);
        }
        else if(
            currentMousePos.x >= leftVKPosition.x
            && currentMousePos.y >= leftVKPosition.y
            && currentMousePos.x <= widthFromCenterToLeftBorder + rectSize
            && currentMousePos.y <= leftVKPosition.y + rectSize
        )
        {
            dragCamera(VirtualKeys::Left);
        }
        else if (
            currentMousePos.x >= leftopVKPosition.x
            && currentMousePos.y >= leftopVKPosition.y
            && currentMousePos.x <= widthFromTopToLeftBorder
            && currentMousePos.y <= heightFromTopToLeftCenterBorder + rectSize
            )
        {
            dragCamera(VirtualKeys::Leftop);
        }
        else if (
            currentMousePos.x >= topVKPosition.x
            && currentMousePos.y >= topVKPosition.y
            && currentMousePos.x <= widthFromTopToLeftBorder + rectSize
            && currentMousePos.y <= heightFromTopToLeftCenterBorder + rectSize
            )
        {
            dragCamera(VirtualKeys::Top);
        }
        else if (
            currentMousePos.x >= rightopVKPosition.x
            && currentMousePos.y >= rightopVKPosition.y
            && currentMousePos.x <= displayRes.x
            && currentMousePos.y <= heightFromTopToLeftCenterBorder
            )
        {
            dragCamera(VirtualKeys::Rightop);
        }
        else if (
            currentMousePos.x >= rightVKPosition.x
            && currentMousePos.y >= rightVKPosition.y
            && currentMousePos.x <= displayRes.x
            && currentMousePos.y <= heightFromTopToLeftCenterBorder + rectSize
            )
        {
            dragCamera(VirtualKeys::Right);
        }
        else if (
            currentMousePos.x >= rightbottomVKPosition.x
            && currentMousePos.y >= rightbottomVKPosition.y
            && currentMousePos.x <= displayRes.x
            && currentMousePos.y <= displayRes.y
            )
        {
            dragCamera(VirtualKeys::Rightbottom);
        }
        else if (
            currentMousePos.x >= bottomVKPosition.x
            && currentMousePos.y >= bottomVKPosition.y
            && currentMousePos.x <= displayRes.x
            && currentMousePos.y <= displayRes.y
            )
        {
            dragCamera(VirtualKeys::Bottom);
        }
        else if (
            currentMousePos.x >= leftbottomVKPosition.x
            && currentMousePos.y >= leftbottomVKPosition.y
            && currentMousePos.x <= widthFromTopToLeftBorder
            && currentMousePos.y <= displayRes.y
            )
        {
            dragCamera(VirtualKeys::Leftbottom);
        }
    }
}

void RacingTopDown::dragCamera(VirtualKeys key)
{
    float accCamAsc = 0.1;
    float maxAccCam = 10.;

    switch (key)
    {
    case VirtualKeys::Center:
        accCam = 0;
        break;
    case VirtualKeys::Left:
        if(offsetX > 0)
        {
            offsetX = offsetX - accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Leftop:
        if (offsetX > 0)
        {
            offsetX = offsetX - accCam;
        }
        if (offsetY > 0)
        {
            offsetY = offsetY - accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Top:
        if (offsetY > 0)
        {
            offsetY = offsetY - accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Rightop:
        if (offsetX < backgroundSize.x * scaleFactor)
        {
            offsetX = offsetX + accCam;
        }
        if (offsetY > 0)
        {
            offsetY = offsetY - accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Right:
        if (offsetX < backgroundSize.x * scaleFactor)
        {
            offsetX = offsetX + accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Rightbottom:
        if (offsetX < backgroundSize.x * scaleFactor)
        {
            offsetX = offsetX + accCam;
        }
        if (offsetY < backgroundSize.y * scaleFactor)
        {
            offsetY = offsetY + accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Bottom:
        if (offsetY < backgroundSize.y * scaleFactor)
        {
            offsetY = offsetY + accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    case VirtualKeys::Leftbottom:
        if (offsetX > 0)
        {
            offsetX = offsetX - accCam;
        }
        if (offsetY < backgroundSize.y * scaleFactor)
        {
            offsetY = offsetY + accCam;
        }
        if (accCam < maxAccCam)
        {
            accCam = accCam + accCamAsc;
        }
        break;
    default:
        accCam = 0;
        break;
    }
}

void RacingTopDown::mouseDeltaCalc(const sf::Event& evt)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))//&& evt.type == sf::Event::MouseMoved
    {
        if (mousePressedLock)
        {
            lastMousePosX = sf::Mouse::getPosition().x;
            lastMousePosY = sf::Mouse::getPosition().y;
            lastMousePosXLocal = sf::Mouse::getPosition(getContext().renderWindow).x;
            lastMousePosYLocal = sf::Mouse::getPosition(getContext().renderWindow).y;

            centerVKPosition.x = lastMousePosXLocal - rectSize + (rectSize / 2);//center
            centerVKPosition.y = lastMousePosYLocal - (rectSize / 2);

            leftVKPosition.x = 0;//left
            leftVKPosition.y = lastMousePosYLocal - (rectSize / 2);

            leftopVKPosition.x = 0;//leftop
            leftopVKPosition.y = 0;

            topVKPosition.x = lastMousePosXLocal - (rectSize / 2);//top
            topVKPosition.y = 0;
            
            rightopVKPosition.x = lastMousePosXLocal + (rectSize / 2);//rightop
            rightopVKPosition.y = 0;

            rightVKPosition.x = lastMousePosXLocal + (rectSize / 2);//right
            rightVKPosition.y = lastMousePosYLocal - (rectSize / 2);

            rightbottomVKPosition.x = lastMousePosXLocal + (rectSize / 2);//rightbottom
            rightbottomVKPosition.y = lastMousePosYLocal + rectSize - (rectSize / 2);

            bottomVKPosition.x = lastMousePosXLocal - rectSize + (rectSize / 2);//bottom
            bottomVKPosition.y = lastMousePosYLocal + rectSize - (rectSize / 2);

            leftbottomVKPosition.x = 0;//leftbottom
            leftbottomVKPosition.y = lastMousePosYLocal + rectSize - (rectSize / 2);
        }
        mousePressedLock = false;
    }
    if (evt.type == sf::Event::MouseButtonReleased)
    {
        mousePressedLock = true;
        lastMousePosX = 0;
        lastMousePosY = 0;
    }
    if (evt.type == sf::Event::MouseWheelScrolled)
    {
        if (evt.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)// WHEEL SCROLLED
        {
            if (evt.mouseWheelScroll.delta == 1)
            {
                //Logger::Log("UP", Logger::Type::Info, Logger::Output::Console);
                //backgrndScale = backgrndScale - 1.;

                if (scaleFactor < 10)
                {
                    scaleFactor = scaleFactor + 0.1;
                    m_paralaxSprite.setScale(scaleFactor, scaleFactor);
                    m_backgroundSprite.setScale(scaleFactor, scaleFactor);
                    m_carSprite.setScale(scaleFactor, scaleFactor);
                    m_pointSprite.setScale(scaleFactor, scaleFactor);
                }
            }
            else
            {
                //Logger::Log("DOWN", Logger::Type::Info, Logger::Output::Console);
                //backgrndScale = backgrndScale + 1.;

                if (scaleFactor > 0)
                {
                    scaleFactor = scaleFactor - 0.1;
                    m_paralaxSprite.setScale(scaleFactor, scaleFactor);
                    m_backgroundSprite.setScale(scaleFactor, scaleFactor);
                    m_carSprite.setScale(scaleFactor, scaleFactor);
                    m_pointSprite.setScale(scaleFactor, scaleFactor);
                }
            }
        }

        /* WHEEL SCROLL STACKOVERFLOW
        if (event.type == sf::Event::MouseWheelScrolled)
        {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                std::cout << "wheel type: vertical" << std::endl;
            else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                std::cout << "wheel type: horizontal" << std::endl;
            else
                std::cout << "wheel type: unknown" << std::endl;

            std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
            std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
        }
        */
    }
}

void RacingTopDown::drawDebugElem()
{
    float widthFromCenterToLeftBorder = lastMousePosXLocal - rectSize + (rectSize / 2);
    float widthFromTopToLeftBorder = lastMousePosXLocal - (rectSize / 2);
    float heightFromTopToLeftCenterBorder = lastMousePosYLocal - (rectSize / 2);
    float widthFromCenterToRightBorder = getContext().appInstance.getVideoSettings().VideoMode.width - lastMousePosXLocal;
    float heightFromBottomToCenterBorder = getContext().appInstance.getVideoSettings().VideoMode.height - lastMousePosYLocal;

    if (!mousePressedLock)// DEBUG
    {
        drawDebugRect(rectSize, rectSize, sf::Color(60, 130, 255, 100), sf::Color(0, 0, 0, 0), centerVKPosition.x, centerVKPosition.y);//CENTER
        drawDebugRect(widthFromCenterToLeftBorder, rectSize, sf::Color(255, 255, 0, 100), sf::Color(0, 0, 0, 0), leftVKPosition.x, leftVKPosition.y);//LEFT
        drawDebugRect(widthFromTopToLeftBorder, heightFromTopToLeftCenterBorder, sf::Color(100, 180, 123, 100), sf::Color(0, 0, 0, 0), leftopVKPosition.x, leftopVKPosition.y);//LEFT TOP
        drawDebugRect(rectSize, heightFromTopToLeftCenterBorder, sf::Color(200, 20, 80, 100), sf::Color(0, 0, 0, 0), topVKPosition.x, topVKPosition.y);//TOP
        drawDebugRect(widthFromCenterToRightBorder, heightFromTopToLeftCenterBorder, sf::Color(100, 255, 130, 100), sf::Color(0, 0, 0, 0), rightopVKPosition.x, rightopVKPosition.y);//RIGHTOP
        drawDebugRect(widthFromCenterToRightBorder, rectSize, sf::Color(128, 128, 0, 100), sf::Color(0, 0, 0, 0), rightVKPosition.x, rightVKPosition.y);//RIGHT
        drawDebugRect(widthFromCenterToRightBorder, heightFromBottomToCenterBorder, sf::Color(20, 100, 255, 100), sf::Color(0, 0, 0, 0), rightbottomVKPosition.x, rightbottomVKPosition.y);//RIGHT BOTTOM
        drawDebugRect(rectSize, heightFromBottomToCenterBorder, sf::Color(49, 50, 190, 100), sf::Color(0, 0, 0, 0), bottomVKPosition.x, bottomVKPosition.y);//BOTTOM
        drawDebugRect(widthFromCenterToLeftBorder, heightFromBottomToCenterBorder, sf::Color(80, 255, 120, pulsar), sf::Color(0, 0, 0, 0), leftbottomVKPosition.x, leftbottomVKPosition.y);//BOTTOM LEFT
    }
}

void RacingTopDown::pulsarLogic()
{
    if (deltaTime > 0 && pulsarToggle)
    {
        pulsar = deltaTime;
    }
    else if (deltaTime > 0 && !pulsarToggle)
    {
        pulsar = 255 - deltaTime;
    }
}
