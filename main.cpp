#include"common.h"
#include"player.h"
#include"map.h"
#include"collision.h"

Player player;
Map map;
Collision collision;

// Drawing routine.
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0, 1.0, 0.1, 100.0); // fov/angle, aspect, near, far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // Sticky/behind camera 
    float cam_pos_x = player.x-(3*sin(player.angle*PI/180));
    float cam_pos_y = player.y-(3*cos(player.angle*PI/180));

    glm::vec2 cam_pos = glm::vec2(cam_pos_x, cam_pos_y);
    glm::normalize(cam_pos);
    gluLookAt(
        cam_pos[0], // Position x
        player.z+2, // Position z
        cam_pos[1], // Position y
        player.x,   // Look at x
        player.z+1.0,        // Look at z
        player.y,   // Look at y
        0.0,        // Up rotation
        1.0,        // Up rotation
        0.0         // Up rotation
    );

    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    // Cull the back faces of the models
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);*/

    map.draw();
    player.draw();
    /*if(collision.on_floor(player.x, player.y)) {
        std::cout << "Player is on floor" << std::endl;
    }
    if(collision.wall_hit(player.x, player.y)) {
        std::cout << "Player hit wall" << std::endl;
    }*/

    glFlush(); 
    glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{

    std::cout << "Setup" << std::endl;

    // Sky color
    glClearColor(0.67578125, 0.87109375, 0.96484375, 1.0); 

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
	glEnable(GL_BLEND); // Enable blending.
    glEnable(GL_MULTISAMPLE); // Enable multisampling.

    // Define light vectors for LIGHT0
    float ambient[] = {1.0, 1.0, 1.0, 1.0};
    float diffuse[] = {0.6, 0.6, 0.6, 1.0};
    float specular[] = {0.3, 0.3, 0.3, 1.0};
    float position[] = {10.0, 15.0, 10.0};

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
    player.init((char*)"models/ninja.obj", Team::BLUE, 22.5, 6.5, collision);

    glutSwapBuffers();
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

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
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
