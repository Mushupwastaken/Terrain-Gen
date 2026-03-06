#pragma once


namespace ne::core {

template <typename T>
constexpr int intFloor(T value) {
    int intValue = static_cast<int>(value);
    return (value < intValue)? intValue - 1 : intValue;
}

template <typename T>
constexpr int intCeil(T value) {
    int intValue = static_cast<int>(value);
    return (value > intValue)? intValue + 1 : intValue;
}

} //namespace ne::core