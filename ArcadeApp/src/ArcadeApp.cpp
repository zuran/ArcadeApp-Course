// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "App.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;
const int MAGNIFICATION = 3;

using namespace std;

//vector<Star2D> GenerateRandomStars() {
//    std::random_device r;
//    std::default_random_engine e1(r());
//    std::uniform_int_distribution<int> uniformX(0, SCREEN_WIDTH);
//    std::uniform_int_distribution<int> uniformY(0, SCREEN_HEIGHT);
//
//    vector<Star2D> stars;
//    for (size_t i = 0; i < 10; ++i)
//    {
//        float randomX = static_cast<float>(uniformX(e1));
//        float randomY = static_cast<float>(uniformY(e1));
//
//        stars.push_back(Star2D::GetRandomStar(Vec2D(randomX, randomY), 5.f, 30.f));
//    }
//    return stars;
//}
//
//void SortStars(vector<Star2D>& stars) {
//    std::sort(stars.begin(), stars.end(), [](Star2D& s1, Star2D& s2) {
//            return s1.GetRadius() < s2.GetRadius();
//    });
//
//    stars[0].SetOrigin(Vec2D(stars[0].GetRadius(), stars[0].GetRadius()));
//    for (size_t i = 1; i < 10; ++i)
//    {
//        stars[i].SetOrigin(Vec2D(stars[i].GetRadius(), 
//            stars[i-1].GetOrigin().GetY() + 
//            stars[i-1].GetRadius() + 
//            stars[i].GetRadius()));
//    }
//
//}
//
//void DrawStars(vector<Star2D>& stars) {
//    std::random_device r;
//    std::default_random_engine e1(r());
//    std::uniform_real_distribution<float> uniformSpeed(-.03f, .03f);
//
//    vector<float> speeds;
//    for (size_t i = 0; i < 10; ++i)
//    {
//        float randomSpeed = uniformSpeed(e1);
//        speeds.push_back(randomSpeed);
//    }
//
//    // Draw
//    Screen theScreen;
//    theScreen.Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION);
//
//    SDL_Event sdlEvent;
//    bool running = true;
//    SDL_PollEvent(&sdlEvent);
//
//    while (!(sdlEvent.type == SDL_QUIT))
//    {
//        SDL_Delay(10);
//        SDL_PollEvent(&sdlEvent);
//
//        for (size_t i = 0; i < 10; ++i)
//        {
//            theScreen.Draw(stars[i], Color::Red());
//            stars[i].SetAngle(stars[i].GetAngle() + speeds[i]);
//        }
//
//        theScreen.SwapScreens();
//    }
//}

int main(int argc, char *args[])
{
    /*auto stars = GenerateRandomStars();
    SortStars(stars);
    DrawStars(stars);*/

    if(App::Singleton().Init(SCREEN_WIDTH, SCREEN_HEIGHT, MAGNIFICATION))
    {
        App::Singleton().Run();
    }

    return 0;
}