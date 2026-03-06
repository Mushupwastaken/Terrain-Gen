#pragma once

#include "core/Vector2Hash.hpp"

#include "Tile.hpp"
#include "Chunk.hpp"

#include "GridPosition.hpp"
#include "GridArea.hpp"

#include <entt/fwd.hpp>

//If we are merging registry into chunkManager, the functions would be implemented here.


namespace ne {

class ChunkManager {
private:
    //Not using "GridPosition" because priv = sf::Vector2f
    std::unordered_map<sf::Vector2i, std::unique_ptr<Chunk>, core::Vector2iHash> m_chunks;
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
    auto chunkPtr = std::make_unique<Chunk>(std::move(chunk));
    chunkPtr->updateMesh = true;

    m_chunks.insert_or_assign(pos.asChunk(), std::move(chunkPtr));
}

void ChunkManager::setTile(Tile tile, GridPosition pos) {    
    const sf::Vector2i chunkPos = pos.asChunk();
    const sf::Vector2i tilePos = pos.asTile();
    
    auto& chunk = m_chunks[chunkPos];

    const sf::Vector2i localPos{ 
        (tilePos.x % constants::chunkSize + constants::chunkSize) % constants::chunkSize,
        (tilePos.y % constants::chunkSize + constants::chunkSize) % constants::chunkSize
    };

    chunk->updateMesh = true;
    chunk->tiles[Chunk::toIndex(localPos)] = tile;
}

inline auto* ChunkManager::getChunk(this auto&& self, GridPosition pos) {
    if(auto it = self.m_chunks.find(pos.asChunk()); it != self.m_chunks.end())
    {
        return it->second.get();
    }

    return static_cast<Chunk*>(nullptr);
}

inline auto* ChunkManager::getTile(this auto&& self, GridPosition pos) {
    if(auto* chunk = self.getChunk(pos))
    {    
        const sf::Vector2i tilePos = pos.asTile();
        
        const sf::Vector2i localPos{ 
            (tilePos.x % constants::chunkSize + constants::chunkSize) % constants::chunkSize,
            (tilePos.y % constants::chunkSize + constants::chunkSize) % constants::chunkSize
        };
    
        return &chunk->tiles[Chunk::toIndex(localPos)];
    }

    return nullptr;
}

template <typename Func>
void ChunkManager::queryChunks(this auto&& self, GridArea area, Func&& callback) {
    const sf::IntRect chunkArea = area.asChunk();
    
    for(int chunkY = chunkArea.position.y; chunkY < chunkArea.position.y + chunkArea.size.y; chunkY++)
    {
        for(int chunkX = chunkArea.position.x; chunkX < chunkArea.position.x + chunkArea.size.x; chunkX++)
        {
            const GridPosition currentPos = GridPosition::fromChunk({chunkX, chunkY});
            auto* chunk = self.getChunk(currentPos);

            callback(chunk, currentPos);
        }
    }
}

template <typename Func>
void ChunkManager::queryTiles(this auto&& self, GridArea area, Func&& callback) {
    const sf::IntRect tileArea = area.asTile();

    self.queryChunks(area, [&](auto* chunk, const GridPosition currentPos) {
        if(chunk == nullptr)
        {
            return;
        }
        
        //Find tile coordinates inside chunk to parce  and callback
        const sf::Vector2i currentTilePos = currentPos.asTile();
        
        const int tileMinX = std::max(currentTilePos.x, tileArea.position.x);
        const int tileMinY = std::max(currentTilePos.y, tileArea.position.y);
        const int tileMaxX = std::min(currentTilePos.x + constants::chunkSize, tileArea.position.x + tileArea.size.x);
        const int tileMaxY = std::min(currentTilePos.y + constants::chunkSize, tileArea.position.y + tileArea.size.y);
    
        for(int tileY = tileMinY ; tileY < tileMaxY; tileY++)
        {
            for(int tileX = tileMinX; tileX < tileMaxX; tileX++)
            {
                const sf::Vector2i localPos{tileX - currentTilePos.x, tileY - currentTilePos.y};
                const std::size_t index = Chunk::toIndex(localPos);

                auto& tile = chunk->tiles[index];
                const GridPosition pos = GridPosition::fromTile({tileX, tileY});

                callback(tile, pos);
            }
        }
    });
}

} //namespace ne