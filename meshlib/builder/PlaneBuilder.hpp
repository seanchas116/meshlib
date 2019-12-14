#pragma once
#include "../Mesh.hpp"

namespace meshlib {

class PlaneBuilder {
  public:
    Mesh build() const;

    glm::vec3 center{0};
    glm::vec2 size{1};
    int normalAxis{0};
    MaterialHandle material;
};

} // namespace meshlib
