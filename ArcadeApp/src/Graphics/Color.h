#pragma once

#include <stdint.h>

struct SDL_PixelFormat;

class Color
{
	uint32_t mColor;

	public:

	static const SDL_PixelFormat* mFormat;
	static void InitColorFormat(const SDL_PixelFormat * format);

	inline static Color Black() {return Color(0, 0, 0);}
	inline static Color White() {return Color(255, 255, 255);}
	inline static Color Red() {return Color(255, 0, 0);}
	inline static Color Green() { return Color(0, 255, 0);}
	inline static Color Blue() {return Color(0, 0, 255);}
	inline static Color Yellow() {return Color(255, 255, 0);}
	inline static Color Magenta() {return Color(255, 0, 255);}
	inline static Color Cyan() {return Color(37, 240, 217);}
	inline static Color Pink() {return Color(252, 197, 224);}
	inline static Color Orange() {return Color(245, 190, 100);}

	Color(): Color(0){}
	Color(uint32_t color): mColor(color){}
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	Color(uint8_t r, uint8_t g, uint8_t b) : Color(r, g, b, 255) {}

	inline bool operator==(const Color& c) const {return mColor == c.mColor;}
	inline bool operator!=(const Color& c) const {return !(*this == c);}
	inline uint32_t GetPixelColor() const {return mColor;}

	void SetRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void SetRed(uint8_t red);
	void SetGreen(uint8_t green);
	void SetBlue(uint8_t blue);
	void setAlpha(uint8_t alpha);

	uint8_t GetRed() const;
	uint8_t GetGreen() const;
	uint8_t GetBlue() const;
	uint8_t GetAlpha() const;

};