#include "gl/RectangleMesh.hpp"

#include <cassert>
#include <iostream>

namespace {
    constexpr size_t FLOATS_PER_VERTEX = 5;
}

namespace gl {
    bool RectangleMesh::Create(const float *vertices, size_t float_count) {
        assert(vertices != nullptr);
        assert(VERTEX_ARRAY == 0);
        assert(VERTEX_BUFFER == 0);
        if (float_count % FLOATS_PER_VERTEX != 0) {
            std::cerr << "RectangleMesh data has invalid float count.\n";
            return false;
        }
        VERTEX_COUNT = static_cast<GLsizei>(float_count / FLOATS_PER_VERTEX);
        glGenVertexArrays(1, &VERTEX_ARRAY);
        glGenBuffers(1, &VERTEX_BUFFER);
        if (VERTEX_ARRAY == 0 || VERTEX_BUFFER == 0) {
            std::cerr << "Failed to create mesh buffers.\n";
            if (VERTEX_BUFFER != 0) {
                glDeleteBuffers(1, &VERTEX_BUFFER);
                VERTEX_BUFFER = 0;
            }
            if (VERTEX_ARRAY != 0) {
                glDeleteVertexArrays(1, &VERTEX_ARRAY);
                VERTEX_ARRAY = 0;
            }
            VERTEX_COUNT = 0;
            return false;
        }
        glBindVertexArray(VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, VERTEX_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(float_count * sizeof(float)), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, FLOATS_PER_VERTEX * sizeof(float),
                              reinterpret_cast<void *>(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
        return true;
    }

    void RectangleMesh::Draw() const {
        assert(VERTEX_ARRAY != 0);
        assert(VERTEX_COUNT > 0);
        glBindVertexArray(VERTEX_ARRAY);
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
    }

    RectangleMesh::~RectangleMesh() {
        if (VERTEX_BUFFER != 0) {
            glDeleteBuffers(1, &VERTEX_BUFFER);
        }
        if (VERTEX_ARRAY != 0) {
            glDeleteVertexArrays(1, &VERTEX_ARRAY);
        }
    }

    RectangleMesh::RectangleMesh(RectangleMesh &&other) noexcept {
        VERTEX_ARRAY = other.VERTEX_ARRAY;
        VERTEX_BUFFER = other.VERTEX_BUFFER;
        VERTEX_COUNT = other.VERTEX_COUNT;
        other.VERTEX_ARRAY = 0;
        other.VERTEX_BUFFER = 0;
        other.VERTEX_COUNT = 0;
    }

    RectangleMesh &RectangleMesh::operator=(RectangleMesh &&other) noexcept {
        if (this == &other) {
            return *this;
        }
        if (VERTEX_BUFFER != 0) {
            glDeleteBuffers(1, &VERTEX_BUFFER);
        }
        if (VERTEX_ARRAY != 0) {
            glDeleteVertexArrays(1, &VERTEX_ARRAY);
        }
        VERTEX_ARRAY = other.VERTEX_ARRAY;
        VERTEX_BUFFER = other.VERTEX_BUFFER;
        VERTEX_COUNT = other.VERTEX_COUNT;
        other.VERTEX_ARRAY = 0;
        other.VERTEX_BUFFER = 0;
        other.VERTEX_COUNT = 0;
        return *this;
    }
}
