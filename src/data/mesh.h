#pragma once

#include <vector>
#include <string>
#include <ramiel/file.h>

namespace ramiel {

    class MeshBase {
    protected:
        std::vector<Vec3u> triangles;
    public:
        const std::vector<Vec3u>& getTriangles() const { return triangles; }
        virtual ~MeshBase() {}
    };

    template<class Vertex>
    class Mesh : public MeshBase {
        std::vector<Vertex> vertices;
    public:
        Mesh(const std::vector<Vec3u>& triangles, const std::vector<Vertex>& vertices) {
            this->triangles = std::move(triangles);
            this->vertices  = std::move(vertices);
        }
        Mesh(const std::string& filename) {
            getObj<Vertex>(filename, vertices, triangles);
        }
        const std::vector<Vertex>& getVertices() const { return vertices; }
    };


    struct MeshVertex {
        Vec3f pos;
        MeshVertex(Vec3f pos = Vec3f()) : pos(pos) {}
        MeshVertex(Vec3f pos, Vec2f txt, Vec3f nml) : pos(pos) {}
    };

    struct MeshVertexT {
        Vec3f pos;
        Vec2f txt;
        MeshVertexT(Vec3f pos = Vec3f(), Vec2f txt = Vec2f()) : pos(pos), txt(txt) {}
        MeshVertexT(Vec3f pos, Vec2f txt, Vec3f nml) : pos(pos), txt(txt) {}
    };

    struct MeshVertexN {
        Vec3f pos;
        Vec3f nml;
        MeshVertexN(Vec3f pos = Vec3f(), Vec3f nml = Vec3f()) : pos(pos), nml(nml) {}
        MeshVertexN(Vec3f pos, Vec2f txt, Vec3f nml) : pos(pos), nml(nml) {}
    };

    struct MeshVertexTN {
        Vec3f pos;
        Vec2f txt;
        Vec3f nml;
        MeshVertexTN(
            Vec3f pos = Vec3f(),
            Vec2f txt = Vec2f(),
            Vec3f nml = Vec3f()
        ) : pos(pos), txt(txt), nml(nml) {}
    };

}
