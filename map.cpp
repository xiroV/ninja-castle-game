#include"loader/material.h"
#include"map.h"

float map_ambi[] = {0.3, 0.3, 0.3, 1.0};
float map_diff[] = {0.5, 0.5, 0.5, 1.0};
float map_spec[] = {0.0, 0.0, 0.0, 1.0};
float map_shin[] = {30};

Map::Map() {}

void Map::init(std::string filename) {
    std::ifstream inFile;

    std::vector<unsigned int> vertex_indices, normal_indices;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec3> temp_vn;
    std::string cur_mat;

    inFile.open(filename);
    if(!inFile) {
        std::cout << "Can't open map file " << filename << std::endl;
        exit(1);
    }
    
    std::string ch;
    while(inFile >> ch) {

        if(ch == "v") {
            glm::vec3 vertex;

            inFile >> vertex.x;
            inFile >> vertex.y;
            inFile >> vertex.z;

            temp_v.push_back(vertex);

        } else if(ch == "mtllib") {
            inFile >> ch;
            this->mtl.load(ch);
        } else if(ch == "usemtl") {
            inFile >> ch; 
            cur_mat = ch;
        } else if (ch == "vn") {
            glm::vec3 normal;

            inFile >> normal.x;
            inFile >> normal.y;
            inFile >> normal.z;

            temp_vn.push_back(normal);
        } else if (ch == "f") {
            unsigned int vertex_index[3], normal_index[3];
            //unsigned int uv_index[3];

            for(int i = 0; i < 3; i++) {
                inFile >> ch;

                int s1 = ch.find("/");
                vertex_index[i] = std::stoi(ch.substr(0, s1));

                std::string ch2 = ch.substr(s1+1, ch.length());

                int s2 = ch2.find("/");
                /*if(s2 > 0) {
                    uv_index[i] = std::stoi(ch.substr(s1, s2));
                }*/

                normal_index[i] = std::stoi(ch2.substr(s2+1, ch.length()));
            }

            vertex_indices.push_back(vertex_index[0]);
            vertex_indices.push_back(vertex_index[1]);
            vertex_indices.push_back(vertex_index[2]);
            normal_indices.push_back(normal_index[0]);
            normal_indices.push_back(normal_index[1]);
            normal_indices.push_back(normal_index[2]);
            colors.push_back(this->mtl.get(cur_mat).diffuse);
            colors.push_back(this->mtl.get(cur_mat).diffuse);
            colors.push_back(this->mtl.get(cur_mat).diffuse);
        }
    } // Done reading file



    inFile.close();

    // Now processing indices
    for(unsigned int i=0; i < vertex_indices.size(); i++) {
        unsigned int vertex_index = vertex_indices[i];
        unsigned int normal_index = normal_indices[i];

        glm::vec3 v = temp_v[vertex_index-1];
        this->vs.push_back(v);

        //glm::vec2 uv = temp_vt[uv_index-1];
        //this->uvs.push_back(uv);

        glm::vec3 normal = temp_vn[normal_index-1];
        this->ns.push_back(normal);
    }

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    
    /*glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
    */

    glGenBuffers(1, &this->nsbuffer);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->ns.size() * sizeof(glm::vec3), &this->ns[0], GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, 0);

    glGenBuffers(1, &this->color_buffer);
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
    glColorPointer(4, GL_FLOAT, 0, 0);

    std::cout << "Map initialized" << std::endl;
}

void Map::draw() {
    glTranslatef(0.0, 0.0, 0.0);
    glScalef(1.0,1.0,1.0);

    //glPointSize(5.0);

    /*glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);*/


    //glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);

    //glDrawArrays(GL_TRIANGLES, 0, this->vs.size());

    //glLinkProgram(shader_program);
    //glUseProgram(this->shader_program);

    //GLint posAttrib = glGetAttribLocation(this->shader_program, "position");


    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    

    /*glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);*/

    /*glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

    glBindVertexArray(this->vao);

    /*glMaterialfv(GL_FRONT, GL_AMBIENT, map_ambi);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, map_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, map_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, map_shin);*/

    glDrawArrays(GL_TRIANGLES, 0, this->vs.size());

    //glDisableVertexAttribArray(0);    
    //glDisableVertexAttribArray(1);    
    //glDisableVertexAttribArray(2);    

    //glutSwapBuffers();  

    //glBufferData(GL_ARRAY_BUFFER, this->vs.size() * sizeof(glm::vec3), &this->vs[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}


