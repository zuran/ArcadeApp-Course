// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <cmath>

#include "Color.h";
#include "Screen.h"
#include "Line2D.h";

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

using namespace std;

int main(int argc, char *args[])
{
    Screen theScreen;
    theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);
    
    float radius = 15.f; //SCREEN_WIDTH / 2;
    float inc = .0f;
    float cX = SCREEN_WIDTH / 2;
    float cY = SCREEN_HEIGHT / 2;
    
    
    Line2D line;
    Line2D line2;

    for(;inc > -M_PI * 2; inc -= .01f) {

    }
    //Line2D line = {Vec2D(0, 0), Vec2D(SCREEN_WIDTH, SCREEN_HEIGHT)};
    
    //theScreen.Draw(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, Color::Yellow());
    

    SDL_Event sdlEvent;
    bool running = true;
    SDL_PollEvent(&sdlEvent);

    while (!(sdlEvent.type == SDL_QUIT))
    {
            SDL_Delay(10);
            SDL_PollEvent(&sdlEvent);

            inc -= 0.02f;
            float oX = cosf(inc + M_PI / 2.5f) * radius;
            float oY = sinf(inc + M_PI / 2.5f) * radius;
            Vec2D p1 = {oX + cX, oY + cY};

            float oX2 = cosf(inc + 2 * M_PI / 2.5f) * radius;
            float oY2 = sinf(inc + 2 * M_PI / 2.5f) * radius;
            Vec2D p2 = { oX2 + cX, oY2 +cY};

            float oX3 = cosf(inc + 3 * M_PI / 2.5f) * radius;
            float oY3 = sinf(inc + 3 * M_PI / 2.5f) * radius;
            Vec2D p3 = { oX3 + cX, oY3 + cY};

            float oX4 = cosf(inc + 4 * M_PI / 2.5f) * radius;
            float oY4 = sinf(inc + 4 * M_PI / 2.5f) * radius;
            Vec2D p4 = { oX4 + cX, oY4 +cY};

            float oX5 = cosf(inc + 5 * M_PI / 2.5f) * radius;
            float oY5 = sinf(inc + 5 * M_PI / 2.5f) * radius;
            Vec2D p5 = { oX5 + cX, oY5 + cY};

            theScreen.Draw(Line2D(p1, p3), Color::Blue());
            theScreen.Draw(Line2D(p2, p4), Color::Blue());
            theScreen.Draw(Line2D(p3, p5), Color::Blue());
            theScreen.Draw(Line2D(p4, p1), Color::Blue());
            theScreen.Draw(Line2D(p5, p2), Color::Blue());

            //line = { Vec2D(cX - oX, cY - oY), Vec2D(cX + oX, cY + oY) };
            //theScreen.Draw(line, Color::Yellow());
            //line2 = { Vec2D(cX - oX2, cY - oY2), Vec2D(cX + oX2, cY + oY2) };
            //theScreen.Draw(line2, Color::Yellow());


            theScreen.SwapScreens();

            //switch (sdlEvent.type)
            //{
            //    case SDL_QUIT:
            //    running = false;
            //    break;
            //}
    }

    return 0;
}


size_t GetIndex(SDL_Surface* noptrWindowSurface, int r, int c)
{
    return r * noptrWindowSurface->w + c;
}
