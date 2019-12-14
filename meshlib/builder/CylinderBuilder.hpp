#pragma once
#include "../Mesh.hpp"

namespace meshlib {

class CylinderBuilder {
  public:
    Mesh build() const;

    glm::vec3 center{0};
    float radius{1};
    float height{1};
    int segmentCount{16};
    int axis{0};
    MaterialHandle material;
};

} // namespace meshlib
