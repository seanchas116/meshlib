#pragma once
#include "../Mesh.hpp"

namespace meshlib {

std::unordered_set<VertexHandle> findConnected(const Mesh &mesh, const std::vector<VertexHandle> &vertices);

} // namespace meshlib
