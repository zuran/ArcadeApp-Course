// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>

#include "Color.h";
#include "Screen.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

using namespace std;

int main(int argc, char *args[])
{
    Screen theScreen;
    theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);
    theScreen.Draw(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, Color::Yellow());
    theScreen.SwapScreens();

    SDL_Event sdlEvent;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&sdlEvent))
        {
            switch (sdlEvent.type)
            {
                case SDL_QUIT:
                running = false;
                break;
            }
        }
    }

    return 0;
}


size_t GetIndex(SDL_Surface* noptrWindowSurface, int r, int c)
{
    return r * noptrWindowSurface->w + c;
}
