#pragma once


namespace std {

template<typename T>
struct hash<sf::Vector2<T>> {
    size_t operator()(const sf::Vector2<T>& vec) const {
        const size_t a = hash<T>{}(vec.x);
        const size_t b = hash<T>{}(vec.y);
        
        return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
    }
};

} //namespace std

