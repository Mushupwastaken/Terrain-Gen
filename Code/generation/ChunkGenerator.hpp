#pragma once

#include "grid/ChunkManager.hpp"

//In a real application, this function wouldnt exist
//My ideal function = creating chunk and skipping tiles that arent equal to None (Allows for structure gen)
//Called using chunkPosition

namespace ne {

void visibleChunkGenerator(sf::RenderWindow& window, ChunkManager& manager);

} //namespace ne
