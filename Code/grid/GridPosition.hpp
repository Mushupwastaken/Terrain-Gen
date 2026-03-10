#pragma once

#include "core/IntMath.hpp"
#include "Config.hpp"


namespace ne {

class GridPosition {
private:
    sf::Vector2f m_worldPos;

    explicit constexpr GridPosition(sf::Vector2f worldPos) : 
        m_worldPos(worldPos) 
    {};
public:
    static constexpr GridPosition fromWorld(sf::Vector2f pos) {
        return GridPosition(pos);
    }
    
    static constexpr GridPosition fromTile(sf::Vector2i pos) {
        return GridPosition(sf::Vector2f(pos) * constants::renderScale);
    }

    static constexpr GridPosition fromChunk(sf::Vector2i pos) {
        return GridPosition(sf::Vector2f(pos) * constants::renderScale * static_cast<float>(constants::chunkSize));
    }

    constexpr sf::Vector2f asWorld() const {
        return m_worldPos;
    }

    constexpr sf::Vector2i asTile() const {
        const sf::Vector2f pos = m_worldPos / constants::renderScale;
        
        return sf::Vector2i{ 
            core::intFloor(pos.x), 
            core::intFloor(pos.y) 
        };
    }

    constexpr sf::Vector2i asChunk() const {
        const sf::Vector2f pos = m_worldPos / constants::renderScale / static_cast<float>(constants::chunkSize);

        return sf::Vector2i{
            core::intFloor(pos.x),
            core::intFloor(pos.y)
        };
    }
};

} //namespace ne
