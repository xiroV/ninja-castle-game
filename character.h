#include "common.h"
#include "core.h"
#include "loader/material.h"

#ifndef __character_h_included__
#define __character_h_included__

class Character {
    public:
        unsigned int id;
        MaterialLoader mtl;
        Team team;
        float x;
        float y;
        float z;
        glm::vec4 mat_ambi;
        glm::vec4 mat_diff;
        glm::vec4 mat_spec;
        float mat_shin;
        float angle;
        bool moving;
        bool backing;
        bool jumping;
        bool rotating_left;
        bool rotating_right;
        bool has_flag;
        int time_last_move;
        int time_last_back;
        int time_last_rot;
        int time_last_jump;
        int time_last_frame;
        int time_last_vel_x;
        int time_last_vel_y;
        int jump_start;
        GLuint buffer;
        GLuint color_buffer;
        GLuint nsbuffer;
        GLint uni_model;
        GLuint vao;
        GLuint shader_program;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        float vel_x;
        float vel_y;
        float vel_z;
        Collision* collision_map;
        bool changed;
        bool dead;
        int dead_time;
        float time_in_center;

        Character();
        void init(unsigned int id, char* filename, Team team, float x, float y, Collision* collision_map);
        void set_team(Team team);
        void set_pos(float x, float y);
        void set_has_flag(bool val);
        void draw();
        void set_moving(bool val);
        void set_jumping(bool val);
        void set_backing(bool val);
        void set_rotating_left(bool val);
        void set_rotating_right(bool val);
        void respawn();
};

#endif
