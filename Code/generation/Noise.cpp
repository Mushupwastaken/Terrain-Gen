#include "Noise.hpp"
#include "core/IntMath.hpp"

namespace ne {

namespace detail {

[[nodiscard]] constexpr float getConstantVector1D(std::uint8_t value) {
    return (value & 1) ? 1.0f : -1.0f;
}

[[nodiscard]] constexpr sf::Vector2f getConstantVector2D(std::uint8_t value) {
    switch(value & 3)
    {
        case 0: return sf::Vector2f(1.0f, 1.0f);
        case 1: return sf::Vector2f(-1.0f, 1.0f);
        case 2: return sf::Vector2f(-1.0f, -1.0f);
        case 3: return sf::Vector2f(1.0f, -1.0f);
    }

    std::unreachable();
};

[[nodiscard]] constexpr float fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
};

} //namespace detail

float perlinNoise(const PermutationTable& table, float x) {
    int flooredX = core::intFloor(x);
    int cellX = flooredX & 255;
    float fractionX = x - flooredX;

    float distLeft = fractionX - 0.0f;
    float distRight = fractionX - 1.0f;

    std::uint8_t valueLeft  = table[cellX];
    std::uint8_t valueRight = table[cellX + 1];

    //The LHS and RHS of a dot product is just multiplication
    float dotLeft = distLeft * detail::getConstantVector1D(valueLeft);
    float dotRight = distRight * detail::getConstantVector1D(valueRight);

    float u = detail::fade(fractionX);
    return std::lerp(dotLeft, dotRight, u);
}

float perlinNoise(const PermutationTable& table, sf::Vector2f pos) {
    //Copied impl from https://rtouti.github.io/graphics/perlin-noise-algorithm
    int flooredX    = core::intFloor(pos.x);
    int flooredY    = core::intFloor(pos.y);
    int cellX       = flooredX & 255;
    int cellY       = flooredY & 255;
    float fractionX = pos.x - flooredX;
    float fractionY = pos.y - flooredY;

    sf::Vector2f topRight(fractionX - 1.0f, fractionY - 1.0f);
	sf::Vector2f topLeft(fractionX - 0.0f, fractionY - 1.0f);
	sf::Vector2f bottomRight(fractionX - 1.0f, fractionY - 0.0f);
	sf::Vector2f bottomLeft(fractionX - 0.0f, fractionY - 0.0f);

    std::uint8_t valueTopRight      = table[table[cellX + 1] + cellY + 1];
    std::uint8_t valueTopLeft       = table[table[cellX + 0] + cellY + 1];
    std::uint8_t valueBottomRight   = table[table[cellX + 1] + cellY + 0];
    std::uint8_t valueBottomLeft    = table[table[cellX + 0] + cellY + 0];

    float dotTopRight = topRight.dot(detail::getConstantVector2D(valueTopRight));
	float dotTopLeft = topLeft.dot(detail::getConstantVector2D(valueTopLeft));
	float dotBottomRight = bottomRight.dot(detail::getConstantVector2D(valueBottomRight));
	float dotBottomLeft = bottomLeft.dot(detail::getConstantVector2D(valueBottomLeft));

    float u = detail::fade(fractionX);
	float v = detail::fade(fractionY);

    float lower = std::lerp(dotBottomLeft, dotBottomRight, u);
    float upper = std::lerp(dotTopLeft, dotTopRight, u);
    
    return std::lerp(lower, upper, v);
}

} //namespace ne
