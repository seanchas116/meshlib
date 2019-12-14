#pragma once
#include "../Mesh.hpp"

namespace meshlib {

struct BeltElement {
    EdgeHandle edge;
    FaceHandle face;
    bool isEdgeReverse;
};

std::vector<BeltElement> findBelt(const Mesh &mesh, EdgeHandle edge);

} // namespace meshlib
