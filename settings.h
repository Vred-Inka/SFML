#pragma once

#include <SFML/Graphics.hpp>

struct WindowsSettings
{
private:
    std::string name = "Default";
    int height = 0;
    int width = 0;

public:
    int GetWidth() const { return width; };
    int GetHeight() const { return height; };

    void SetWidth(int w) { width = w; };
    void SetHeight(int h) { height = h; };
};

class ShapeSettings
{
private:
    float x = 0.0f;
    float y = 0.0f;
    float sx = 0.0f;
    float sy = 0.0f;
    sf::Color color = sf::Color(0, 0, 0, 255);
    std::string name;

public:
    float GetX() const { return x; };
    float GetY() const { return y; };
    void SetX(float x) { this->x = x; };
    void SetY(float y) { this->y = y; };

    float GetSpeedX() const { return sx; };
    float GetSpeedY() const { return sy; };
    void SetSpeedX(float speed) { sx = speed; };
    void SetSpeedY(float speed) { sy = speed; };
    void SetSpeed(float x, float y) { sx = x, sy = y; };

    sf::Color GetColor() const { return color; };
    void SetColor(int r, int g, int b);

    std::string GetName() const { return name; };
    void SetName(std::string& name) { this->name = std::move(name); };
};

class CircleSettings : public ShapeSettings
{
private:
    float r = 0.0f;

public:
    CircleSettings(): r(0.0f) {};
    CircleSettings(float x, float y, float r); 

    float GetRadius() const { return r; };
};

class RectangleSettings : public ShapeSettings
{
private:
    float m_Height = 0.0f;
    float m_Width = 0.0f;

public:
    RectangleSettings() {};
    RectangleSettings(float x, float y, float w, float h);

    float GetWidth() const { return m_Width; };
    float GetHeight() const { return m_Height; };
};

class FontSettings
{
public:
    std::string file = "";
    int size{ 0 };
    int r{ 0 };
    int g{ 0 };
    int b{ 0 };

    sf::Color GetColor() { return sf::Color(r, g, b); }
};

class MainSettings
{
public:
    WindowsSettings mWindow;
    FontSettings mFont;
    //ShapeSettings m_ShapeSettings;

public:
    int GetWindowHeight() const {return mWindow.GetHeight(); };
    int GetWindowWidth() const { return mWindow.GetWidth(); };

    std::string GetFontFile() const { return mFont.file; };
    int GetFontSize() const { return mFont.size; };   

};
