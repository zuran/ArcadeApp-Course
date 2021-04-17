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
#include <algorithm>
#include "BmpImage.h"
#include "SpriteSheet.h"
#include "BitmapFont.h"

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
        //SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;
        SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
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
        mBackBuffer.SetPixel(color, int(point.GetX()), int(point.GetY()));
    }
}

void Screen::Draw(const Line2D& line, const Color& color)
{
    assert(moptrWindow);
    if (moptrWindow)
    {
        int dx, dy;
        int x0 = int(roundf(line.GetP0().GetX()));
        int y0 = int(roundf(line.GetP0().GetY()));
        int x1 = int(roundf(line.GetP1().GetX()));
        int y1 = int(roundf(line.GetP1().GetY()));

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
    float angle = star.GetAngle() - float(M_PI) / 2;

    Vec2D p [5];
    for (int i = 0; i < 5; ++i, angle += float(M_PI) / 2.5f)
    {
        p[i] = Vec2D(x + cosf(angle) * r, y + sinf(angle) * r);
    }

    Draw(Line2D(p[0], p[2]), color);
    Draw(Line2D(p[1], p[3]), color);
    Draw(Line2D(p[2], p[4]), color);
    Draw(Line2D(p[3], p[0]), color);
    Draw(Line2D(p[4], p[1]), color);
}

void Screen::Draw(const Triangle& triangle, const Color& color, bool fill, const Color& fillColor)
{
    auto p = triangle.GetPoints();
    
    if (fill)
    {
        FillPoly(p, [fillColor](uint32_t x, uint32_t y) { return fillColor; });
    }

    Draw(Line2D(p[0], p[1]), color);
    Draw(Line2D(p[1], p[2]), color);
    Draw(Line2D(p[2], p[0]), color);
}

void Screen::Draw(const AARectangle& rect, const Color& color, bool fill, const Color& fillColor)
{
    auto p = rect.GetPoints();

    if (fill)
    {
        FillPoly(p, [fillColor](uint32_t x, uint32_t y) { return fillColor; });
    }

    Draw(Line2D(p[0], p[1]), color);
    Draw(Line2D(p[1], p[2]), color);
    Draw(Line2D(p[2], p[3]), color);
    Draw(Line2D(p[3], p[0]), color);
}

void Screen::Draw(const Circle& circle, const Color& color, bool fill, const Color& fillColor)
{
    static unsigned int NUM_CIRCLE_SEGMENTS = 30;

    std::vector<Vec2D> circlePoints;
    std::vector<Line2D> lines;

    float angle = TWO_PI / float(NUM_CIRCLE_SEGMENTS);

    Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
    Vec2D p1 = p0;
    Line2D nextLineToDraw;

    for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
    {
        p1.Rotate(angle, circle.GetCenterPoint());
        nextLineToDraw.SetP1(p1);
        nextLineToDraw.SetP0(p0);

        //Draw(nextLineToDraw, color);
        lines.push_back(nextLineToDraw);
        p0 = p1;
        circlePoints.push_back(p0);
    }

    if (fill)
    {
        FillPoly(circlePoints, [fillColor](uint32_t x, uint32_t y) {
            return fillColor;
        });
    }

    for (const Line2D& line : lines)
    {
        Draw(line, color);
    }
}

void Screen::Draw(const SpriteSheet& ss, const std::string& spriteName, const Vec2D& pos, const Color& overlayColor)
{
    Draw(ss.GetBmpImage(), ss.GetSprite(spriteName), pos, overlayColor);
}

void Screen::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& pos, const Color& overlayColor)
{
    uint32_t x = pos.GetX();
    
    const SpriteSheet& ss = font.GetSpriteSheet();
    for (char c : textLine)
    {
        if (c == ' ')
        {
            x += font.GetFontSpacingBetweenWords();
            continue;
        }

        Sprite sprite = ss.GetSprite(std::string("") + c);
        Draw(ss.GetBmpImage(), sprite, Vec2D(x, pos.GetY()), overlayColor);
        x += sprite.width;
        x += font.GetFontSpacingBetweenLetters();
    }
}

