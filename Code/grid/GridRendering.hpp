#pragma once

#include "ChunkManager.hpp"


namespace ne {

void updateChunkMesh(Chunk& chunk);

void drawVisibleChunks(sf::RenderWindow& window, ChunkManager& manager);

} //namespace ne