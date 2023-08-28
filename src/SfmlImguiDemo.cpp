#include "SfmlImguiDemo.hpp"

#include <App.hpp>

namespace
{
    bool circleExists = true;
    float circleRadius = 200.0f;
    int circleSegments = 100;
    float circleColor[4] = { (float)204 / 255, (float)77 / 255, (float)5 / 255, (float)255 / 255 };
	float backgroundColor[3] = { (float)78 / 255, (float)89 / 255, (float)43 / 255 };

    const float timePerFrame = 1.f / 60.f;

    sf::CircleShape shape(circleRadius, circleSegments);
}

SfmlImguiDemo::SfmlImguiDemo(StateStack& stateStack, Context context)
	: State(stateStack, context),
	m_messageBus(context.appInstance.getMessageBus())
{
    auto& rw = getContext().renderWindow;
    ImGui::SFML::Init(rw);

    shape.setFillColor(sf::Color
    (
        (int)(circleColor[0] * 255),
        (int)(circleColor[1] * 255),
        (int)(circleColor[2] * 255),
        (int)(circleColor[3] * 255)
    )); // Color circle
    shape.setOrigin(circleRadius, circleRadius);

    shape.setPosition((float)1024/2, (float)768/2); // Center circle
}

SfmlImguiDemo::~SfmlImguiDemo()
{
	ImGui::SFML::Shutdown();
}

//public
bool SfmlImguiDemo::update(float dt)
{
    //updateDrawImgui(dt);
	return true;
}

void SfmlImguiDemo::draw()
{
    drawImguiDemo(timePerFrame);
}

bool SfmlImguiDemo::handleEvent(const sf::Event& evt)
{
    ImGui::SFML::ProcessEvent(evt);

    return false; //consume events
}

void SfmlImguiDemo::handleMessage(const Message& msg)
{
}

//private

void SfmlImguiDemo::drawImguiDemo(float dt)
{
    auto& rw = getContext().renderWindow;
    ImGui::SFML::Update(rw, static_cast<sf::Time>(sf::seconds(dt)));

    ImGui::Begin("Window title");
    ImGui::Text("Window text");
    ImGui::Checkbox("Circle", &circleExists);
    ImGui::TextColored(ImVec4(0.1f, 0.2f, 0.9f, 1.f), std::to_string(dt).c_str());
    ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
    ImGui::SliderInt("Sides", &circleSegments, 3, 150);
    ImGui::ColorEdit4("Color Circle", circleColor);
    ImGui::ColorEdit3("Color Background", backgroundColor);
    ImGui::End();

    shape.setRadius(circleRadius);
    shape.setOrigin(circleRadius, circleRadius);
    shape.setPointCount(circleSegments);
    shape.setFillColor(sf::Color
    (
        (int)(circleColor[0] * 255),
        (int)(circleColor[1] * 255),
        (int)(circleColor[2] * 255),
        (int)(circleColor[3] * 255)
    )); // Color circle

    rw.clear(sf::Color
    (
        (int)(backgroundColor[0] * 255),
        (int)(backgroundColor[1] * 255),
        (int)(backgroundColor[2] * 255)
    ));

    if (circleExists)
        rw.draw(shape);

    ImGui::SFML::Render(rw);
}