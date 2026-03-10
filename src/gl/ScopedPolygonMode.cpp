#include <gl/ScopedPolygonMode.hpp>

namespace gl
{
    ScopedPolygonMode::ScopedPolygonMode(GLenum mode)
    {
        GLint modes[2] = {GL_FILL, GL_FILL};
        glGetIntegerv(GL_POLYGON_MODE, modes);
        PREVIOUS_MODE_FRONT = modes[0];
        PREVIOUS_MODE_BACK = modes[1];
        HAS_PREVIOUS_MODE = true;
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }

    ScopedPolygonMode::~ScopedPolygonMode()
    {
        if (!HAS_PREVIOUS_MODE)
        {
            return;
        }
        glPolygonMode(GL_FRONT, static_cast<GLenum>(PREVIOUS_MODE_FRONT));
        glPolygonMode(GL_BACK, static_cast<GLenum>(PREVIOUS_MODE_BACK));
    }
}
