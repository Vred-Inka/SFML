// SFML_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "spec1.cpp"

#include <chrono>
#include <ctime>
#include "Entity.h"

void sMovement(std::vector<Entity>& entities)
{
    for (Entity& e : entities)
    {
        if (e.cTransform)
        {
            e.cTransform->m_Pos += e.cTransform->m_Speed;
        }
    }
}

void sEnemySpawner()
{
    int min = 55;
    int max = 85100;
    int diff = 1 + max - min;
    srand((unsigned)time(0));
    //float r = min + rand() % diff;
}

void doStuff(std::vector<Entity>& entities)
{
    for (Entity& e : entities)
    {
        e.cTransform->m_Pos += e.cTransform->m_Speed;
        //e.cShape->shape.SetPosition(e.cTransform->pos);
        //window.draw(e.cShape->shape);
    }
}

#include "imgui.h"
#include <SFML/Graphics.hpp>
#include "game.h"

int main()
{
    Game g("config.txt");
    g.Run();
    //main2();
}