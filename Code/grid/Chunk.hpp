#pragma once

#include "Config.hpp"
#include "Tile.hpp"

#include <entt/fwd.hpp>


namespace ne {

struct Chunk {    
    std::array<Tile, constants::chunkExtent> tiles;    
    std::unique_ptr<sf::VertexBuffer> mesh = nullptr;
    bool updateMesh = true;
    
    static constexpr sf::Vector2i toPosition(std::size_t index) {
        return sf::Vector2i{
            static_cast<int>(index) % constants::chunkSize,
            static_cast<int>(index) / constants::chunkSize
        };
    }

    static constexpr std::size_t toIndex(sf::Vector2i position) {
        return static_cast<std::size_t>(position.y * constants::chunkSize + position.x);
    }
};

} //namespace ne::grid



