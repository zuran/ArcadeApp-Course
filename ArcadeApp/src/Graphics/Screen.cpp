#include "Screen.h"
#include "Vec2D.h"
#include <SDL.h>
#include <cassert>

Screen::Screen(): mWidth(0), mHeight(0), mnoptrWindowSurface(nullptr), moptrWindow(nullptr)
{
}

Screen::~Screen()
{
    if (moptrWindow)
    {
        SDL_DestroyWindow(moptrWindow);
        moptrWindow = nullptr;
    }

    SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t w, uint32_t h, uint32_t mag)
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_INIT Failed" << std::endl;
        return nullptr;
    }

    mWidth = w;
    mHeight = h;

    moptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth * mag, mHeight * mag, 0);

    if (moptrWindow)
    {
        mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow);
        SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;
        Color::InitColorFormat(pixelFormat);
        mClearColor = Color::Black();
        mBackBuffer.Init(pixelFormat->format, mWidth, mHeight);
        mBackBuffer.Clear(mClearColor);
    }

    return moptrWindow;
}

void Screen::SwapScreens()
{
    assert(moptrWindow);
    if(moptrWindow)
    {
        ClearScreen();
        SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);
        SDL_UpdateWindowSurface(moptrWindow);

        mBackBuffer.Clear(mClearColor);
    }
}

void Screen::Draw(int x, int y, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        mBackBuffer.SetPixel(color, x, y);
    }
}

void Screen::Draw(const Vec2D& point, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        mBackBuffer.SetPixel(color, point.GetX(), point.GetY());
    }
}

void Screen::ClearScreen()
{
    assert(moptrWindow);
    if(moptrWindow)
    {
        SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
    }
}
