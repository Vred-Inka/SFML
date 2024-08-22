// SFML_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;
#include "settings.h"
#include "CShape.h"
#include "RShape.h"

std::vector<CShape> sCircles;
std::vector<RShape> sRectangles;

sf::Font myFont;
MainSettings settings;

void AddShape(RectangleSettings& rs)
{
    RShape rect(rs, settings, myFont);
    sRectangles.push_back(std::move(rect));
}

void AddShape(CircleSettings& cs)
{
    CShape circle(cs, settings, myFont);
    sCircles.push_back(std::move(circle));
}

void LoadConfig(MainSettings& settings)
{
    char filename[] = "config.txt";
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
                
                CircleSettings cs(x,y,rad);
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
                int width, height;
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

int main()
{
    LoadConfig(settings);

    for (int x = 0; x < 40; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            RectangleSettings rs(x * 10.0f, y * 10.0f, 15.0f, 15.0f);
            rs.SetColor(y * 10, x * 10, y * 20);
            RShape rect(rs, settings, myFont);
            sRectangles.push_back(std::move(rect));
        }
    }
      
    const int height = settings.GetWindowHeight();
    const int width = settings.GetWindowWidth();

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    window.setFramerateLimit(60);

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
                    circleMoveSpeed  *= -1.0f;
                }
            }
        }

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

        window.draw(text);
        window.display();
    }

    

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
