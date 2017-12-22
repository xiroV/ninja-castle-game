#include"common.h"

#ifndef __core_h_included__
#define __core_h_included__

class Collision {
    public:
        glm::vec2 center_point;
        std::vector<glm::vec3> floor_verts;
        std::vector<glm::vec3> floor_norms;
        std::vector<glm::vec3> wall_verts;
        std::vector<glm::vec3> wall_norms;
        std::vector<glm::vec4> player_position;
       
        Collision();
        void init(std::string filename);
        bool on_floor(float x, float y);
        bool wall_hit_x(float x, float y);
        bool wall_hit_y(float x, float y);
        float distance(glm::vec2 p1, glm::vec2 p2);
        float player_distance(unsigned int id);
        bool player_collision(unsigned int id);
        void report(unsigned int id, float x, float y, float z, float angle);
        glm::vec4 get_enemy_pos(unsigned int id);
        bool in_center(unsigned int id);
};

#endif
