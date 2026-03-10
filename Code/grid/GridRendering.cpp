#include "GridRendering.hpp"
#include "Config.hpp"


namespace ne {

namespace detail {

inline std::array<sf::Vertex, constants::chunkExtent * 6> vertices;

//AI ass function (debug, leave it be)
sf::Color getAssetColor(std::uint16_t value) {
    using namespace Tiles;
    
    switch(value) 
    {
        case Air:
            return sf::Color(0x00BFFFFF);
        case Grass:
            return sf::Color(0x00FF08FF);
        case Dirt:
            return sf::Color(0xA52A2AFF);
        case Stone:
            return sf::Color(0x808080FF); 
    }

    return sf::Color::White;
}

} //namespace detail

void updateChunkMesh(Chunk& chunk) {
    std::size_t vertexCount{0};

    for(auto [index, tile] : chunk.tiles | std::views::enumerate)
    {
        if(tile.value == Tiles::None)
        {
            continue;
        }

        const auto [x, y] = Chunk::toPosition(index);
        const sf::Color color = detail::getAssetColor(tile.value);

        //Apply triangles to array
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 0) * constants::renderScale, color);
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 0) * constants::renderScale, color);
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 1) * constants::renderScale, color);
        
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 0) * constants::renderScale, color);
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 1) * constants::renderScale, color);
        detail::vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 1) * constants::renderScale, color);
    }

    if(!chunk.mesh)
    {
        chunk.mesh = std::make_unique<sf::VertexBuffer>(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Static);
        std::ignore = chunk.mesh->create(vertexCount);
    }

    std::ignore = chunk.mesh->update(detail::vertices.data(), vertexCount, 0);
}

void drawVisibleChunks(sf::RenderWindow& window, ChunkManager& manager) {
    const sf::Vector2i windowSize{window.getSize()};
    
    const sf::Vector2f topLeft     = window.mapPixelToCoords({0, 0});
    const sf::Vector2f bottomRight = window.mapPixelToCoords(windowSize);
    
    const sf::FloatRect windowRect{topLeft, bottomRight - topLeft};
    const GridArea chunkArea = GridArea::fromWorld(windowRect);

    //Query chunks in area, and update / render as required
    manager.queryChunks(chunkArea, [&](Chunk* chunkPtr, const GridPosition position) {
        if(!chunkPtr)
        {
            return;
        }
    
        sf::RenderStates states;
        states.transform.translate(position.asWorld());
        //states.texture = &texture;

        if(chunkPtr->isDirty)
        {
            chunkPtr->isDirty = false;
            updateChunkMesh(*chunkPtr);
        }

        window.draw(*chunkPtr->mesh, states);
    });
}

} //namespace ne
