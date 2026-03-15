#include "SampleGen.hpp"
#include "grid/Config.hpp"

#include <random>   


namespace ne {

namespace detail {

std::mt19937 gen{305050};
PermutationTable pTable(gen);

} //namespace detail

Chunk generateChunk(GridPosition pos) {
    const sf::Vector2i globalTilePos = pos.asTile();

    //This is stupid -> heightmap calculation
    std::array<int, constants::chunkSize> heightMap{};
    int minHeight = std::numeric_limits<int>::max();
    int maxHeight = std::numeric_limits<int>::min();

    for(int x = 0; x < constants::chunkSize; x++)
    {
        const float noiseValue = perlinNoise(detail::pTable, (globalTilePos.x + x) * 0.10f);
        heightMap[x] = static_cast<int>(noiseValue * 5.0f);
        
        if(heightMap[x] < minHeight)
        {
            minHeight = heightMap[x];
        }
        else if(heightMap[x] > maxHeight)
        {
            maxHeight = heightMap[x];
        }
    }
    
    //Actual chunk generation
    Chunk chunk;
    chunk.tiles.fill({Tiles::Air});

    for(auto [index, tile] : chunk.tiles | std::views::enumerate)
    {
        sf::Vector2i localPos = Chunk::toPosition(index);
        sf::Vector2i worldPos = globalTilePos + localPos;

        if(worldPos.y > maxHeight + heightMap[localPos.x] + 6)
        {
            //Stone with caves
            sf::Vector2f worldPosf{worldPos};
            const float noiseA = perlinNoise(detail::pTable, worldPosf * 0.08f);
            const float noiseB = perlinNoise(detail::pTable, worldPosf * 0.11f);
            
            if(noiseA + noiseB < 0.125f)
            {
                tile.value = Tiles::Stone;
            }
        }
        else if(worldPos.y > maxHeight + heightMap[localPos.x] + 3)
        {
            //Stone
            tile.value = Tiles::Stone;
        }
        else if(worldPos.y > heightMap[localPos.x])
        {
            //Grass
            tile.value = Tiles::Dirt;
        }
    }

    return chunk;
}

} //namespace ne
