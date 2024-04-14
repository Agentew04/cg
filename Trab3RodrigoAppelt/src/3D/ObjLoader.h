#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <string>
#include <vector>
#include "../Math/Vector3.h"

struct ObjFile {
public:
    std::vector<Vector3> vertices;
    std::vector<std::vector<int>> faces;
    Vector3 scale;
};

class ObjLoader {
public:

    static ObjFile load(const std::string& filename);

private:
    // Add private member variables here

    // Add private helper functions here
};

#endif // __OBJLOADER_H__