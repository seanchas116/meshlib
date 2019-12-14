#pragma once
#include "../Mesh.hpp"

namespace meshlib {

std::vector<EdgeHandle> findLoop(const Mesh &mesh, EdgeHandle edge);

} // namespace meshlib
