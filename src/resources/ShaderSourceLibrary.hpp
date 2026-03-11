#pragma once

#include <string>
#include <string_view>

namespace resources
{
    struct ShaderSourcePair
    {
        std::string VERTEX_SOURCE;
        std::string FRAGMENT_SOURCE;
    };

    class ShaderSourceLibrary
    {
    public:
        explicit ShaderSourceLibrary(std::string_view shader_directory_path);
        bool LoadPairWithFallback(const char *vertex_file_name, const char *fragment_file_name,
                                  const char *fallback_vertex_file_name, const char *fallback_fragment_file_name,
                                  ShaderSourcePair &out_sources) const;

    private:
        bool LoadPair(const char *vertex_file_name, const char *fragment_file_name,
                      ShaderSourcePair &out_sources) const;
        std::string BuildPath(const char *file_name) const;

        std::string_view SHADER_ROOT_PATH;
    };
}
