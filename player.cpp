#include "player.h"

float mat0_color[] = {0.0, 1.0, 1.0, 1.0};
float mat1_color[] = {1.0, 0.0, 0.0, 1.0};
float mat1_spec[] = {0.8, 0.8, 0.8, 1.0};
float mat1_shine[] = {30};

const char* player_vertex_source = R"glsl(
    #version 150 core

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)glsl";

const char* player_fragment_source = R"glsl(
    #version 150 core

    out vec4 outColor;

    void main()
    {
        outColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
)glsl";



Player::Player() {}

void Player::init(char* filename, bool player, Team team, float x, float y) {
    FILE *fp;     


    std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
    
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec2> temp_vt;
    std::vector<glm::vec3> temp_vn;

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
            uv_indices.push_back(uv_index[0]);
            uv_indices.push_back(uv_index[1]);
            uv_indices.push_back(uv_index[2]);
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
        this->vertices.push_back(v);

        /*glm::vec2 uv = temp_vt[uv_index-1];
        this->uvs.push_back(uv);*/

        glm::vec3 normal = temp_vn[normal_index-1];
        this->normals.push_back(normal);
    }


    // Generate model buffers
    glGenBuffers(1, &this->buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);

    /*glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);*/
    
    glGenBuffers(1, &this->nsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
 



    // Vertex Shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &player_vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Fragment Shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &player_fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Shader Program
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, vertex_shader);
    glAttachShader(this->shader_program, fragment_shader);

    if(player) {
        this->state = PLAYER;
    } else {
        this->state = WAIT;
    }

    this->team = team;
    this->x = x;
    this->y = y;
    this->z = CHAR_SIZE/2;
    this->angle = 0.0;
    this->moving = false;
    this->backing = false;
    this->jumping = false;
    this->rotating_left = false;
    this->rotating_right = false;
    this->has_flag = false;
    this->time_last_move = 0;
    this->time_last_back = 0;
    this->time_last_rot = 0;

}

void Player::draw() {
    //glTranslatef(this->x, 0.0, this->y);
    glScalef(1.0, 1.0, 1.0);
    //glRotatef(110, 0, 1, 0);

    glUseProgram(this->shader_program);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);*/

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);



    int cur_time = glutGet(GLUT_ELAPSED_TIME);

    bool changed = false;

    // Movement to position 
    glTranslatef(this->x, 0.0, this->y);
     

    // Rotation around itself
    glRotatef(this->angle, 0, 1, 0);
    
    // Make rotation transformation
    /*glm::mat4 rot; 
    rot = glm::rotate(
        rot,
        glm::radians(this->angle),
        glm::vec3(0.0, 1.0, 0.0)
    );

    // Apply rotation transformation
    GLint uniTrans = glGetUniformLocation(shader_program, "rot");
    glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(rot));
    */


    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat1_color);
    /*glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shine);*/

    //glutSolidCube(CHAR_SIZE-1); // Box.

    // Left Rotation
    if(this->rotating_left) {
        //std::cout << "Rotating left" << std::endl;
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle++;
            if(this->angle >= 360) {
                this->angle = 0;
            }
        }
        changed = true;
    }

    // Right Rotation
    if(this->rotating_right) {
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle--;
            if(this->angle <= 0) {
                this->angle = 360;
            }
        }
        changed = true;
    }

    // Movement
    if(this->moving && this->y < MAP_SIZE && this->y > 0 && this->x < MAP_SIZE && this->x > 0) {
        if(cur_time - this->time_last_move > CHAR_MOVE_FRAMERATE) {

            //std::cout << "Moving!" << std::endl;
            // Calculate new position based on angle
            this->x = this->x + sin(angle*PI/180)/sin(90*PI/180) * CHAR_MOVE_SPEED;
            this->y = this->y + sin((180.0-angle-90.0)*PI/180)/sin(90*PI/180) * CHAR_MOVE_SPEED;

            if(this->x > MAP_SIZE-CHAR_SIZE/2) {
                this->x = MAP_SIZE-CHAR_SIZE/2;
            }

            if(this->y > MAP_SIZE-CHAR_SIZE/2) {
                this->y = MAP_SIZE-CHAR_SIZE/2;
            }

            if(this->x < CHAR_SIZE/2) {
                this->x = CHAR_SIZE/2;
            }

            if(this->y < CHAR_SIZE/2) {
                this->y = CHAR_SIZE/2;
            }
        }
        changed = true;
    }
/*
    // Movement (backwards)
    if(this->backing
            && this->y < MAP_SIZE
            && this->y > 0
            && this->x < MAP_SIZE
            && this->x > 0) {
        if(cur_time - this->time_last_back > CHAR_BACK_FRAMERATE) {

            // Calculate new position based on angle
            this->x = this->x + sin(-angle*PI/180)/sin(90*PI/180);
            this->y = this->y + sin(-(180.0-angle-90.0)*PI/180)/sin(90*PI/180);

            if(this->x > MAP_SIZE-CHAR_SIZE/2) {
                this->x = MAP_SIZE-CHAR_SIZE/2;
            }

            if(this->y > MAP_SIZE-CHAR_SIZE/2) {
                this->y = MAP_SIZE-CHAR_SIZE/2;
            }

            if(this->x < CHAR_SIZE/2) {
                this->x = CHAR_SIZE/2;
            }

            if(this->y < CHAR_SIZE/2) {
                this->y = CHAR_SIZE/2;
            }
        }
        changed = true;
    }

    // Jump
    if(this->jumping && this->z == CHAR_SIZE/2) {
        this->jump_start = cur_time;
    }
*/

    glDisableVertexAttribArray(0);    
    glDisableVertexAttribArray(2);    


    if(changed) {
        if(cur_time - this->time_last_back > CHAR_BACK_FRAMERATE) {
            this->time_last_back = cur_time;
        }
        if(cur_time - this->time_last_move > CHAR_MOVE_FRAMERATE) {
            this->time_last_move = cur_time;
        }
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->time_last_rot = cur_time;
        }
        glutPostRedisplay();
    }


    
}

void Player::set_team(Team team) {
    this->team = team;
}

void Player::set_state(AiState state) {
    this->state = state;
}

void Player::set_pos(float x, float y) {
    this->x = x;
    this->y = y;
}

void Player::set_has_flag(bool val) {
    this->has_flag = val;
}

void Player::set_moving(bool val) {
    this->moving = val;
}

void Player::set_rotating_left(bool val) {
    this->rotating_left = val;
}

void Player::set_rotating_right(bool val) {
    this->rotating_right = val;    
}

void Player::set_backing(bool val) {
    this->backing = val;
}

void Player::set_jumping(bool val) {
    this->jumping = val;
}
