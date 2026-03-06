#pragma once

#include "PermutationTable.hpp"


namespace ne {

[[nodiscard]] float perlinNoise(const PermutationTable& table, float coord);

[[nodiscard]] float perlinNoise(const PermutationTable& table, sf::Vector2f pos);

} //namespace ne
