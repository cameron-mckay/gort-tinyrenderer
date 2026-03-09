//
// Created by CameronM on 3/9/2026.
//

#ifndef GORT_TINYRENDERER_MODEL_H
#define GORT_TINYRENDERER_MODEL_H

#include <string>
#include <vector>

struct vertex {
    float x, y, z;
};

struct face {
    int v0, v1, v2;
};

class Model
{
public:
    Model(std::string path);
    std::vector<vertex> vertices;
    std::vector<face> faces;
};

#endif //GORT_TINYRENDERER_MODEL_H