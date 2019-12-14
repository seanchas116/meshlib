#include "FlipFace.hpp"
#include <range/v3/view/reverse.hpp>

namespace meshlib {

FaceHandle flipFace(Mesh &mesh, FaceHandle face) {
    auto reverseUVPoints = mesh.uvPoints(face) | ranges::views::reverse | ranges::to_vector;
    auto newFace = mesh.addFace(reverseUVPoints, mesh.material(face));
    mesh.removeFace(face);
    return newFace;
}

} // namespace meshlib
