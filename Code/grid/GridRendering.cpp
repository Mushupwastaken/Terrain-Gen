#include "GridRendering.hpp"

#include "ChunkConfig.hpp"
#include "Assets.hpp"

#include <iosfwd>


namespace ne {

namespace detail {

sf::Color getAssetColor(std::uint16_t value) {
    constexpr static float phi = 0.618033988749895f;

    const float hue = std::fmod(static_cast<float>(value) * phi, 1.0f) * 6.0f;
    
    auto asColor = [hue](float offset) -> std::uint8_t {
        const float k = std::fmod(offset + hue, 6.0f);
        const float intensity = 0.5f + 0.5f * std::clamp(std::min(k - 3.0f, 9.0f - k), -1.0f, 1.0f);
        
        return static_cast<std::uint8_t>(255 * intensity);
    };

    // Offsets 5, 3, and 1 represent the R, G, and B channel positions
    return sf::Color(asColor(5), asColor(3), asColor(1));
}

} //namespace detail

void updateChunkMesh(Chunk& chunk) {
    static std::array<sf::Vertex, constants::chunkExtent * 6> vertices;
    std::size_t vertexCount{0};

    for(auto [index, tile] : chunk.tiles | std::views::enumerate)
    {
        if(tile.value == Assets::None)
        {
            continue;
        }

        const auto [x, y] = Chunk::toPosition(index);
        const sf::Color color = detail::getAssetColor(tile.value);

        //Apply triangles to array
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 0) * constants::renderScale, color);
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 0) * constants::renderScale, color);
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 1) * constants::renderScale, color);
        
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 0) * constants::renderScale, color);
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 1, y + 1) * constants::renderScale, color);
        vertices[vertexCount++] = sf::Vertex(sf::Vector2f(x + 0, y + 1) * constants::renderScale, color);
    }

    if(!chunk.mesh)
    {
        chunk.mesh = std::make_unique<sf::VertexBuffer>(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Static);
        std::ignore = chunk.mesh->create(vertexCount);
    }

    std::ignore = chunk.mesh->update(vertices.data(), vertexCount, 0);
}

void drawVisibleChunks(sf::RenderWindow& window, ChunkManager& manager) {
    const sf::Vector2i windowSize{window.getSize()};
    
    const sf::Vector2f topLeft     = window.mapPixelToCoords({0, 0});
    const sf::Vector2f bottomRight = window.mapPixelToCoords(windowSize);
    
    const sf::FloatRect windowRect{topLeft, bottomRight - topLeft};
    const GridArea chunkArea = GridArea::fromWorld(windowRect);

    //Query chunks in area, and update / render as required
    manager.queryChunks(chunkArea, [&](Chunk* chunk, const GridPosition position) {
        if(chunk == nullptr)
        {
            return;
        }
    
        sf::RenderStates states;
        states.transform.translate(position.asWorld());
        //states.texture = &texture;

        if(chunk->updateMesh)
        {
            chunk->updateMesh = false;
            updateChunkMesh(*chunk);
        }

        window.draw(*chunk->mesh, states);
    });
}


} //namespace ne

