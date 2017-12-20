#include"common.h"
#include"character.h"
#include"player.h"
#include"ai.h"
#include"map.h"
#include"core.h"

Player player;
AI ai;
Map map;
Collision collision;
int start_time = std::time(nullptr);
int elapsed_time;
int last_score_print_time;

// Drawing routine.
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    elapsed_time = std::time(nullptr);

    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0, 1.0, 0.1, 100.0); // fov/angle, aspect, near, far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    // Sticky/behind camera 
    float cam_pos_x = player.x-(3*sin(player.angle*PI/180));
    float cam_pos_y = player.y-(3*cos(player.angle*PI/180));
    float cam_pos_z;
    if(player.z >= -1.0) { 
        cam_pos_z = player.z+2;
    } else {
        cam_pos_z = 3.0;
    }

    glm::vec3 cam_pos = glm::vec3(cam_pos_x, cam_pos_y, cam_pos_z);
    glm::normalize(cam_pos);
    gluLookAt(
        cam_pos[0],  // Position x
        cam_pos[2],  // Position z
        cam_pos[1],  // Position y
        player.x,    // Look at x
        player.z+1.0,// Look at z
        player.y,    // Look at y
        0.0,         // Up rotation
        1.0,         // Up rotation
        0.0          // Up rotation
    );


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    // Cull the back faces of the models
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glPushMatrix();
    map.draw();
    glPopMatrix();

    glPushMatrix();
    player.draw();
    glPopMatrix();

    glPushMatrix();
    ai.draw();
    ai.ai();
    glPopMatrix();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos3f(10.0, 20.0, 10.0);
    if((elapsed_time-start_time) % 5 == 0 && (elapsed_time-start_time != last_score_print_time)) {
        std::cout << "Points; Player " << player.time_in_center << " - Computer " << ai.time_in_center << std::endl;
        last_score_print_time = elapsed_time-start_time;

        if(player.time_in_center >= 500) {
            std::cout << "Player won the game by " << player.time_in_center - ai.time_in_center << " points!" << std::endl;
            ai.time_in_center = 0;
            player.time_in_center = 0;
            ai.respawn();
            player.respawn();
        }
        if(ai.time_in_center >= 500) {
            std::cout << "Computer won the game by " << ai.time_in_center - player.time_in_center << " points!" << std::endl;
            ai.time_in_center = 0;
            player.time_in_center = 0;
            ai.respawn();
            player.respawn();
        }
    }

    if(ai.changed || player.changed) {
        glutPostRedisplay();
    }

    //glFlush(); 
    glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // Sky color
    glClearColor(0.67578125, 0.87109375, 0.96484375, 1.0); 
    glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND); // Enable blending.
    glEnable(GL_MULTISAMPLE); // Enable multisampling.
    glEnableClientState(GL_VERTEX_ARRAY);

    // Define light vectors for LIGHT0
    float ambient[] = {1.0, 1.0, 1.0, 1.0};
    float diffuse[] = {0.8, 0.8, 0.8, 1.0};
    float specular[] = {0.3, 0.3, 0.3, 1.0};
    float position[] = {22.5, 31.0, 10.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_AUTO_NORMAL); // Automatic normal calculation for Bezier surfaces.
    glEnable(GL_NORMALIZE); // Automatically resize all normals to unit length.
   
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    map.init("models/map1.obj");
    collision.init("models/map1.obj");
    player.init((unsigned int)0, (char*)"models/ninja.obj", Team::BLUE, 22.5, 6.5, &collision);
    ai.init((unsigned int)1, (char*)"models/ninja.obj", Team::RED, 22.5, 8.0, &collision);

    std::cout << "Game initialization finished" << std::endl << std::endl;
    std::cout << "- Stay on the platform in the middle to gain points." << std::endl;
    std::cout << "- The player who first gets a score of 500 wins the game." << std::endl;
    std::cout << "- Move using the Arrow keys, press Z to jump." << std::endl << std::endl;

    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   //glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0); // left, right, bottom, top, near, far
   //glFrustum(0.0, 100.0, 0.0, 100.0, -1.0, 1.0); // left, right, bottom, top, near, far
   gluPerspective(90.0, 0.5, 1.0, 100.0); // fov/angle, aspect, near, far
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// ASCII keyboard input
void keyInput(unsigned char key, int x, int y)
{
    switch(key) 
    {
        case 'z':
            player.set_jumping(true);
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}

// ASCII keyboard input release
void keyUpInput(unsigned char key, int x, int y) {
    switch(key) 
    {
        case 'z':
            player.set_jumping(false);
            break;
     default:
            break;
    }
}

// Called on non-ascii keypress
void specialInput(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
        player.set_moving(true);
    }
    if(key == GLUT_KEY_DOWN) {
        player.set_backing(true);
    }
    if(key == GLUT_KEY_LEFT) {
        player.set_rotating_left(true);
    }
    if(key == GLUT_KEY_RIGHT) {
        player.set_rotating_right(true);
    }
    glutPostRedisplay();
}

// Called on keyup
void specialUpInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) {
       player.set_moving(false);
   }
   if(key == GLUT_KEY_DOWN) {
       player.set_backing(false);
   }
   if(key == GLUT_KEY_LEFT) {
       player.set_rotating_left(false);
   }
   if(key == GLUT_KEY_RIGHT) {
       player.set_rotating_right(false);
   }

   glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv) {
    
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE); 
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Awesome Game");
    glutDisplayFunc(display); 
    glutReshapeFunc(resize);  
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutKeyboardFunc(keyInput);
    glutKeyboardUpFunc(keyUpInput);
    glutSpecialFunc(specialInput);
    glutSpecialUpFunc(specialUpInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup(); 
    glutMainLoop(); 
}
