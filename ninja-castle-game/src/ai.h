//#include "common.h"
//#include "collision.h"
#include "character.h"

class AI: public Character {
    public:
/*    public:
        int id;
        Team team;
        float x;
        float y;
        float z;
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
        int jump_start;
        GLuint buffer;
        GLuint uvbuffer;
        GLuint nsbuffer;
        GLuint vao;
        GLint uni_model;
        GLuint shader_program;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;
        float vel_x;
        float vel_y;
        float vel_z;
        Collision collision_map;
        bool changed;

        AI();
        */
    /*public:
        AI():Character() {};
        void init(int id, char* filename, Team team, float x, float y, Collision collision_map);
        void set_team(Team team);
        void set_pos(float x, float y);
        void set_has_flag(bool val);
        void draw();
        void set_moving(bool val);
        void set_jumping(bool val);
        void set_backing(bool val);
        void set_rotating_left(bool val);
        void set_rotating_right(bool val);
        void respawn();*/
    void ai();
};

