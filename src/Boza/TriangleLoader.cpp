#include "TriangleLoader.hpp"
#include "Logger.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace boza
{
    MeshData TriangleLoader::load_from_obj(const std::string& filename)
    {
        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn, err;

        if (!LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str()))
        {
            Logger::error("Failed to load OBJ file: {}", err);
            return {};
        }

        if (!warn.empty()) Logger::warn(warn);

        MeshData out_mesh_data;

        out_mesh_data.vertices.reserve(attrib.vertices.size() / 3);

        for (size_t i = 0; i < attrib.vertices.size(); i += 3)
        {
            out_mesh_data.vertices.emplace_back(
                attrib.vertices[i],
                attrib.vertices[i + 1],
                attrib.vertices[i + 2]
            );
        }

        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
                out_mesh_data.indices.push_back(static_cast<uint32_t>(index.vertex_index));
        }

        return out_mesh_data;
    }
}
