#include "ai.h"

float ai_ambi[] = {0.0, 0.0, 0.0, 0.0};
float ai_diff[] = {0.8, 0.3, 0.3, 1.0};
float ai_spec[] = {0.1, 0.1, 0.1, 1.0};
float ai_shin[] = {10};

/*const char* player_vertex_source = R"glsl(
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
        outColor = vec4(0.0, 0.0, 1.0, 1.0);
    }
)glsl";*/


AI::AI() {}

void AI::init(char* filename, Team team, float x, float y, Collision collision_map) {
    FILE *fp;

    this->collision_map = collision_map;

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
        } else if (strcmp(ch, "vt") == 0) {
            glm::vec2 uv;
            fscanf(fp, "%f %f\n", &uv.x, &uv.y);
            temp_vt.push_back(uv);
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
        this->vertices.push_back(v);

        /*glm::vec2 uv = temp_vt[uv_index-1];
        this->uvs.push_back(uv);*/

        glm::vec3 normal = temp_vn[normal_index-1];
        this->normals.push_back(normal);
    }


    // Generate vao
    std::cout << "Generating vao" << std::endl;
    glGenVertexArrays(1, &this->vao);

    std::cout << "vao generated: " << this->vao << std::endl;

    // BEGIN bind to vao
    glBindVertexArray(this->vao);

    // Generate model buffers
    std::cout << "Generating vbo" << std::endl;
    glGenBuffers(1, &this->buffer);
    std::cout << "vbo generated: " << this->buffer << std::endl;

    std::cout << "Enabling client state" << std::endl;
    glEnableClientState(GL_VERTEX_ARRAY);

    std::cout << "Binding to buffer" << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

    std::cout << "Inserting data" << std::endl << this->vertices.size()*sizeof(glm::vec3) << std::endl;

    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0]);
    
    std::cout << "Vertex pointer" << std::endl;
    glVertexPointer(3, GL_FLOAT, 0, 0);

    //std::cout << "player_buffer=" << this->buffer << std::endl;

    glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &this->nsbuffer);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);

    glNormalPointer(GL_FLOAT, 0, 0);

    // Vertex Shader
    /*GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &player_vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Fragment Shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &player_fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Shader Program
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, vertex_shader);
    glAttachShader(this->shader_program, fragment_shader);*/
    //glBindFragDataLocation(this->shader_program, 0, "outColor");
    //glLinkProgram(this->shader_program);
    //glUseProgram(this->shader_program);

    /*glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

    /*glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);*/

    /*glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);*/




    this->team = team;
    this->x = x;
    this->y = y;
    this->z = 0;
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
    this->time_last_jump = 0;
    this->vel_x = 0.0;
    this->vel_y = 0.0;
    this->vel_z = 0.0;
    this->changed = true;

    std::cout << "AI initialized" << std::endl;
}

