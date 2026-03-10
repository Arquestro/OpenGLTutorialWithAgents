#pragma once

#include <gl/GlApi.hpp>

#include <cstddef>

namespace gl
{
    class RectangleMesh
    {
    public:
        RectangleMesh() = default;
        ~RectangleMesh();
        RectangleMesh(const RectangleMesh &) = delete;
        RectangleMesh &operator=(const RectangleMesh &) = delete;
        RectangleMesh(RectangleMesh &&other) noexcept;
        RectangleMesh &operator=(RectangleMesh &&other) noexcept;
        bool Create(const float *vertices, size_t float_count);
        void Draw() const;

    private:
        GLuint VERTEX_ARRAY = 0;
        GLuint VERTEX_BUFFER = 0;
        GLsizei VERTEX_COUNT = 0;
    };
}
