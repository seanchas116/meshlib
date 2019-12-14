#pragma once
#include "Handle.hpp"
#include <array>
#include <glm/glm.hpp>
#include <range/v3/action/join.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/transform.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace meshlib {

class Mesh {
    struct VertexData {
        bool isDeleted = false;
        bool isSelected = false;
        float corner = 0;
        glm::vec3 position = glm::vec3(0);
        std::vector<UVPointHandle> uvPoints;
        std::vector<EdgeHandle> edges;
    };

    struct UVPointData {
        bool isDeleted = false;
        glm::vec2 uvPosition = glm::vec2(0);
        VertexHandle vertex;
        std::vector<FaceHandle> faces;
    };

    struct EdgeData {
        bool isDeleted = false;
        bool isSharp = false;
        float crease = 0;
        std::array<VertexHandle, 2> vertices;
        std::vector<FaceHandle> faces;
    };

    struct FaceData {
        bool isDeleted = false;
        MaterialHandle material;
        std::vector<UVPointHandle> uvPoints;
        std::vector<EdgeHandle> edges;
    };

    auto &vertexData(VertexHandle handle) { return _vertices[handle.index]; }
    auto &vertexData(VertexHandle handle) const { return _vertices[handle.index]; }

    auto &uvPointData(UVPointHandle handle) { return _uvPoints[handle.index]; }
    auto &uvPointData(UVPointHandle handle) const { return _uvPoints[handle.index]; }

    auto &edgeData(EdgeHandle handle) { return _edges[handle.index]; }
    auto &edgeData(EdgeHandle handle) const { return _edges[handle.index]; }

    auto &faceData(FaceHandle handle) { return _faces[handle.index]; }
    auto &faceData(FaceHandle handle) const { return _faces[handle.index]; }

    std::vector<VertexData> _vertices;
    std::vector<UVPointData> _uvPoints;
    std::vector<EdgeData> _edges;
    std::vector<FaceData> _faces;

  public:
    VertexHandle addVertex(glm::vec3 position);
    UVPointHandle addUVPoint(VertexHandle v, glm::vec2 position);
    EdgeHandle addEdge(const std::array<VertexHandle, 2> &vertices);
    FaceHandle addFace(const std::vector<UVPointHandle> &uvPoints, MaterialHandle material);

    void removeVertex(VertexHandle v);
    void removeUVPoint(UVPointHandle v);
    void removeEdge(EdgeHandle e);
    void removeFace(FaceHandle f);

    Mesh collectGarbage() const;

    void clear();

    // TODO: exclude deleted items
    size_t allVertexCount() const { return _vertices.size(); }
    size_t allUVPointCount() const { return _uvPoints.size(); }
    size_t allEdgeCount() const { return _edges.size(); }
    size_t allFaceCount() const { return _faces.size(); }

    // vertices including deleted ones
    auto allVertices() const {
        return ranges::view::iota(0, int(_vertices.size())) | ranges::view::transform([](int index) { return VertexHandle(index); });
    }
    auto allUVPoints() const {
        return ranges::view::iota(0, int(_uvPoints.size())) | ranges::view::transform([](int index) { return UVPointHandle(index); });
    }
    auto allEdges() const {
        return ranges::view::iota(0, int(_edges.size())) | ranges::view::transform([](int index) { return EdgeHandle(index); });
    }
    auto allFaces() const {
        return ranges::view::iota(0, int(_faces.size())) | ranges::view::transform([](int index) { return FaceHandle(index); });
    }

    auto vertices() const {
        return allVertices() | ranges::view::filter([this](auto handle) { return !vertexData(handle).isDeleted; });
    }
    auto uvPoints() const {
        return allUVPoints() | ranges::view::filter([this](auto handle) { return !uvPointData(handle).isDeleted; });
    }
    auto edges() const {
        return allEdges() | ranges::view::filter([this](auto handle) { return !edgeData(handle).isDeleted; });
    }
    auto faces() const {
        return allFaces() | ranges::view::filter([this](auto handle) { return !faceData(handle).isDeleted; });
    }

    auto uvPoints(VertexHandle v) const {
        return vertexData(v).uvPoints | ranges::view::filter([this](auto handle) { return !uvPointData(handle).isDeleted; });
    }
    auto edges(VertexHandle v) const {
        return vertexData(v).edges | ranges::view::filter([this](auto handle) { return !edgeData(handle).isDeleted; });
    }

