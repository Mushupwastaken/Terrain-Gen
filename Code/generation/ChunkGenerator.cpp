#include "ChunkGenerator.hpp"
#include "SampleGen.hpp"


namespace ne {

void visibleChunkGenerator(sf::RenderWindow& window, ChunkManager& manager) {
    const sf::Vector2i windowSize{window.getSize()};
    
    const sf::Vector2f topLeft     = window.mapPixelToCoords({0, 0});
    const sf::Vector2f bottomRight = window.mapPixelToCoords(windowSize);
    
    const sf::FloatRect windowRect{topLeft, bottomRight - topLeft};
    const GridArea chunkArea = GridArea::fromWorld(windowRect);

    manager.queryChunks(chunkArea, [&](Chunk* chunk, const GridPosition position) {
        if(chunk != nullptr)
        {
            return;
        }

        manager.setChunk(generateChunk(position), position);
    });
}

} //namespace ne