#include"map.h"

float map_color[] = {0.8, 0.8, 0.8, 1.0};
/*float mat1_color[] = {1.0, 0.0, 0.0, 1.0};
float mat1_spec[] = {0.8, 0.8, 0.8, 1.0};
float mat1_shine[] = {30};*/

//GLuint vao;

const char* vertex_source = R"glsl(
    #version 150 core

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)glsl";

const char* fragment_source = R"glsl(
    #version 150 core

    out vec4 outColor;

    void main()
    {
        outColor = vec4(0.5, 0.5, 0.5, 1.0);
    }
)glsl";


Map::Map() {}

void Map::init(char* filename) {
    FILE *fp;     


    std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
    
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec2> temp_vt;
    std::vector<glm::vec3> temp_vn;



    //GLfloat x,y,z;


    //map = glGenLists(1);
    //
    std::cout << "Reading map" << std::endl;

    fp = fopen(filename, "r");
    if(!fp) {
        printf("Can't open map file %s\n", filename);
        exit(1);
    }
    

    while(!(feof(fp))) {
        char ch[128];

        fscanf(fp, "%s ", ch);

        if(strcmp(ch, "v") == 0) {
            glm::vec3 vertex;
            fscanf(fp, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_v.push_back(vertex);
        /*} else if (strcmp(ch, "vt") == 0) {
            std::cout << "lla" <<std::endl;
            glm::vec2 uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            temp_vt.push_back(uv);*/
        } else if (strcmp(ch, "vn") == 0) {
            glm::vec3 normal;
            fscanf(fp, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_vn.push_back(normal);
        } else if (strcmp(ch, "f") == 0) {
            unsigned int vertex_index[3], uv_index[3], normal_index[3];
            fscanf(fp, "%d//%d %d//%d %d//%d\n",
                    &vertex_index[0],
                    //&uv_index[0],
                    &normal_index[0],
                    &vertex_index[1],
                    //&uv_index[1],
                    &normal_index[1],
                    &vertex_index[2],
                    //&uv_index[2],
                    &normal_index[2]);

            /*if(matches != 9) {
                printf("Faces can't be read, try exporting with other options");
                exit(1);
            }*/

            vertex_indices.push_back(vertex_index[0]);
            vertex_indices.push_back(vertex_index[1]);
            vertex_indices.push_back(vertex_index[2]);
            /*uv_indices.push_back(uv_index[0]);
            uv_indices.push_back(uv_index[1]);
            uv_indices.push_back(uv_index[2]);*/
            normal_indices.push_back(normal_index[0]);
            normal_indices.push_back(normal_index[1]);
            normal_indices.push_back(normal_index[2]);
        }
    } // Done reading file


    fclose(fp);

    // Now processing indices
    for(unsigned int i=0; i < vertex_indices.size(); i++) {
        unsigned int vertex_index = vertex_indices[i];
        //unsigned int uv_index = uv_indices[i];
        unsigned int normal_index = normal_indices[i];

        glm::vec3 v = temp_v[vertex_index-1];
        this->vs.push_back(v);

        //glm::vec2 uv = temp_vt[uv_index-1];
        //this->uvs.push_back(uv);

        glm::vec3 normal = temp_vn[normal_index-1];
        this->ns.push_back(normal);
    }


    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);
    
    //GLuint this->uvbuffer;
    glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &this->nsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->ns.size() * sizeof(glm::vec3), &this->ns[0], GL_STATIC_DRAW);
 

    // Vertex Shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Fragment Shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);
    glCompileShader(fragment_shader);
    
    // Shader Program
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, vertex_shader);
    glAttachShader(this->shader_program, fragment_shader);
}

void Map::draw() {
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(1.0,1.0,1.0);

    //glPointSize(5.0);

    //glGenVertexArrays(1, &vao);

    //glBindVertexArray(vao);


    //glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);

    //glDrawArrays(GL_TRIANGLES, 0, this->vs.size());

    //glLinkProgram(shader_program);
    glUseProgram(this->shader_program);

    //GLint posAttrib = glGetAttribLocation(this->shader_program, "position");


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);*/

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, this->vs.size());

    glDisableVertexAttribArray(0);    
    //glDisableVertexAttribArray(1);    
    glDisableVertexAttribArray(2);    

    //glutSwapBuffers();  

    //glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);
}


