#pragma once

#include <cstdint>
#include <functional>

namespace meshlib {

template <typename Tag>
struct Handle {
    Handle() : index(0) {}
    explicit Handle(int index) : index(index) {}
    bool operator==(const Handle &other) const { return index == other.index; }
    bool operator!=(const Handle &other) const { return !operator==(other); }
    int index;
};

struct VertexHandleTag {};
struct UVPointHandleTag {};
struct EdgeHandleTag {};
struct FaceHandleTag {};
struct MaterialHandleTag {};

using VertexHandle = Handle<VertexHandleTag>;
using UVPointHandle = Handle<UVPointHandleTag>;
using EdgeHandle = Handle<EdgeHandleTag>;
using FaceHandle = Handle<FaceHandleTag>;
using MaterialHandle = Handle<MaterialHandleTag>;

} // namespace meshlib

namespace std {

template <typename Tag>
struct hash<meshlib::Handle<Tag>> {
    size_t operator()(const meshlib::Handle<Tag> &value) const {
        return std::hash<int>()(value.index);
    }
};

} // namespace std
