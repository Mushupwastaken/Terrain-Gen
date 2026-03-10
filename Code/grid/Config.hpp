#pragma once


namespace ne::constants {

//The plane of a chunk, where chunk extent = size * size
inline constexpr int chunkSize = 32;

//Scale at which the tiles in meshes generate
inline constexpr float renderScale = 4.0f;

//Container size of chunk.tiles
inline constexpr std::size_t chunkExtent = static_cast<std::size_t>(chunkSize * chunkSize);

} //namespace ne::constants

