#pragma once

//Might override std::hash for it, sounds much smarter ngl

namespace ne::core {
    
struct Vector2iHash {
    [[nodiscard]] std::size_t operator()(sf::Vector2i position) const noexcept {
        return (static_cast<std::uint64_t>(position.x) << 32) | static_cast<std::uint32_t>(position.y);
    }

    static_assert(sizeof(sf::Vector2i) <= 8,    "sf::Vector2i is too large to pack into a 64-bit hash");
    static_assert(sizeof(std::size_t) >= 8,     "Hash must return at minimum 64-bits");
};

} //namespace ne::core