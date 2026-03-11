#include <resources/ShaderSourceLibrary.hpp>

#include <io/TextFileLoader.hpp>

#include <cassert>
#include <iostream>

namespace resources
{
    ShaderSourceLibrary::ShaderSourceLibrary(std::string_view shader_directory_path)
        : SHADER_ROOT_PATH(shader_directory_path)
    {
    }

    bool ShaderSourceLibrary::LoadPairWithFallback(const char *vertex_file_name, const char *fragment_file_name,
                                                   const char *fallback_vertex_file_name,
                                                   const char *fallback_fragment_file_name,
                                                   ShaderSourcePair &out_sources) const
    {
        if (LoadPair(vertex_file_name, fragment_file_name, out_sources))
        {
            return true;
        }
        std::cerr << "Failed to load primary shader files. Falling back to error shader files.\n";
        if (!LoadPair(fallback_vertex_file_name, fallback_fragment_file_name, out_sources))
        {
            std::cerr << "Failed to load fallback shader files as well.\n";
            return false;
        }
        return true;
    }

    bool ShaderSourceLibrary::LoadPair(const char *vertex_file_name, const char *fragment_file_name,
                                       ShaderSourcePair &out_sources) const
    {
        assert(vertex_file_name != nullptr);
        assert(fragment_file_name != nullptr);
        std::string vertex_path = BuildPath(vertex_file_name);
        std::string fragment_path = BuildPath(fragment_file_name);
        if (!io::ReadTextFile(vertex_path, out_sources.VERTEX_SOURCE))
        {
            std::cerr << "Could not read vertex shader file: " << vertex_path << '\n';
            return false;
        }
        if (!io::ReadTextFile(fragment_path, out_sources.FRAGMENT_SOURCE))
        {
            std::cerr << "Could not read fragment shader file: " << fragment_path << '\n';
            return false;
        }
        return true;
    }

    std::string ShaderSourceLibrary::BuildPath(const char *file_name) const
    {
        assert(file_name != nullptr);
        assert(!SHADER_ROOT_PATH.empty());
        std::string path;
        path.reserve(SHADER_ROOT_PATH.size() + 1 + std::char_traits<char>::length(file_name));
        path.append(SHADER_ROOT_PATH.data(), SHADER_ROOT_PATH.size());
        path += '/';
        path += file_name;
        return path;
    }
}