void AI::draw() {

   /* glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glVertexPointer(3, GL_FLOAT, 0, NULL); 
    glVertexPointer(3, GL_FLOAT, sizeof(glm::vec3), &this->buffer); */
    glBindVertexArray(this->vao);

    int cur_time = glutGet(GLUT_ELAPSED_TIME);
    this->changed = false;

    float gravity = 0.02;

    // If not on floor
    if(!this->collision_map.on_floor(this->x, this->y)) {
        if(cur_time - this->time_last_jump > 20) {
            if(this->z > -15) {
                this->vel_z -= gravity;
                this->z += vel_z;
            }
        }
        this->changed = true;
    }

    // Calculate gravity 
    if(this->jumping || this->z > 0.0) {
        if(cur_time - this->time_last_jump > 20) {
            if(!this->changed) {
                this->vel_z -= gravity;
                this->z += vel_z;
            }
        }
        this->changed = true;
    }

    if(this->z <= 0.0 && this->collision_map.on_floor(this->x, this->y)) {
        this->z = 0.0;
    }

    // Respawn if dead
    if(this->z < -14) {
        this->respawn();
    }

    // Movement to position 
    glTranslatef(this->x, this->z, this->y);

    // Rotation around itself
    glRotatef(this->angle, 0, 1, 0);
    
    // Make rotation transformation
    /*glm::mat4 model;
    model = glm::rotate(
        model,
        glm::radians(this->angle),
        glm::vec3(0.0, 1.0, 0.0)
    );*/

    // Apply rotation transformation
    //glUniformMatrix4fv(this->uni_model, 1, GL_FALSE, glm::value_ptr(model));

    glMaterialfv(GL_FRONT, GL_AMBIENT, ai_ambi);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ai_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ai_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, ai_shin);

    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

    /*glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shine);*/

    // Left Rotation
    if(this->rotating_left) {
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle += CHAR_ROT_SPEED;
            if(this->angle >= 360) {
                this->angle = 0;
            }
        }
        this->changed = true;
    }

    // Right Rotation
    if(this->rotating_right) {
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle -= CHAR_ROT_SPEED;
            if(this->angle <= 0) {
                this->angle = 360;
            }
        }
        this->changed = true;
    }

    // Movement
    //if(this->moving && this->y < MAP_SIZE_Y && this->y > 0 && this->x < MAP_SIZE_X && this->x > 0) {
    if(this->moving && this->z >= -1.0) {
        if(cur_time - this->time_last_move > CHAR_MOVE_FRAMERATE) {

            // Calculate new position based on angle
            /*float new_x = this->x + sin(angle*PI/180)/sin(90*PI/180) * CHAR_MOVE_SPEED;
            float new_y = this->y + sin((180.0-angle-90.0)*PI/180)/sin(90*PI/180) * CHAR_MOVE_SPEED;*/
            float new_x = this->x + (sin(this->angle*PI/180) * CHAR_MOVE_SPEED);
            float new_y = this->y + (cos(this->angle*PI/180) * CHAR_MOVE_SPEED);

            float ray_x = this->x + (sin(this->angle*PI/180) * CHAR_MOVE_SPEED*2);
            float ray_y = this->y + (cos(this->angle*PI/180) * CHAR_MOVE_SPEED*2);


            //float ray_x = this->x + sin(angle*PI/180)/sin(90*PI/180);
            //float ray_y = this->y + sin((180.0-angle-90.0)*PI/180)/sin(90*PI/180);

            std::cout << "Position: " << this->x << " " << this->y << " " << this->z << std::endl;
            std::cout << "Velocity: " << this->vel_z << std::endl;
            //std::cout << "Ray: " << ray_x << " " << ray_y << std::endl; 

            if(this->collision_map.on_floor(this->x, this->y)) {
                std::cout << "PLAYER_ON_FLOOR" << std::endl;
            }

            if(this->collision_map.wall_hit_x(ray_x, ray_y)) {
                //std::cout << "Collision x" << std::endl;
                new_x = this->x;
            }

            if(this->collision_map.wall_hit_y(ray_x, ray_y)) {
                //std::cout << "Collision y" << std::endl;
                new_y = this->y;
            }

            this->x = new_x;
            this->y = new_y;

            /*if(this->x < CHAR_SIZE/2+4) {
                this->x = CHAR_SIZE/2+4;
            }

            if(this->y < CHAR_SIZE/2+2) {
                this->y = CHAR_SIZE/2+2;
            }*/
        }
        this->changed = true;
    }

    // Movement (backwards)
    /*if(this->backing
            && this->y < MAP_SIZE_Y
            && this->y > 0
            && this->x < MAP_SIZE_X
            && this->x > 0) {
        if(cur_time - this->time_last_back > CHAR_BACK_FRAMERATE) {

            // Calculate new position based on angle
            this->x = this->x + sin(-angle*PI/180)/sin(90*PI/180) * (CHAR_MOVE_SPEED/2.5);
            this->y = this->y + sin(-(180.0-angle-90.0)*PI/180)/sin(90*PI/180) * (CHAR_MOVE_SPEED/2.5);

            if(this->x > MAP_SIZE_X-CHAR_SIZE/2) {
                this->x = MAP_SIZE_X-CHAR_SIZE/2;
            }

            if(this->y > MAP_SIZE_Y-CHAR_SIZE/2) {
                this->y = MAP_SIZE_Y-CHAR_SIZE/2;
            }

            if(this->x < CHAR_SIZE/2+4) {
                this->x = CHAR_SIZE/2+4;
            }

            if(this->y < CHAR_SIZE/2+2) {
                this->y = CHAR_SIZE/2+2;
            }
        }
        this->changed = true;
    }*/

    
    if(this->changed) {
        if(cur_time - this->time_last_back > CHAR_BACK_FRAMERATE) {
            this->time_last_back = cur_time;
        }
        if(cur_time - this->time_last_move > CHAR_MOVE_FRAMERATE) {
            this->time_last_move = cur_time;
        }
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->time_last_rot = cur_time;
        }
        if(cur_time - this->time_last_jump > 20) {
            this->time_last_jump = cur_time;
        }
        //glutPostRedisplay();
    }

    glBindVertexArray(0);
    /*glDisableVertexAttribArray(0);    
    glDisableVertexAttribArray(2);*/

    std::cout << "AI drawn: " << this->x << " " << this->y << " " << this->z << std::endl;
}

void AI::set_team(Team team) {
    this->team = team;
}

void AI::set_pos(float x, float y) {
    this->x = x;
    this->y = y;
}

void AI::set_has_flag(bool val) {
    this->has_flag = val;
}

void AI::set_moving(bool val) {
    this->moving = val;
}

void AI::set_rotating_left(bool val) {
    this->rotating_left = val;
}

void AI::set_rotating_right(bool val) {
    this->rotating_right = val;    
}

void AI::set_backing(bool val) {
    this->backing = val;
}

void AI::set_jumping(bool val) {
    if(val) {
        this->jumping = true;
        if(this->z == 0) {
            this->vel_z = 0.5;
        }
    } else {
        this->jumping = false;
        this->vel_z = 0.0;   
    }
}

void AI::respawn() {
    this->x = 22.5;
    this->y = 8.0;
    this->z = 0.0;

}
