#pragma once
#include <fstream>
#include <iostream>

#include "CShape.h"
#include "RShape.h"
#include "settings.h"
#include "helper.h"

//to delete
#include "scollision.h"

using namespace std;

static MainSettings settings;
static sf::Font myFont;
static uint32_t m_CurrentFrame = 0;

static std::vector<CShape> sCircles;
static std::vector<RShape> sRectangles;

static void AddShape(RectangleSettings& rs)
{
    RShape rect(rs, settings, myFont);
    sRectangles.push_back(std::move(rect));
}

static void AddShape(CircleSettings& cs)
{
    CShape circle(cs, settings, myFont);
    sCircles.push_back(std::move(circle));
}

static void LoadConfig(MainSettings& settings)
{
    char filename[] = "config_1.txt";
    std::ifstream ifs(filename);

    if (ifs.is_open()) {
        std::string type;
        ifs >> type;
        while (!ifs.eof())
        {
            if (type == "Window")
            {
                int width, height;
                ifs >> width >> height;
                settings.mWindow.SetWidth(width);
                settings.mWindow.SetHeight(height);
                ifs >> type;
            }

            if (type == "Circle")
            {
                std::string name;
                float x, y, sx, sy, rad;
                int r, g, b;

                ifs >> name >> x >> y >> sx >> sy >> r >> g >> b >> rad;

                CircleSettings cs(x, y, rad);
                cs.SetColor(r, g, b);
                cs.SetName(name);
                cs.SetSpeed(sx, sy);
                AddShape(cs);
                ifs >> type;
            }

            if (type == "Rectangle")
            {
                std::string name;
                float x, y, sx, sy;
                float width, height;
                int r, g, b;

                ifs >> name >> x >> y >> sx >> sy >> r >> g >> b >> width >> height;

                RectangleSettings rs(x, y, width, height);
                rs.SetColor(r, g, b);
                rs.SetName(name);
                rs.SetSpeed(sx, sy);
                AddShape(rs);
                ifs >> type;
            }

            if (type == "Font")
            {
                ifs >> settings.mFont.file >> settings.mFont.size;
                ifs >> settings.mFont.r >> settings.mFont.g >> settings.mFont.b;
                if (!myFont.loadFromFile(settings.GetFontFile()))
                {
                    std::cerr << "Could not load font!\n";
                    exit(-1);
                }
                ifs >> type;
            }
        }
    }
    ifs.close();
}

static void sRender(sf::RenderWindow& window)
{
    window.clear();

    for (CShape& cir : sCircles)
    {
        cir.Draw(window);
        cir.Move();
    }

    for (RShape& rect : sRectangles)
    {
        rect.Draw(window);
        rect.Move();
    }

    window.display();
}

static int main2()
{
    LoadConfig(settings);

    for (int x = 0; x < 40; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            RectangleSettings rs(x * 10.0f, y * 10.0f, 15, 15);
            rs.SetColor(y * 10, x * 10, y * 20);
            RShape rect(rs, settings, myFont);
            sRectangles.push_back(std::move(rect));
        }
    }

    const int height = settings.GetWindowHeight();
    const int width = settings.GetWindowWidth();

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    window.setFramerateLimit(60);

    Vec2 v1(100, 300);
    Vec2 v2(0.2f, 200);
    Vec2 v3(10, 10);
    v1.Add(v2).Scale(5);

    float dist = v3.Dist(v1);
    float circleMoveSpeed = 0.5f;


    sf::Text text("Sample text", myFont, settings.GetFontSize());
    float widtht = text.getLocalBounds().width;
    //TODO   //text.setColor();
    text.setPosition(0, height - (float)text.getCharacterSize());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                if (event.key.code == sf::Keyboard::X)
                {
                    circleMoveSpeed *= -1.0f;
                }
            }
        }

        //sUserInput();
        //sMovement();
        UpdateCollisions();
        sRender(window);

        window.draw(text);

        m_CurrentFrame++;
    }
    return 0;
}


