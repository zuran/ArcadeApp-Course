#include "Screen.h"
#include "Vec2D.h"
#include "Line2D.h"
#include "Star2D.h"
#include "Triangle.h"
#include "AARectangle.h"
#include "Circle.h"
#include "Utils.h"
#include <SDL.h>
#include <cassert>
#include <cmath>

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

void Screen::Draw(const Line2D& line, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        int dx, dy;
        int x0 = roundf(line.GetP0().GetX());
        int y0 = roundf(line.GetP0().GetY());
        int x1 = roundf(line.GetP1().GetX());
        int y1 = roundf(line.GetP1().GetY());

        dx = x1 - x0;
        dy = y1 - y0;

        signed const char ix((dx > 0) - (dx < 0)); // 1 or -1
        signed const char iy((dy > 0) - (dy < 0));

        dx = abs(dx) * 2;
        dy = abs(dy) * 2;

        Draw(x0, y0, color);
        if (dx >= dy)
        {
            // go along in the x
            int d = dy - dx / 2;
            while (x0 != x1)
            {
                if (d >= 0)
                {
                    d -= dx;
                    y0 += iy;
                }

                d += dy;
                x0 += ix;

                Draw(x0, y0, color);
            }
        }
        else
        {
            // go along in the y
            int d = dx - dy / 2;
            while (y0 != y1)
            {
                if (d >= 0)
                {
                    d -= dy;
                    x0 += ix;
                }

                d += dx;
                y0 += iy;

                Draw(x0, y0, color);
            }
        }
    }
}

void Screen::Draw(const Star2D& star, const Color& color)
{
    float x = star.GetOrigin().GetX();
    float y = star.GetOrigin().GetY();
    float r = star.GetRadius();
    float angle = star.GetAngle() - M_PI / 2;

    Vec2D p [5];
    for (int i = 0; i < 5; ++i, angle += M_PI / 2.5f)
    {
        p[i] = Vec2D(x + cosf(angle) * r, y + sinf(angle) * r);
    }

    Draw(Line2D(p[0], p[2]), color);
    Draw(Line2D(p[1], p[3]), color);
    Draw(Line2D(p[2], p[4]), color);
    Draw(Line2D(p[3], p[0]), color);
    Draw(Line2D(p[4], p[1]), color);
}

void Screen::Draw(const Triangle& triangle, const Color& color)
{
    auto p = triangle.GetPoints();
    Draw(Line2D(p[0], p[1]), color);
    Draw(Line2D(p[1], p[2]), color);
    Draw(Line2D(p[2], p[0]), color);
}

void Screen::Draw(const AARectangle& rect, const Color& color)
{
    auto p = rect.GetPoints();
    Draw(Line2D(p[0], p[1]), color);
    Draw(Line2D(p[1], p[2]), color);
    Draw(Line2D(p[2], p[3]), color);
    Draw(Line2D(p[3], p[0]), color);
}

void Screen::Draw(const Circle& circle, const Color& color)
{
    static unsigned int NUM_CIRCLE_SEGMENTS = 30;

    float angle = TWO_PI / float(NUM_CIRCLE_SEGMENTS);

    Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
    Vec2D p1 = p0;
    Line2D nextLineToDraw;

    for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
    {
        p1.Rotate(angle, circle.GetCenterPoint());
        nextLineToDraw.SetP1(p1);
        nextLineToDraw.SetP0(p0);

        Draw(nextLineToDraw, color);
        p0 = p1;
    }
}

Screen& Screen::operator=(const Screen& screen)
{
    return *this;
}

void Screen::ClearScreen()
{
    assert(moptrWindow);
    if(moptrWindow)
    {
        SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
    }
}
