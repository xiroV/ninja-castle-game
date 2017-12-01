#ifndef __COMMON_H_INCLUDED__
#define __COMMON_H_INCLUDED__

#include<glm/glm.hpp>
#include<glm/vec3.hpp>
#include<glm/vec2.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<GL/glext.h>
#include<math.h>
#include<iostream>
#include<time.h>
#include<vector>
#include<string.h>
#include<fstream>

#define MAP_SIZE_Y 52.0
#define MAP_SIZE_X 35.0
#define CHAR_SIZE 1.0
#define CHAR_MOVE_SPEED 0.3
#define CHAR_MOVE_FRAMERATE 30
#define CHAR_BACK_FRAMERATE 50
#define CHAR_ROT_SPEED 1
#define CHAR_ROT_FRAMERATE 1
#define GRAVITY 0.5

#define PI 3.14159265

enum Team {
    RED,
    BLUE
};

#endif
