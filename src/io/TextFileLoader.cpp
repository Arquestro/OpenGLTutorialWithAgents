#include <io/TextFileLoader.hpp>

#include <fstream>
#include <sstream>

namespace io
{
    bool ReadTextFile(const std::string &path, std::string &out_text)
    {
        std::ifstream input(path);
        if (!input.is_open())
        {
            return false;
        }
        std::ostringstream stream;
        stream << input.rdbuf();
        out_text = stream.str();
        return true;
    }
}
