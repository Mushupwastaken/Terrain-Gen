#pragma once


namespace ne::Assets {

enum : std::uint16_t {
    None = 0,
    Air,
    Dirt,
    Grass,
    Stone,

    //ITEMS
    WoodenSword = 16384,
    WoodenPickaxe,
    WoodenAxe
};

[[nodiscard]] constexpr bool isTile(std::uint16_t value) noexcept {
    return static_cast<std::uint16_t>(value) < 16384;
}

[[nodiscard]] constexpr bool isItem(std::uint16_t value) noexcept {
    return static_cast<std::uint16_t>(value) >= 16384;
}

} //namespace ne::Asset