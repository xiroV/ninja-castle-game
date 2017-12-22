#include"core.h"

Collision::Collision() {}

void Collision::init(std::string filename) {
    std::ifstream inFile;

    this->center_point = glm::vec2(22.5, 31.0);
    this->player_position.push_back(glm::vec4(0, 0, 0, 0));
    this->player_position.push_back(glm::vec4(0, 0, 0, 0));

    std::vector<unsigned int> vertex_indices, normal_indices;
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec3> temp_vn;
    bool read = true;

    inFile.open(filename);
    if(!inFile) {
        std::cout << "Can't open collision file " << filename << std::endl;
        exit(1);
    }
    
    // Read floor
    std::string ch;
    while(inFile >> ch && read) {

        if(ch == "v") {
            glm::vec3 vertex;

            inFile >> vertex.x;
            inFile >> vertex.y;
            inFile >> vertex.z;

            temp_v.push_back(vertex);
        } else if (ch == "o") {
            inFile >> ch;
            int s1 = ch.find("_");
            if(ch.substr(0, s1) != "floor") {
                read = false;
            }
        } else if (ch == "vn") {
            glm::vec3 normal;

            inFile >> normal.x;
            inFile >> normal.y;
            inFile >> normal.z;

            temp_vn.push_back(normal);
        } else if (ch == "f") {
            unsigned int vertex_index[3], normal_index[3];
            //unsigned int uv_index[3]

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
        }
    } // Done reading floor

    // Now processing floor indices
    for(unsigned int i=0; i < vertex_indices.size(); i++) {
        unsigned int vertex_index = vertex_indices[i];
        unsigned int normal_index = normal_indices[i];

        glm::vec3 v = temp_v[vertex_index-1];
        this->floor_verts.push_back(v);

        glm::vec3 normal = temp_vn[normal_index-1];
        this->floor_norms.push_back(normal);
    }

    inFile.close();

    std::cout << "Collision map intialized" << std::endl;
}

bool Collision::on_floor(float x, float y) {
    for(unsigned int i = 0; i < this->floor_verts.size(); i+=3) {
        glm::vec2 p1, p2, p3;
        p1.x = this->floor_verts[i].x;
        p1.y = this->floor_verts[i].z;
        p2.x = this->floor_verts[i+1].x;
        p2.y = this->floor_verts[i+1].z;
        p3.x = this->floor_verts[i+2].x;
        p3.y = this->floor_verts[i+2].z;

        // Barycentric coordinates
        float alpha = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
        float beta = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
        float gamma = 1.0f - alpha - beta;

        if(alpha > 0.0 && beta > 0.0 && gamma > 0.0) {
            return true;
        }
    }
    return false;
}

bool Collision::wall_hit_x(float x, float y) {
    if(x > 10 && x < 34 && y > 18 && y < 45) {
        return false;
    }
    return !this->on_floor(x, y);
}

bool Collision::wall_hit_y(float x, float y) {
    if(x > 10 && x < 34 && y > 18 && y < 45) {
        return false;
    }
    return !this->on_floor(x, y);
}

float Collision::distance(glm::vec2 p1, glm::vec2 p2) {
    float distance = sqrt(
        pow(p1.x-p2.x, 2) +
        pow(p1.y-p2.y, 2)
    );

    return distance;
}

float Collision::player_distance(unsigned int id) {
    glm::vec4 player = this->player_position[id];
    glm::vec4 enemy = this->get_enemy_pos(id);

    // Calculate enemy ray
    float ray_x = enemy.x + (sin(enemy.w*PI/180) * CHAR_MOVE_SPEED);
    float ray_y = enemy.y + (cos(enemy.w*PI/180) * CHAR_MOVE_SPEED);

    float distance = sqrt(
        pow(player.x-ray_x, 2) +
        pow(player.y-ray_y, 2) +
        pow(player.z-enemy.z, 2)
    );

    return distance;
}

bool Collision::player_collision(unsigned int player) {
    // Set to true if the player collided with enemy
    // Used to set velocities on knock-back for character
    
    if(this->player_distance(player) < 1.0) {
        return true;
    }

    return false;
}

// Method for players to report their position
void Collision::report(unsigned int id, float x, float y, float z, float angle) {
    this->player_position[id].x = x; 
    this->player_position[id].y = y; 
    this->player_position[id].z = z; 
    this->player_position[id].w = angle; 
}

glm::vec4 Collision::get_enemy_pos(unsigned int player) {
    unsigned int enemy;
    if(player == 0) {
        enemy = 1;
    } else {
        enemy = 0;
    }

    return this->player_position[enemy];
}

bool Collision::in_center(unsigned int id) {
    float distance = this->distance(glm::vec2(this->player_position[id].x, this->player_position[id].y), this->center_point);
    if(distance < 8.5) {
        return true;
    }
    return false;
}
