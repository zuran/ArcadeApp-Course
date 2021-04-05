// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>

#include "Color.h";
#include "ScreenBuffer.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;

using namespace std;

int main(int argc, char *args[])
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_INIT Failed" << std::endl;
        return 1;
    }

    SDL_Window* optrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (optrWindow == nullptr)
    {
        std::cerr << "SDL_CreateWindow Failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Surface* noptrWindowSurface = SDL_GetWindowSurface(optrWindow);
    
    SDL_PixelFormat * pixelFormat = noptrWindowSurface->format;
    
    Color::InitColorFormat(pixelFormat);

    ScreenBuffer screenBuffer;
    screenBuffer.Init(pixelFormat->format, noptrWindowSurface->w, noptrWindowSurface->h);
    
    screenBuffer.Clear(Color::Blue());
    screenBuffer.SetPixel(Color::Red(), SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

    SDL_BlitSurface(screenBuffer.GetSurface(), nullptr, noptrWindowSurface, nullptr);

    SDL_UpdateWindowSurface(optrWindow);

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

    SDL_DestroyWindow(optrWindow);
    SDL_Quit();

    return 0;
}


size_t GetIndex(SDL_Surface* noptrWindowSurface, int r, int c)
{
    return r * noptrWindowSurface->w + c;
}
