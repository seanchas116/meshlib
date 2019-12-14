#include "CylinderBuilder.hpp"
#include "../algorithm/Extrude.hpp"
#include "CircleBuilder.hpp"
#include <QtGlobal>

using namespace glm;

namespace meshlib {

Mesh CylinderBuilder::build() const {
    CircleBuilder circleBuilder;
    circleBuilder.center = center;
    circleBuilder.radius = radius;
    circleBuilder.segmentCount = segmentCount;
    circleBuilder.normalAxis = axis;

    Mesh mesh = circleBuilder.build();
    auto extrudedVertices = extrude(mesh, mesh.vertices() | ranges::to_vector, true);

    vec3 extrudeOffset(0);
    extrudeOffset[axis] = height;

    for (auto &&v : extrudedVertices) {
        mesh.setPosition(v, mesh.position(v) + extrudeOffset);
    }

    return mesh;
}

} // namespace meshlib
