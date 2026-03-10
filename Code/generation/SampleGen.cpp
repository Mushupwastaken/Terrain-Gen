#include "SampleGen.hpp"
#include "grid/Config.hpp"

#include <random>


namespace ne {

std::mt19937 gen{__LINE__};
PermutationTable table(gen);

Chunk generateChunk(GridPosition pos) {
    const sf::Vector2i currentTilePos = pos.asTile();

    std::array<int, constants::chunkSize> heights{};
    int minHeight = std::numeric_limits<int>::max();
    int maxHeight = std::numeric_limits<int>::min();

    for(int x=0; x<constants::chunkSize; x++)
    {
        const float noiseValue = perlinNoise(table, (currentTilePos.x + x) * 0.10f);
        heights[x] = static_cast<int>(noiseValue * 5.0f);
        
        if(heights[x] < minHeight)
        {
            minHeight = heights[x];
        }
        else if(heights[x] > maxHeight)
        {
            maxHeight = heights[x];
        }
    }
    
    Chunk chunk;
    chunk.tiles.fill({Tiles::Air, 0});

    for(int y = 0; y < constants::chunkSize; y++)
    {
        for(int x = 0; x < constants::chunkSize; x++)
        {
            sf::Vector2i localTilePos = {x, y};
            sf::Vector2i globalTilePos = currentTilePos + localTilePos;
            std::size_t index = Chunk::toIndex(localTilePos);
            
            if(globalTilePos.y > maxHeight + heights[x] + 6)
            {
                //Stone with caves
                float noiseA = perlinNoise(table, sf::Vector2f(globalTilePos) * 0.08f);
                float noiseB = perlinNoise(table, sf::Vector2f(globalTilePos) * 0.11f);

                if(noiseA + noiseB < 0.125f)
                {
                    chunk.tiles[index].value = Tiles::Stone;
                }
            }
            else if(globalTilePos.y > maxHeight + heights[x] + 3)
            {
                //Stone
                chunk.tiles[index].value = Tiles::Stone;
            }
            else if(globalTilePos.y > heights[x])
            {
                //Grass
                chunk.tiles[index].value = Tiles::Dirt;
            }
        }
    }

    return chunk;
}

} //namespace ne
