#include"../common.h"

#ifndef __MATLOADER_H_INCLUDED__
#define __MATLOADER_H_INCLUDED__


struct material_t {
    std::string name;
    float ambient[4];
    glm::vec4 diffuse;
    float specular[4];
    float exponent;
    float alpha;
    int illum;
};

class MaterialLoader {
    public:
        std::vector<material_t> materials;

        MaterialLoader();
        void load(std::string filename);
        material_t get(std::string name);
};

#endif