void Screen::Draw(const BmpImage& image, const Sprite& sprite, const Vec2D& pos, const Color& overlayColor)
{
    float rVal = static_cast<float>(overlayColor.GetRed()) / 255.0f;
    float gVal = static_cast<float>(overlayColor.GetGreen()) / 255.0f;
    float bVal = static_cast<float>(overlayColor.GetBlue()) / 255.0f;
    float aVal = static_cast<float>(overlayColor.GetAlpha()) / 255.0f;

    uint32_t width = sprite.width;
    uint32_t height = sprite.height;

    const std::vector<Color>& pixels = image.GetPixels();
    auto topLeft = pos;
    auto topRight = pos + Vec2D(width, 0);
    auto bottomLeft = pos + Vec2D(0, height);
    auto bottomRight = pos + Vec2D(width, height);

    std::vector<Vec2D> points = {topLeft, bottomLeft, bottomRight, topRight};
    Vec2D xAxis = topRight - topLeft;
    Vec2D yAxis = bottomLeft - topLeft;

    const float invXAxisLengthSq = 1.0f / xAxis.Mag2();
    const float invYAxisLengthSq = 1.0f / yAxis.Mag2();

    FillPoly(points, [&](uint32_t px, uint32_t py) {
        Vec2D p = {static_cast<float>(px), static_cast<float>(py)};
        Vec2D d = p - topLeft;

        float u = invXAxisLengthSq * d.Dot(xAxis);
        float v = invYAxisLengthSq * d.Dot(yAxis);

        u = Clamp(u, 0.0f, 1.0f);
        v = Clamp(v, 0.0f, 1.0f);

        float tx = roundf(u * static_cast<float>(sprite.width));
        float ty = roundf(v * static_cast<float>(sprite.height));

        Color imageColor = pixels[GetIndex(image.GetWidth(), ty + sprite.yPos, tx + sprite.xPos)];
        Color newColor = {
            static_cast<uint8_t>(imageColor.GetRed() * rVal),
            static_cast<uint8_t>(imageColor.GetGreen() * gVal),
            static_cast<uint8_t>(imageColor.GetBlue() * bVal),
            static_cast<uint8_t>(imageColor.GetAlpha() * aVal) };

        return newColor;
    });
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

void Screen::FillPoly(const std::vector<Vec2D>& points, FillPolyFunc func)
{
    if (points.size() > 0)
    {
        float top = points[0].GetY();
        float bottom = points[0].GetY();
        float right = points[0].GetX();
        float left = points[0].GetX();

        for (size_t i = 1; i < points.size(); ++i)
        {
            if (points[i].GetY() < top)
            {
               top = points[i].GetY();
            }
            if (points[i].GetY() > bottom)
            {
                bottom = points[i].GetY();
            }
            if (points[i].GetX() < left)
            {
                left = points[i].GetX();
            }
            if (points[i].GetX() > right)
            {
                right = points[i].GetX();
            }
        }

        for (int pixelY = int(top); pixelY < bottom; ++pixelY)
        {
            std::vector<float> nodeXVec;
            size_t j = points.size() - 1;

            for (size_t i = 0; i < points.size(); ++i)
            {
                float pointiY = points[i].GetY();
                float pointjY = points[j].GetY();

                if ((pointiY <= (float)pixelY && pointjY > (float)pixelY ||
                    (pointjY <= (float)pixelY && pointiY > (float)pixelY)))
                {
                    float denom = pointjY - pointiY;
                    if (IsEqual(denom, 0))
                    {
                        continue;
                    }

                    float x = points[i].GetX() + (pixelY - pointiY) / (denom) * 
                        (points[j].GetX() - points[i].GetX());

                    nodeXVec.push_back(x);
                }
                j = i;
            }
            std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());

            for (size_t k = 0; k < nodeXVec.size(); k += 2)
            {
                if (nodeXVec[k] > right)
                {
                    break;
                }

                if (nodeXVec[k + 1] > left)
                {
                    if (nodeXVec[k] < left)
                    {
                        nodeXVec[k] = left;
                    }
                    if (nodeXVec[k + 1] > right)
                    {
                        nodeXVec[k+1] = right;
                    }
                    
                    //Line2D line = {Vec2D(nodeXVec[k], pixelY), Vec2D(nodeXVec[k+1], pixelY)};
                    //Draw(line, color);

                    for (int pixelX = int(nodeXVec[k]); pixelX < nodeXVec[k + 1]; ++pixelX)
                    {
                        Draw(pixelX, pixelY, func(pixelX, pixelY));
                    }
                }
            }
        }

        
    }
}
