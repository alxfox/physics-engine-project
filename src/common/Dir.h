#ifndef GP_COMMON_DIR_H
#define GP_COMMON_DIR_H

#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#else
#include <dirent.h>
#endif

namespace gp
{
class Dir
{
private:
    std::string m_path;

public:
    enum {
        Dirs = 0x01,
        Files = 0x02
    };

    Dir(std::string const& path) : m_path(path) {}
    std::vector<std::string> list(unsigned types = 0x00);
    bool isAbsolutePath();
};
}

#endif // GP_COMMON_DIR_H

