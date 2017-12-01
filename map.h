#include "common.h"

class Map {
    public:
        //GLuint map;
        
        std::vector<glm::vec3> vs;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> ns;
        GLuint vbo;
        GLuint uvbuffer;
        GLuint nsbuffer;
        GLuint shader_program;


        Map();
        void init(std::string filename);
        void draw();
};

