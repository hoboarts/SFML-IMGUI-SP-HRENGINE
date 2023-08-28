#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML & IMGUI");
    ImGui::SFML::Init(window);

    bool circleExists = true;
    float circleRadius = 200.0f;
    int circleSegments = 100;
    float circleColor[4] = { static_cast<float>(204) / 255, (float)77 / 255, (float)5/ 255, (float)255 / 255 };
    float backgroundColor[3] = { (float)78 / 255, (float)89 / 255, (float)43 / 255 };

    sf::CircleShape shape(circleRadius, circleSegments);
    shape.setFillColor(sf::Color
    (
        (int)(circleColor[0] * 255),
        (int)(circleColor[1] * 255),
        (int)(circleColor[2] * 255),
        (int)(circleColor[3] * 255)
    )); // Color circle
    shape.setOrigin(circleRadius,circleRadius);
    
    shape.setPosition(400, 400); // Center circle

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Window title");
        ImGui::Text("Window text");
        ImGui::Checkbox("Circle", &circleExists);
        ImGui::Text("Govno");
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

        window.clear(sf::Color
        (
            (int)(backgroundColor[0] * 255),
            (int)(backgroundColor[1] * 255),
            (int)(backgroundColor[2] * 255)
        )); // Color background
        
        if (circleExists)
            window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}