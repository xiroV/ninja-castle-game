#include"character.h"
#include"ai.h"

void AI::ai() {
    float left_ray_angle = this->angle + 90;
    float right_ray_angle = this->angle - 90;
    float back_ray_angle = this->angle - 180;

    if(left_ray_angle > 360) {
        left_ray_angle -= 360;
    }

    if(right_ray_angle < 0) {
        right_ray_angle += 360;
    }

    if(back_ray_angle < 0) {
        back_ray_angle += 360;
    }

    glm::vec2 target = this->collision_map->center_point;
    glm::vec2 self_pos = glm::vec2(this->x, this->y);
    glm::vec4 full_enemy_pos = this->collision_map->get_enemy_pos(this->id);
    glm::vec2 enemy_pos = glm::vec2(full_enemy_pos.x, full_enemy_pos.y);

    if(this->collision_map->distance(self_pos, enemy_pos) < 10) {
        target = enemy_pos;
    }

    // Find ray positions
    glm::vec2 front_ray = glm::vec2(this->x + (sin(this->angle*PI/180)),this->y + (cos(this->angle*PI/180)));
    glm::vec2 left_ray = glm::vec2(this->x + (sin(left_ray_angle*PI/180)),  this->y + (cos(left_ray_angle*PI/180)));
    glm::vec2 right_ray = glm::vec2(this->x + (sin(right_ray_angle*PI/180)), this->y + (cos(right_ray_angle*PI/180)));
    glm::vec2 back_ray = glm::vec2(this->x + (sin(back_ray_angle*PI/180)), this->y + (cos(back_ray_angle*PI/180)));

    // Check nearby environment
    bool front_on_floor = this->collision_map->on_floor(front_ray.x, front_ray.y);
    bool left_on_floor = this->collision_map->on_floor(left_ray.x, left_ray.y);
    bool right_on_floor = this->collision_map->on_floor(right_ray.x, right_ray.y);

    // Calculate distances
    float dist_self_goal = this->collision_map->distance(self_pos, target);
    float dist_front_goal = this->collision_map->distance(front_ray, target);
    float dist_left_goal = this->collision_map->distance(left_ray, target);
    float dist_right_goal = this->collision_map->distance(right_ray, target);
    float dist_back_goal = this->collision_map->distance(back_ray, target);

    if(front_on_floor) {
        this->set_moving(true);  
    }

    if(dist_front_goal < dist_left_goal) {
        this->set_rotating_left(false);
    }
    if(dist_front_goal < dist_right_goal) {
        this->set_rotating_right(false);
    }



    if(!front_on_floor) {
        this->set_moving(false);
        if(dist_left_goal > dist_right_goal) {
            this->set_rotating_left(true);
        } else {
            this->set_rotating_right(true);
        }
    }

    if(this->moving) {
        if(dist_left_goal < dist_self_goal && left_on_floor) {
            this->set_rotating_left(true);
        }
        if(dist_right_goal < dist_self_goal && right_on_floor) {
            this->set_rotating_right(true);
        }
        if(dist_back_goal < dist_self_goal && right_on_floor) {
            this->set_rotating_right(true);
        }
    }

    


    /*if(dist_front_goal > dist_left_goal || dist_front_goal > dist_right_goal) {
        if(dist_left_goal < dist_right_goal) {
            this->set_rotating_left(true);
        } else {
            this->set_rotating_right(true);
        }
    }*/

}
