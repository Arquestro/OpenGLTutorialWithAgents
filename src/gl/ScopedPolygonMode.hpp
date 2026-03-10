#pragma once

#include "gl/GlApi.hpp"

namespace gl {
    class ScopedPolygonMode {
    public:
        explicit ScopedPolygonMode(GLenum mode);
        ~ScopedPolygonMode();
        ScopedPolygonMode(const ScopedPolygonMode &) = delete;
        ScopedPolygonMode &operator=(const ScopedPolygonMode &) = delete;
        ScopedPolygonMode(ScopedPolygonMode &&) = delete;
        ScopedPolygonMode &operator=(ScopedPolygonMode &&) = delete;

    private:
        GLint PREVIOUS_MODE_FRONT = GL_FILL;
        GLint PREVIOUS_MODE_BACK = GL_FILL;
        bool HAS_PREVIOUS_MODE = false;
    };
}
