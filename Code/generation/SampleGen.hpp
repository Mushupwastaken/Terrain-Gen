#pragma once

#include "PermutationTable.hpp"
#include "Noise.hpp"
#include <random>


#include "grid/Assets.hpp"
#include "grid/ChunkConfig.hpp"
#include "grid/Chunk.hpp"
#include "grid/GridPosition.hpp"
#include "grid/GridArea.hpp"

#include "grid/ChunkManager.hpp"

//This is lazily programmed, used for showcasing the engines power, and ease of use.
//Additional dependencies, like Noise are used similarly.

namespace ne {

Chunk generateChunk(GridPosition pos);

void debugGenerator(sf::RenderWindow& window, ChunkManager& manager);

} //namespace ne
