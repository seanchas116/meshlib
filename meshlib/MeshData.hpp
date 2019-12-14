#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json_fwd.hpp>
#include <vector>

namespace meshlib {

class Mesh;

struct MeshData {
    std::vector<glm::vec3> vertexPositionArray;
    std::vector<uint8_t> vertexSelectedArray;
    std::vector<float> vertexCornerArray;

    std::vector<glm::vec2> uvPositionArray;
    std::vector<int32_t> uvVertexArray;

    std::vector<uint8_t> edgeSharpArray;
    std::vector<float> edgeCreaseArray;
    std::vector<std::array<int32_t, 2>> edgeVerticesArray;

    std::vector<int32_t> faceMaterialArray;
    std::vector<int32_t> faceVertexCountArray;
    std::vector<int32_t> faceUVPointArray;

    explicit MeshData(const Mesh &mesh);
    Mesh toMesh() const;
};

void to_json(nlohmann::json &json, const MeshData &meshData);
void from_json(const nlohmann::json &json, MeshData &meshData);

} // namespace meshlib
