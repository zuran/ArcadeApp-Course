#include "Vec2D.h"

std::ostream& operator<<(std::ostream& consoleOut, const Vec2D& vec) {
    std::cout << "X: " << vec.mX << ", Y: " << vec.mY << std::endl;
    return consoleOut;
}