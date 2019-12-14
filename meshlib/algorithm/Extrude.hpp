#pragma once

#include "../Mesh.hpp"

namespace meshlib {

std::vector<VertexHandle> extrude(Mesh &mesh, const std::vector<VertexHandle> &vertices, bool addFlipFace = false);

} // namespace meshlib
