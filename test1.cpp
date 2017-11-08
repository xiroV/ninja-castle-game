#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include<iostream>
#include<time.h>
#include<math.h>

#define MAP_SIZE 60.0
#define CHAR_SIZE 5.0
#define CHAR_MOVE_SPEED 1
#define CHAR_MOVE_FRAMERATE 50
#define CHAR_ROT_SPEED 1
#define CHAR_ROT_FRAMERATE 1

#define PI 3.14159265


// Begin globals.
float pos_v = 3.0;
float pos_h = 3.0;
float angle = 0.0;

bool move = false;
bool rotate_l = false;
bool rotate_r = false;
int cur_time;
int last_move_time = 0;
int last_rot_time = 0;

float mat0_color[] = {0.0, 0.0, 1.0, 1.0};
float mat1_color[] = {1.0, 0.0, 0.0, 1.0};
float mat1_spec[] = {0.8, 0.8, 0.8, 1.0};
float mat1_shine[] = {30};
//float matAmbAndDif2[] = {0.3, 0.3, 0.7, 1.0};
//float matSpec[] = {0.5, 0.5, 0.5, 1.0};
//float matShine[] = {50.0};*/



// Drawing routine.
void display(void) {

    //std::cout << "display(): pos_v=" << pos_v << " pos_h=" << pos_h << " angle=" << angle << std::endl;

    bool changed = false;


	//glEnable(GL_BLEND); // Enable blending.
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
    //glEnable(GL_LINE_SMOOTH); // Enable line antialiasing.
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.

    //glEnable(GL_POINT_SMOOTH);  // Enable point antialiasing.
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Ask for best point antialiasing.


    //glEnable(GL_MULTISAMPLE); // Enable multisampling.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    
    // Stationary camera with eye-movement
    //gluLookAt(-2.0, 25.0, -10.0, (float)pos_h, 0.0, (float)pos_v, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
    
    // Following camera
    //gluLookAt((float)pos_h-10.0, 20.0, (float)pos_v-30.0, (float)pos_h, 0.0, (float)pos_v, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
    
    // Sticky/behind camera

    float cam_pos_h = pos_h - (pos_h * sin(angle*PI/180));
    float cam_pos_v = pos_v - (pos_h * cos(angle*PI/180));

    gluLookAt(cam_pos_h, 20.0, cam_pos_v, (float)pos_h, 0.0, (float)pos_v, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
     



    glPolygonMode(GL_FRONT, GL_FILL);

    // Material properties of the box.
    /*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);*/


    // Draw floor
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat0_color);
    glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, MAP_SIZE);
        glVertex3f(MAP_SIZE, 0.0, MAP_SIZE);
        glVertex3f(MAP_SIZE, 0.0, 0.0);
    glEnd();




    
    // Movement to position 
    glTranslatef((float)pos_h, CHAR_SIZE/2, (float)pos_v);

    // Rotation around itself
    glRotatef((float)angle, 0, 1, 0);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat1_color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shine);

    glutSolidSphere(CHAR_SIZE-1, 50, 50); // Box.


    glEnable(GL_DEPTH_TEST);





    // Cull the back faces of the sphere.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    glFlush(); 

    cur_time = glutGet(GLUT_ELAPSED_TIME);

    // Left Rotation
    if(rotate_l) {
        //std::cout << "Rotating left" << std::endl;
        if(cur_time - last_rot_time > CHAR_ROT_FRAMERATE) {
            angle++;
            if(angle >= 360) {
                angle = 0;
            }
        }
        changed = true;
    }

    // Movement
    if(move && pos_v < MAP_SIZE && pos_v > 0 && pos_h < MAP_SIZE && pos_h > 0) {
        if(cur_time - last_move_time > CHAR_MOVE_FRAMERATE) {

            //std::cout << "Moving!" << std::endl;
            // Calculate new position based on angle
            pos_h = pos_h + sin(angle*PI/180)/sin(90*PI/180);
            pos_v = pos_v + sin((180.0-angle-90.0)*PI/180)/sin(90*PI/180);

            if(pos_h > MAP_SIZE-CHAR_SIZE/2) {
                pos_h = MAP_SIZE-CHAR_SIZE/2;
            }

            if(pos_v > MAP_SIZE-CHAR_SIZE/2) {
                pos_v = MAP_SIZE-CHAR_SIZE/2;
            }

            if(pos_h < CHAR_SIZE/2) {
                pos_h = CHAR_SIZE/2;
            }

            if(pos_v < CHAR_SIZE/2) {
                pos_v = CHAR_SIZE/2;
            }
        }
        changed = true;
    }

    // Right Rotation
    if(rotate_r) {
        if(cur_time - last_rot_time > CHAR_ROT_FRAMERATE) {
            angle--;
            if(angle <= 0) {
                angle = 360;
            }
        }
        changed = true;
    }


    
    if(changed) {
        if(cur_time - last_move_time > CHAR_MOVE_FRAMERATE) {
            last_move_time = cur_time;
        }
        if(cur_time - last_rot_time > CHAR_ROT_FRAMERATE) {
            last_rot_time = cur_time;
        }
        glutPostRedisplay();
    }

}

