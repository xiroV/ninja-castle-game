#include"collision.h"

Collision::Collision() {}

void Collision::init(std::string filename) {
    std::ifstream inFile;

    std::vector<unsigned int> vertex_indices, normal_indices;
    std::vector<glm::vec3> temp_v;
    std::vector<glm::vec3> temp_vn;
    bool read = true;

    std::cout << "Reading collision map" << std::endl;

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
            unsigned int vertex_index[3], uv_index[3], normal_index[3];

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

    read = true; 
    vertex_indices.clear();
    normal_indices.clear();
    temp_v.clear();
    temp_vn.clear();

    // Read walls
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
            if(ch.substr(0, s1) != "walls") {
                read = false;
            }
        } else if (ch == "vn") {
            glm::vec3 normal;

            inFile >> normal.x;
            inFile >> normal.y;
            inFile >> normal.z;

            temp_vn.push_back(normal);
        } else if (ch == "f") {
            unsigned int vertex_index[3], uv_index[3], normal_index[3];

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
        }
    } // Done reading floor

    inFile.close();

    // Now processing wall indices
    for(unsigned int i=0; i < vertex_indices.size(); i++) {
        unsigned int vertex_index = vertex_indices[i];
        unsigned int normal_index = normal_indices[i];

        glm::vec3 v = temp_v[vertex_index-1];
        this->wall_verts.push_back(v);

        glm::vec3 normal = temp_vn[normal_index-1];
        this->wall_norms.push_back(normal);
    }

    std::cout << "Done reading floor" << std::endl;
    std::cout << "Read " << this->floor_verts.size() << " vertices" << std::endl;
    std::cout << "Done reading walls" << std::endl;
    std::cout << "Read " << this->wall_verts.size() << " vertices" << std::endl;

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

        /*std::cout << "p1: " << p1.x << " " << p1.y << std::endl;
        std::cout << "p2: " << p2.x << " " << p2.y << std::endl;
        std::cout << "p3: " << p3.x << " " << p3.y << std::endl;
        std::cout << x << " " << y << std::endl;*/

        // Barycentric coordinates
        float alpha = ((p2.y - p3.y) * (x - p3.x) + (p3.x - p2.x) * (y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
        float beta = ((p3.y - p1.y) * (x - p3.x) + (p1.x - p3.x) * (y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
        float gamma = 1.0f - alpha - beta;
        //std::cout << alpha << " " << beta << " " << gamma << std::endl;

        if(alpha > 0.0 && beta > 0.0 && gamma > 0.0) {
            return true;
        }
    }
    return false;
}

bool Collision::wall_hit_x(float x, float y) {
    /*if(this->on_floor(x, y)) {
        return false;
    } else {
        if(x < 7) {
            return true;
        } else if(x > 36) {
            return true;
        }
    }
    return false;*/

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
    //}
    //    return false;
    /*if(this->on_floor(x, y)) {
        return false;
    } else {
        if(y < 15) {
            return true;
        } else if(y > 45) {
            return true;
        }
    }*/
}
