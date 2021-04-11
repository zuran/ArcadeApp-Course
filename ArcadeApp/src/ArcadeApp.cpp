// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <cmath>

#include "Color.h"
#include "Screen.h"
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"

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
    Star2D star = Star2D(cX, cY, 0, radius);
    bool isGrowing = true;

    Triangle triangle = {Vec2D(60, 10), Vec2D(10, 110), Vec2D(110, 110)};
    AARectangle rect = {Vec2D(115, 115), 50, 30};
    Circle circle = {Vec2D(160, 160), 45};

    SDL_Event sdlEvent;
    bool running = true;
    SDL_PollEvent(&sdlEvent);

    while (!(sdlEvent.type == SDL_QUIT))
    {
            SDL_Delay(10);
            SDL_PollEvent(&sdlEvent);

            
            inc -= 0.01f;
            theScreen.Draw(star, Color::Cyan());
            theScreen.Draw(triangle, Color::Red(), true, Color::Red());
            theScreen.Draw(rect, Color::Blue(), true, Color::Blue());
            theScreen.Draw(circle, Color::Orange(), true, Color::Orange());

            star.SetAngle(inc);
            if (radius >= cX)
            {
                isGrowing = false;
            }
            else if (radius <= 15.f)
            {
                isGrowing = true;
            }
            if (isGrowing)
            {
                radius += 1.f;
            }
            else
            {
                radius -= 1.f;
            }
            star.SetRadius(radius);

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
