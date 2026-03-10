#pragma once


namespace ne::constants {

//The plane of a chunk, where chunk extent = size * size
inline constexpr int chunkSize = 32;
inline constexpr float invChunkSize = 1.0f / static_cast<float>(chunkSize);

//Scale at which the tiles in meshes generate
inline constexpr float renderScale = 4.0f;
inline constexpr float invRenderScale = 1.0f / renderScale;

//Container size of chunk.tiles
inline constexpr std::size_t chunkExtent = static_cast<std::size_t>(chunkSize * chunkSize);

} //namespace ne::constants