    auto vertex(UVPointHandle p) const {
        return uvPointData(p).vertex;
    }
    auto faces(UVPointHandle p) const {
        return uvPointData(p).faces | ranges::view::filter([this](auto handle) { return !faceData(handle).isDeleted; });
    }

    auto faces(VertexHandle v) const {
        return uvPoints(v) | ranges::view::transform([this](UVPointHandle uvPoint) {
                   return faces(uvPoint);
               }) |
               ranges::action::join;
    }

    auto &vertices(EdgeHandle e) const { return edgeData(e).vertices; }
    auto faces(EdgeHandle e) const {
        return edgeData(e).faces | ranges::view::filter([this](auto handle) { return !faceData(handle).isDeleted; });
    }

    auto &uvPoints(FaceHandle f) const { return faceData(f).uvPoints; }
    auto vertices(FaceHandle f) const {
        return uvPoints(f) | ranges::view::transform([this](UVPointHandle uvPoint) {
                   return vertex(uvPoint);
               });
    }

    auto &edges(FaceHandle f) const { return faceData(f).edges; }

    template <typename TVertices, CONCEPT_REQUIRES_(ranges::Range<TVertices>())>
    auto edges(TVertices &&vertices) const {
        std::unordered_map<EdgeHandle, size_t> edgeCounts;

        for (auto v : vertices) {
            for (auto e : edges(v)) {
                if (edgeCounts.find(e) != edgeCounts.end()) {
                    ++edgeCounts[e];
                } else {
                    edgeCounts[e] = 1;
                }
            }
        }
        std::unordered_set<EdgeHandle> edges;

        for (auto [edge, count] : edgeCounts) {
            if (count == 2) {
                edges.insert(edge);
            }
        }

        return edges;
    }

    template <typename TVertices, CONCEPT_REQUIRES_(ranges::Range<TVertices>())>
    auto faces(TVertices &&vertices) const {
        std::unordered_map<FaceHandle, size_t> faceCounts;

        for (auto v : vertices) {
            for (auto &f : faces(v)) {
                if (faceCounts.find(f) != faceCounts.end()) {
                    ++faceCounts[f];
                } else {
                    faceCounts[f] = 1;
                }
            }
        }
        std::unordered_set<FaceHandle> faces;

        for (auto [face, count] : faceCounts) {
            if (count == this->vertices(face).size()) {
                faces.insert(face);
            }
        }

        return faces;
    }

    bool isSelected(VertexHandle v) const { return vertexData(v).isSelected; }
    void setSelected(VertexHandle v, bool selected) { vertexData(v).isSelected = selected; }

    float corner(VertexHandle v) const { return vertexData(v).corner; }
    void setCorner(VertexHandle v, float corner) { vertexData(v).corner = corner; }

    glm::vec3 position(VertexHandle v) const { return vertexData(v).position; }
    void setPosition(VertexHandle v, glm::vec3 pos) { vertexData(v).position = pos; }

    glm::vec2 uvPosition(UVPointHandle uv) const { return uvPointData(uv).uvPosition; }
    void setUVPosition(UVPointHandle uv, glm::vec2 pos) { uvPointData(uv).uvPosition = pos; }

    std::array<glm::vec3, 2> positions(EdgeHandle e) const {
        auto pos0 = position(vertices(e)[0]);
        auto pos1 = position(vertices(e)[1]);
        return {pos0, pos1};
    }

    bool isSharp(EdgeHandle edge) const { return edgeData(edge).isSharp; }
    void setSharp(EdgeHandle edge, bool isSharp) { edgeData(edge).isSharp = isSharp; }

    float crease(EdgeHandle edge) const { return edgeData(edge).crease; }
    void setCrease(EdgeHandle edge, float crease) { edgeData(edge).crease = crease; }

    MaterialHandle material(FaceHandle face) const { return faceData(face).material; }
    void setMaterial(FaceHandle face, MaterialHandle material) { faceData(face).material = material; }

    glm::vec3 calculateNormal(FaceHandle face) const;

    void selectAll();
    void deselectAll();

    auto selectedVertices() const {
        return vertices() | ranges::view::filter([this](auto handle) { return isSelected(handle); });
    }
    auto selectedEdges() const {
        return edges(selectedVertices());
    }
    auto selectedFaces() const {
        return faces(selectedVertices());
    }

    void merge(const Mesh &other);
};

} // namespace meshlib
