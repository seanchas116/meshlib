#pragma once
#include "../Mesh.hpp"

namespace meshlib {

class CubeBuilder {
  public:
    Mesh build() const;

    glm::vec3 minPos{-1};
    glm::vec3 maxPos{1};
    MaterialHandle material;
};

} // namespace meshlib
