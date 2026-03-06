#pragma once

#include <random>


namespace ne {

class PermutationTable {
private:
    std::array<std::uint8_t, 512> m_data;
public: 
    explicit PermutationTable(std::mt19937& gen) {
        std::iota(m_data.begin(), m_data.begin() + 256, 0);
        std::shuffle(m_data.begin(), m_data.begin() + 256, gen);
        std::copy(m_data.begin(), m_data.begin() + 256, m_data.begin() + 256);
    }

    [[nodiscard]] std::uint8_t operator[](std::size_t index) const {
        return m_data[index];
    }
};

} //namespace ne