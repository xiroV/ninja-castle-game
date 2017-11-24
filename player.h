#include "common.h"

enum Team {
    RED,
    BLUE
};

enum AiState {
    WAIT,
    KILL_EFC,
    CAP_FLAG,
    RET_FLAG,
    KILL_ANY,
    PLAYER,
};

class Player {
    public:
        AiState state;
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
        int jump_start;
        GLuint buffer;
        GLuint uvbuffer;
        GLuint nsbuffer;
        GLuint shader_program;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> normals;

        Player();
        void init(char* filename, bool player, Team team, float x, float y);
        void set_team(Team team);
        void set_state(AiState state);
        void set_pos(float x, float y);
        void set_has_flag(bool val);
        void draw();
        void set_moving(bool val);
        void set_jumping(bool val);
        void set_backing(bool val);
        void set_rotating_left(bool val);
        void set_rotating_right(bool val);
};

