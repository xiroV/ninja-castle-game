#include"common.h"
#ifndef __COLLISION_H_INCLUDED__
#define __COLLISION_H_INCLUDED__

class Collision {
    public:
        std::vector<glm::vec3> floor_verts;
        std::vector<glm::vec3> floor_norms;
        std::vector<glm::vec3> wall_verts;
        std::vector<glm::vec3> wall_norms;
       
        Collision();
        void init(std::string filename);
        bool on_floor(float x, float y);
        bool wall_hit_x(float x, float y);
        bool wall_hit_y(float x, float y);
};

#endif
