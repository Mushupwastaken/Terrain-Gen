#pragma once


namespace ne {

namespace Tiles {

enum : std::uint16_t {
    None = 0,
    Air,
    Dirt,
    Grass,
    Stone,
};

} //namespace Tiles


struct Tile {
    std::uint16_t value     : 14 {};
    std::uint16_t rotation  : 2  {};
};

static_assert(sizeof(Tile) == 2, "Failed to align to 8 bytes");

} //namespace ne
