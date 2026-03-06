#include "Noise.hpp"


namespace ne {

namespace detail {

constexpr float getConstantVector1D(std::uint8_t value) {
    return (value & 1) ? 1.0f : -1.0f;
}

constexpr sf::Vector2f getConstantVector2D(std::uint8_t value) {
    switch(value & 3)
    {
        case 0: return sf::Vector2f(1.0f, 1.0f);
        case 1: return sf::Vector2f(-1.0f, 1.0f);
        case 2: return sf::Vector2f(-1.0f, -1.0f);
        case 3: return sf::Vector2f(1.0f, -1.0f);
    }

    std::unreachable();
};

constexpr float fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
};

} //namespace detail

float perlinNoise(const PermutationTable& table, float coord) {
    int cellCoord = static_cast<int>(std::floor(coord)) & 255;
    float fractionCoord = coord - std::floor(coord);

    float distLeft = fractionCoord - 0.0f;
    float distRight = fractionCoord - 1.0f;

    std::uint8_t valueLeft  = table[cellCoord];
    std::uint8_t valueRight = table[cellCoord + 1];

    //The LHS and RHS of a dot product is just multiplication
    float dotLeft = distLeft * detail::getConstantVector1D(valueLeft);
    float dotRight = distRight * detail::getConstantVector1D(valueRight);

    float u = detail::fade(fractionCoord);
    return std::lerp(dotLeft, dotRight, u);
}

float perlinNoise(const PermutationTable& table, sf::Vector2f pos) {
    //Copied impl from https://rtouti.github.io/graphics/perlin-noise-algorithm
    int flooredX    = static_cast<int>(std::floor(pos.x));
    int flooredY    = static_cast<int>(std::floor(pos.y));
    int cellX       = flooredX % 255;
    int cellY       = flooredY % 255;
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
