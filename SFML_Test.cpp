// SFML_Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "engine/spec1.h"
#include "engine/game.h"
#include "engine/gameengine.h"

int main()
{
  // Game g("assets/config_1.txt");
    //g.Run();


    GameEngine game("assets/assets.txt");
    game.Run();
    //main2();
}