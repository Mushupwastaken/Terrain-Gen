#pragma once

#include "core/IntMath.hpp"
#include "ChunkConfig.hpp"


//Using templates could fix the fallbacks of this function (recomputation)
//Making it natively store the inputted type and casting from there would improve aswell

//Eg:
/*

template <typename T>
class GridPosition {
    //fromWorld sets stored to sf::Vector2f, else-wise, sf::Vector2i

};

*/


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
        return GridPosition(sf::Vector2f(pos * constants::chunkSize) * constants::renderScale);
    }

    constexpr sf::Vector2f asWorld() const {
        return m_worldPos;
    }

    constexpr sf::Vector2i asTile() const {
        sf::Vector2f pos = m_worldPos * constants::invRenderScale;
        
        return sf::Vector2i{
            core::intFloor(pos.x), 
            core::intFloor(pos.y)
        };
    }

    constexpr sf::Vector2i asChunk() const {
        sf::Vector2f pos = m_worldPos * constants::invChunkSize * constants::invRenderScale;
        
        return sf::Vector2i{
            core::intFloor(pos.x),
            core::intFloor(pos.y)
        };
    }
};

} //namespace ne