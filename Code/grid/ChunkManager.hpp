#pragma once

#include "core/Vector2Hash.hpp"

#include "Tile.hpp"
#include "Chunk.hpp"

#include "GridPosition.hpp"
#include "GridArea.hpp"


namespace ne {

class ChunkManager {
private:
    //Not using "GridPosition" because priv = sf::Vector2f
    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>> m_chunks;

    //For caching getChunk -> important for constant use
    Chunk* m_lastChunkPtr = nullptr; 
    sf::Vector2i m_lastChunkPos;
public:
    void setChunk(Chunk chunk, GridPosition pos);
    void setTile(Tile tile, GridPosition pos);
    
    [[nodiscard]] inline auto* getChunk(this auto&& self, GridPosition pos); 
    [[nodiscard]] inline auto* getTile(this auto&& self, GridPosition pos);

    template <typename Func>
    void queryChunks(this auto&& self, GridArea area, Func&& callback);

    template <typename Func>
    void queryTiles(this auto&& self, GridArea area, Func&& callback);
};

//Public function API
void ChunkManager::setChunk(Chunk chunk, GridPosition pos) {
    const auto chunkPos = pos.asChunk();
    
    auto [it, _] = m_chunks.insert_or_assign(chunkPos, std::make_unique<Chunk>(std::move(chunk)));
    it->second->isDirty = true;

    m_lastChunkPtr = it->second.get();
    m_lastChunkPos = chunkPos;
}

void ChunkManager::setTile(Tile tile, GridPosition pos) {    
    const sf::Vector2i chunkPos = pos.asChunk();
    const sf::Vector2i tilePos = pos.asTile();
    
    auto& chunkPtr = m_chunks[chunkPos];
    
    if(!chunkPtr)
    {
        chunkPtr = std::make_unique<Chunk>();
    }

    const sf::Vector2i localPos{ 
        (tilePos.x % constants::chunkSize + constants::chunkSize) % constants::chunkSize,
        (tilePos.y % constants::chunkSize + constants::chunkSize) % constants::chunkSize
    };

    chunkPtr->isDirty = true;
    chunkPtr->tiles[Chunk::toIndex(localPos)] = tile;
}

inline auto* ChunkManager::getChunk(this auto&& self, GridPosition pos) {
    const auto chunkPos = pos.asChunk();
    
    if(self.m_lastChunkPtr && self.m_lastChunkPos == chunkPos)
    {
        return self.m_lastChunkPtr;
    } 
    
    if(auto it = self.m_chunks.find(chunkPos); it != self.m_chunks.end())
    {
        self.m_lastChunkPtr = it->second.get();
        self.m_lastChunkPos = chunkPos;
        return self.m_lastChunkPtr;
    }

    self.m_lastChunkPtr = nullptr; 
    return static_cast<Chunk*>(nullptr);
}

inline auto* ChunkManager::getTile(this auto&& self, GridPosition pos) {
    if(auto* chunkPtr = self.getChunk(pos))
    {    
        const sf::Vector2i tilePos = pos.asTile();
        
        const sf::Vector2i localPos{ 
            (tilePos.x % constants::chunkSize + constants::chunkSize) % constants::chunkSize,
            (tilePos.y % constants::chunkSize + constants::chunkSize) % constants::chunkSize
        };
    
        return &chunkPtr->tiles[Chunk::toIndex(localPos)];
    }

    return static_cast<Tile*>(nullptr);
}

template <typename Func>
void ChunkManager::queryChunks(this auto&& self, GridArea area, Func&& callback) {
    const sf::IntRect chunkArea = area.asChunk();
    
    for(int chunkY = chunkArea.position.y; chunkY < chunkArea.position.y + chunkArea.size.y; chunkY++)
    {
        for(int chunkX = chunkArea.position.x; chunkX < chunkArea.position.x + chunkArea.size.x; chunkX++)
        {
            const GridPosition currentPos = GridPosition::fromChunk({chunkX, chunkY});
            auto* chunkPtr = self.getChunk(currentPos);

            callback(chunkPtr, currentPos);
        }
    }
}

template <typename Func>
void ChunkManager::queryTiles(this auto&& self, GridArea area, Func&& callback) {
    const sf::IntRect tileArea = area.asTile();

    self.queryChunks(area, [&](auto* chunkPtr, const GridPosition currentPos) {
        if(chunkPtr == nullptr)
        {
            return;
        }
        
        const sf::Vector2i currentTilePos = currentPos.asTile();
        
        const int tileMinX = std::max(currentTilePos.x, tileArea.position.x);
        const int tileMinY = std::max(currentTilePos.y, tileArea.position.y);
        const int tileMaxX = std::min(currentTilePos.x + constants::chunkSize - 1, tileArea.position.x + tileArea.size.x);
        const int tileMaxY = std::min(currentTilePos.y + constants::chunkSize - 1, tileArea.position.y + tileArea.size.y);
    
        for(int tileY = tileMinY ; tileY <= tileMaxY; tileY++)
        {
            for(int tileX = tileMinX; tileX <= tileMaxX; tileX++)
            {
                const sf::Vector2i localPos{ 
                    (tileX % constants::chunkSize + constants::chunkSize) % constants::chunkSize, 
                    (tileY % constants::chunkSize + constants::chunkSize) % constants::chunkSize 
                };
                
                auto& tile = chunkPtr->tiles[Chunk::toIndex(localPos)];
                const GridPosition pos = GridPosition::fromTile({tileX, tileY});

                callback(tile, pos);
            }
        }
    });
}

} //namespace ne

