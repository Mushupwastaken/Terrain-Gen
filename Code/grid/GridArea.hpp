#pragma once

#include "core/IntMath.hpp"
#include "Config.hpp"


namespace ne {

class GridArea {
private:
    sf::FloatRect m_worldArea;

    explicit constexpr GridArea(sf::FloatRect worldArea) : 
        m_worldArea(worldArea)
    {};
public:
    static constexpr GridArea fromWorld(sf::FloatRect area) {
        return GridArea(area);
    }
    
    static constexpr GridArea fromTile(sf::IntRect area) {
        return GridArea({
            sf::Vector2f(area.position) * constants::renderScale,
            sf::Vector2f(area.size) * constants::renderScale
        });
    }

    static constexpr GridArea fromChunk(sf::IntRect area) {
        return GridArea({
            sf::Vector2f(area.position) * constants::renderScale * static_cast<float>(constants::chunkSize),
            sf::Vector2f(area.size) * constants::renderScale * static_cast<float>(constants::chunkSize)
        });
    }

    constexpr sf::FloatRect asWorld() const {
        return m_worldArea;
    }

    constexpr sf::IntRect asTile() const {
        const auto [pos, size] = m_worldArea;

        const sf::Vector2f minEdge = pos / constants::renderScale;
        const sf::Vector2f maxEdge = (pos + size) / constants::renderScale;

        const sf::Vector2i minIndex{ 
            core::intFloor(minEdge.x), 
            core::intFloor(minEdge.y) 
        };
        
        const sf::Vector2i maxIndex{ 
            core::intCeil(maxEdge.x), 
            core::intCeil(maxEdge.y) 
        };

        return sf::IntRect{minIndex, maxIndex - minIndex};
    }

    constexpr sf::IntRect asChunk() const {
        const auto [pos, size] = m_worldArea;

        const sf::Vector2f minEdge = pos / constants::renderScale / static_cast<float>(constants::chunkSize);
        const sf::Vector2f maxEdge = (pos + size) / constants::renderScale / static_cast<float>(constants::chunkSize);

        const sf::Vector2i minIndex{ 
            core::intFloor(minEdge.x), 
            core::intFloor(minEdge.y) 
        };
        
        const sf::Vector2i maxIndex{ 
            core::intCeil(maxEdge.x), 
            core::intCeil(maxEdge.y) 
        };

        return sf::IntRect{minIndex, maxIndex - minIndex};
    }
};

} //namespace ne

