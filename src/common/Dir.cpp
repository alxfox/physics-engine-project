#include "Dir.h"

std::vector<std::string> gp::Dir::list(unsigned types)
{
    std::vector<std::string> entries;
#ifdef _WIN32
    char path[2048];
    sprintf(path, "%s/*", m_path.c_str());
    
    WIN32_FIND_DATA data;
    HANDLE hfind = INVALID_HANDLE_VALUE;

    auto checkAndAdd = [&entries, &types] (WIN32_FIND_DATA data) {
        if (data.cFileName[0] != '.'
                && (!(types & Dirs) || (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                && (!(types & Files) || !(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))) {
            entries.push_back(data.cFileName);
        }
    };

    hfind = FindFirstFile(path, &data);
    checkAndAdd(data);

    if (hfind != INVALID_HANDLE_VALUE) {
        while (FindNextFile(hfind, &data) != 0) {
            checkAndAdd(data);
        }
    }

    FindClose(hfind);

#else
    DIR *dir = opendir(m_path.c_str());
    struct dirent *item = NULL;
    if (dir != NULL) {
        while ((item = readdir(dir))) {
            if (  item->d_name[0] != '.' &&
                  (!(types & Dirs) || (item->d_type & DT_DIR)) &&
                  (!(types & Files) || !(item->d_type & DT_DIR))) {
                entries.push_back(item->d_name);
            }
        }
        closedir(dir);
    }
#endif

    return entries;
}

bool gp::Dir::isAbsolutePath()
{
#ifdef _WIN32
    return m_path.size() >= 2 &&
           m_path[1] == ':' &&
           ((m_path[0] >= 'A' && m_path[0] <= 'Z') ||
           (m_path[0] >= 'a' && m_path[0] <= 'z'));
#else
    return (m_path.size() >= 1) && m_path[0] == '/';
#endif
}
