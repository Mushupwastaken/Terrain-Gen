#pragma once

#include "PermutationTable.hpp"
#include "Noise.hpp"

#include "grid/Chunk.hpp"
#include "grid/ChunkManager.hpp"

#include "grid/GridPosition.hpp"
#include "grid/GridArea.hpp"

#include "grid/ChunkManager.hpp"

namespace ne {

[[nodiscard]] Chunk generateChunk(GridPosition pos);

} //namespace ne
