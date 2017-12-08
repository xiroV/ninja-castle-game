#include"character.h"

Character::Character() {}

void Character::init(unsigned int id, char* filename, Team team, float x, float y, Collision* collision_map) {

    this->team = team;
    // Define colors/materials based on team
    /*if(this->team == Team::BLUE) {
        this->mat_ambi = glm::vec4(0.0, 0.0, 0.0, 0.0);
        this->mat_diff = glm::vec4(0.3, 0.3, 0.3, 1.0);
        this->mat_spec = glm::vec4(0.1, 0.1, 0.1, 1.0);
        this->mat_shin = 10.0;
    } else if(this->team == Team::RED) {
        this->mat_ambi = glm::vec4(0.0, 0.0, 0.0, 0.0);
        this->mat_diff = glm::vec4(0.8, 0.3, 0.3, 1.0);
        this->mat_spec = glm::vec4(0.1, 0.1, 0.1, 1.0);
        this->mat_shin = 10.0;
    }*/


    std::ifstream inFile;

    this->id = id;
    this->collision_map = collision_map;

    std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
    
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec2> temp_vt;
    std::vector<glm::vec3> temp_vn;
    std::string cur_mat;

    inFile.open(filename);
    if(!inFile) {
        std::cout << "Can't open character file" << filename << std::endl;
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
            unsigned int vertex_index[3], uv_index[3], normal_index[3];
            /*fscanf(fp, "%d//%d %d//%d %d//%d\n",
                    &vertex_index[0],
                    //&uv_index[0],
                    &normal_index[0],
                    &vertex_index[1],
                    //&uv_index[1],
                    &normal_index[1],
                    &vertex_index[2],
                    //&uv_index[2],
                    &normal_index[2]);

            if(matches != 9) {
                printf("Faces can't be read, try exporting with other options");
                exit(1);
            }

            vertex_indices.push_back(vertex_index[0]);
            vertex_indices.push_back(vertex_index[1]);
            vertex_indices.push_back(vertex_index[2]);
            uv_indices.push_back(uv_index[0]);
            uv_indices.push_back(uv_index[1]);
            uv_indices.push_back(uv_index[2]);
            normal_indices.push_back(normal_index[0]);
            normal_indices.push_back(normal_index[1]);
            normal_indices.push_back(normal_index[2]);*/

            for(int i = 0; i < 3; i++) {
                inFile >> ch;

                int s1 = ch.find("/");
                vertex_index[i] = std::stoi(ch.substr(0, s1));

                std::string ch2 = ch.substr(s1+1, ch.length());

                int s2 = ch2.find("/");
                if(s2 > 0) {
                    uv_index[i] = std::stoi(ch.substr(s1, s2));
                }

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
    glGenVertexArrays(1, &this->vao);

    // BEGIN bind to vao
    glBindVertexArray(this->vao);

    // Generate model buffers
    glGenBuffers(1, &this->buffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    /*glGenBuffers(1, &this->uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);*/
    
    glGenBuffers(1, &this->nsbuffer);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->nsbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
    glNormalPointer(GL_FLOAT, 0, 0);

    glGenBuffers(1, &this->color_buffer);
    glEnableClientState(GL_COLOR_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, this->color_buffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
    glColorPointer(4, GL_FLOAT, 0, 0);


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
    this->time_last_frame = 0;
    this->time_last_vel_x = 0;
    this->time_last_vel_y = 0;
    this->vel_x = 0.0;
    this->vel_y = 0.0;
    this->vel_z = 0.0;
    this->changed = true;
    this->dead = false;
    this->dead_time = 0;
    this->time_in_center = 0.0;

    this->respawn();
}

void Character::draw() {
    this->changed = false;
    float cur_time = glutGet(GLUT_ELAPSED_TIME);
    glBindVertexArray(this->vao);
    

    float gravity = 0.05;

    // Movement to position 
    glTranslatef(this->x, this->z, this->y);

    // Rotation around itself
    glRotatef(this->angle, 0, 1, 0);
    
    /*glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(this->mat_ambi));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(this->mat_diff));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(this->mat_spec));
    glMaterialfv(GL_FRONT, GL_SHININESS, &this->mat_shin);
    */
    glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());


    if(this->dead && cur_time - this->dead_time > 5000) {
        std::cout << cur_time - this->dead_time << std::endl;
        this->dead = false;
        this->respawn();
        this->changed = true;
    }

    // If not on floor
    if(!this->collision_map->on_floor(this->x, this->y)) {
        if(cur_time - this->time_last_jump > 20) {
            if(this->z > -15) {
                this->vel_z -= gravity;
                this->z += this->vel_z;
            }
            this->time_last_jump = cur_time;
        }
        this->changed = true;
    }

    // Calculate gravity on jump
    if(this->jumping || this->z > 0.0) {
        if(cur_time - this->time_last_jump > 20) {
            if(!this->changed) {
                this->vel_z -= gravity;
                this->z += this->vel_z;
            }
            this->time_last_jump = cur_time;
        }
        this->changed = true;
    }

    // Calculate velocity x
    if(this->vel_x != 0 && this->z > 0) {
        if(cur_time - this->time_last_vel_x > 20) {
            this->vel_x -= gravity;
            if(!this->collision_map->wall_hit_x(this->x + this->vel_x, this->y)) {
                this->x += this->vel_x;
            }
            this->time_last_vel_x = cur_time;
        }
        this->changed = true;
    }

    // Calculate velocity y
    if(this->vel_y != 0 && this->z > 0) {
        if(cur_time - this->time_last_vel_y > 20) {
            this->vel_y -= gravity;
            if(!this->collision_map->wall_hit_y(this->x, this->y + this->vel_y)) {
                this->y += this->vel_y;
            }
            this->time_last_vel_y = cur_time;
        }
        this->changed = true;
    }


    if(this->z <= 0.0 && this->collision_map->on_floor(this->x, this->y)) {
        this->z = 0.0;
        this->vel_x = 0.0;
        this->vel_y = 0.0;
    }

    // Set death
    if(this->z < -14 && !this->dead) {
        this->dead = true;
        this->dead_time = cur_time;
    }

    // Left Rotation
    if(this->rotating_left && !this->dead) {
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle += CHAR_ROT_SPEED;
            if(this->angle >= 360) {
                this->angle = 0;
            }
            this->time_last_rot = cur_time;
        }
        this->changed = true;
    }

    // Right Rotation
    if(this->rotating_right && !this->dead) {
        if(cur_time - this->time_last_rot > CHAR_ROT_FRAMERATE) {
            this->angle -= CHAR_ROT_SPEED;
            if(this->angle <= 0) {
                this->angle = 360;
            }
            this->time_last_rot = cur_time;
        }
        this->changed = true;
    }


    // Movement
    if(this->moving && this->z >= -4.0) {
        if(cur_time - this->time_last_move > CHAR_MOVE_FRAMERATE) {

            // Calculate new position based on angle
            float new_x = this->x + (sin(this->angle*PI/180) * CHAR_MOVE_SPEED);
            float new_y = this->y + (cos(this->angle*PI/180) * CHAR_MOVE_SPEED);

            float ray_x = this->x + (sin(this->angle*PI/180) * CHAR_MOVE_SPEED*2);
            float ray_y = this->y + (cos(this->angle*PI/180) * CHAR_MOVE_SPEED*2);

            if(this->id == 0) {
                float left_ray_angle = this->angle + 90;
                float right_ray_angle = this->angle - 90;

                if(left_ray_angle < 0) {
                    left_ray_angle += 360;
                }

                if(right_ray_angle > 360) {
                    left_ray_angle -= 360;
                }

                float ray_x = this->x + (sin(this->angle*PI/180)*5);
                float ray_y = this->y + (cos(this->angle*PI/180)*5);
                float left_ray_x = this->x + (sin(left_ray_angle*PI/180)*5);
                float left_ray_y = this->y + (cos(left_ray_angle*PI/180)*5);
                float right_ray_x = this->x + (sin(right_ray_angle*PI/180)*5);
                float right_ray_y = this->y + (cos(right_ray_angle*PI/180)*5);
            }

            if(this->collision_map->wall_hit_x(ray_x, ray_y)) {
                new_x = this->x;
            }

            if(this->collision_map->wall_hit_y(ray_x, ray_y)) {
                new_y = this->y;
            }

            this->x = new_x;
            this->y = new_y;

            this->time_last_move = cur_time;
        }
        this->changed = true;
    }

    glBindVertexArray(0);
    this->collision_map->report(this->id, this->x, this->y, this->z, this->angle);

    if(this->collision_map->player_collision(this->id)) {
        glm::vec4 enemy_pos = this->collision_map->get_enemy_pos(this->id);
        float test_pos_x = enemy_pos.x + (sin(enemy_pos.w*PI/180));
        float test_pos_y = enemy_pos.y + (cos(enemy_pos.w*PI/180));

        this->vel_x = test_pos_x-enemy_pos.x;
        this->vel_y = test_pos_y-enemy_pos.y;

        this->vel_z = 0.5;    
        this->jumping = true;
        this->time_last_jump = cur_time;
    }

    if(this->collision_map->in_center(this->id)) {
        float frame_end_time = glutGet(GLUT_ELAPSED_TIME);
        this->time_in_center += frame_end_time - cur_time;
    }
}

void Character::set_team(Team team) {
    this->team = team;
}

void Character::set_pos(float x, float y) {
    this->x = x;
    this->y = y;
}

void Character::set_has_flag(bool val) {
    this->has_flag = val;
}

void Character::set_moving(bool val) {
    this->moving = val;
}

void Character::set_rotating_left(bool val) {
    this->rotating_left = val;
}

void Character::set_rotating_right(bool val) {
    this->rotating_right = val;    
}

void Character::set_backing(bool val) {
    this->backing = val;
}

void Character::set_jumping(bool val) {
    if(val) {
        this->jumping = true;
        if(this->z == 0) {
            this->vel_z = 0.6;
        }
    } else {
        this->jumping = false;
    }
}

void Character::respawn() {
    if(this->team == Team::RED) {
        this->x = 22.5;
        this->y = 56.0;
        this->z = 0.0;
        this->angle = 180.0;
    } else if(this->team == Team::BLUE) {
        this->x = 22.5;
        this->y = 7.0;
        this->z = 0.0;
        this->angle = 0.0;
    }
}