// Initialization routine.
void setup(void) 
{
    glClearColor(1.0, 1.0, 1.0, 1.0); 

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

    // Set global ambient light
    /*float global_ambient[] = {0.3, 0.3, 0.3, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);*/

    // Define light vectors for LIGHT0
    float ambient[] = {0.4, 0.4, 0.4, 1.0};
    float diffuse[] = {0.6, 0.6, 0.6, 1.0};
    float specular[] = {1.0, 1.0, 1.0, 1.0};
    float position[] = {10.0, 15.0, 10.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    




    glShadeModel(GL_SMOOTH);

   // Light property vectors.
   /*float lightAmb[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };*/

   // Light properties.
   //glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   //glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   /*glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);*/
   
   //glEnable(GL_LIGHT0); // Enable particular light source.
   /*glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
   

   // Material properties.
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
   */
   

   glEnable(GL_AUTO_NORMAL); // Automatic normal calculation for Bezier surfaces.
   glEnable(GL_NORMALIZE); // Automatically resize all normals to unit length.
   
   glEnable(GL_LINE_SMOOTH);

   glColor3f(0.0, 0.0, 0.0);
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
   gluPerspective(90.0, 0.5, 1.0, 100.0); // fovy/angle, aspect, near, far
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// ASCII keyboard input
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

// Called on non-ascii keypress
void specialInput(int key, int x, int y)
{
    if(key == GLUT_KEY_UP) {
        if (pos_v < 50) {
            move = true;
            //pos_v++;
            //glutPostRedisplay();
        }
    }
    if(key == GLUT_KEY_DOWN) {
        if (pos_v > 3) {
            pos_v--;
        }
    }
    if(key == GLUT_KEY_LEFT) {
        rotate_l = true;
    }
    if(key == GLUT_KEY_RIGHT) {
        rotate_r = true;
    }

    glutPostRedisplay();
}

// Called on keyup
void specialUpInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) {
       if (pos_v < 50) {
           move = false;
           glutPostRedisplay();
           //pos_v++;
       }
   }
   if(key == GLUT_KEY_DOWN) {
       
       if (pos_v > 3) {
           pos_v--;
       }
   }
   if(key == GLUT_KEY_LEFT) {
       rotate_l = false;
   }
   if(key == GLUT_KEY_RIGHT) {
       rotate_r = false;
   }

   glutPostRedisplay();
}

// Main routine.
int main(int argc, char **argv) 
{
   //std::cout << "main()" << std::endl;
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH); 
   glutInitWindowSize(1024, 768);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("square.cpp");
   glutDisplayFunc(display); 
   glutReshapeFunc(resize);  
   glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialInput);
   glutSpecialUpFunc(specialUpInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
   glutMainLoop(); 
}
