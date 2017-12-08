#include "common.h"
#include "loader/material.h"

class Map {
    public:
        std::vector<glm::vec3> vs;
        std::vector<glm::vec2> uvs;
        std::vector<glm::vec3> ns;
        MaterialLoader mtl;
        GLuint vao;
        GLuint vbo;
        GLuint color_buffer;
        GLuint nsbuffer;
        GLuint shader_program;

        Map();
        void init(std::string filename);
        void draw();
};

