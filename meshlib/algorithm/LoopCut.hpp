#pragma once
#include "../Mesh.hpp"

namespace meshlib {

std::vector<VertexHandle> loopCut(Mesh &mesh, EdgeHandle edge, float cutPosition);

} // namespace meshlib
