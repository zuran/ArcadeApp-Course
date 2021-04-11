// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include <cmath>
#include <random>
#include <algorithm>

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

vector<Star2D> GenerateRandomStars() {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniformX(0, SCREEN_WIDTH);
    std::uniform_int_distribution<int> uniformY(0, SCREEN_HEIGHT);

    vector<Star2D> stars;
    for (size_t i = 0; i < 10; ++i)
    {
        float randomX = static_cast<float>(uniformX(e1));
        float randomY = static_cast<float>(uniformY(e1));

        stars.push_back(Star2D::GetRandomStar(Vec2D(randomX, randomY), 5.f, 30.f));
    }
    return stars;
}

void SortStars(vector<Star2D>& stars) {
    std::sort(stars.begin(), stars.end(), [](Star2D& s1, Star2D& s2) {
            return s1.GetRadius() < s2.GetRadius();
    });

    stars[0].SetOrigin(Vec2D(stars[0].GetRadius(), stars[0].GetRadius()));
    for (size_t i = 1; i < 10; ++i)
    {
        stars[i].SetOrigin(Vec2D(stars[i].GetRadius(), 
            stars[i-1].GetOrigin().GetY() + 
            stars[i-1].GetRadius() + 
            stars[i].GetRadius()));
    }

}

void DrawStars(vector<Star2D>& stars) {
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_real_distribution<float> uniformSpeed(-.03f, .03f);

    vector<float> speeds;
    for (size_t i = 0; i < 10; ++i)
    {
        float randomSpeed = uniformSpeed(e1);
        speeds.push_back(randomSpeed);
    }

    // Draw
    Screen theScreen;
    theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);

    SDL_Event sdlEvent;
    bool running = true;
    SDL_PollEvent(&sdlEvent);

    while (!(sdlEvent.type == SDL_QUIT))
    {
        SDL_Delay(10);
        SDL_PollEvent(&sdlEvent);

        for (size_t i = 0; i < 10; ++i)
        {
            theScreen.Draw(stars[i], Color::Red());
            stars[i].SetAngle(stars[i].GetAngle() + speeds[i]);
        }

        theScreen.SwapScreens();
    }
}

int main(int argc, char *args[])
{
    auto stars = GenerateRandomStars();
    SortStars(stars);
    DrawStars(stars);

    /*Screen theScreen;
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
    triangle.MoveTo(Vec2D(cX, cY));
    rect.MoveTo(Vec2D(cX, cY));
    circle.MoveTo(Vec2D(cX, cY));

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
            theScreen.Draw(circle, Color(0, 255, 0, 150), true, Color(0, 255, 0, 150));

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
    */
    return 0;
}


size_t GetIndex(SDL_Surface* noptrWindowSurface, int r, int c)
{
    return r * noptrWindowSurface->w + c;
}
