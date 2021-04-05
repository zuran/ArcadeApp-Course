// ArcadeApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Vec2D.h"

using namespace std;

int main()
{
    Vec2D aVec(0,10);
    Vec2D bVec(5, -1);

    cout << bVec.Reflect(aVec);

    return 0;
}