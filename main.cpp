#include"common.h"
#include"player.h"
#include"map.h"




// Begin globals.
int last_move_time = 0;
int last_rot_time = 0;
Player player;
Map map;

/*float mat0_color[] = {0.0, 0.0, 1.0, 1.0};
float mat1_color[] = {1.0, 0.0, 0.0, 1.0};
float mat1_spec[] = {0.8, 0.8, 0.8, 1.0};
float mat1_shine[] = {30};*/


//float matAmbAndDif2[] = {0.3, 0.3, 0.7, 1.0};
//float matSpec[] = {0.5, 0.5, 0.5, 1.0};
//float matShine[] = {50.0};*/

GLuint ninja;
float ninjarot;
char ch='1';

void loadObj(char *fname) {
    FILE *fp;
    int read;

    GLfloat x, y, z;

    char ch;

    ninja = glGenLists(1);
    fp = fopen(fname, "r");
    if(!fp) {
        printf("Can't open file %s\n", fname);
        exit(1);
    }

    glPointSize(2.0);

    glNewList(ninja, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        while(!(feof(fp))) {
            read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
            if(read == 4 && ch == 'v') {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }

    glPopMatrix();
    glEndList();
    fclose(fp);
}

void drawNinja() {
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glColor3f(1.0,0.23,0.27);
    glScalef(1.1,1.1,1.1);
    glRotatef(ninjarot, 0, 1, 0);
    glCallList(ninja);
    glPopMatrix();
    ninjarot=ninjarot + 5.0;
    if(ninjarot> 360) {
        ninjarot = ninjarot - 360;
    }
}


// Drawing routine.
void display(void) {
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Specify blending parameters.
    //glEnable(GL_LINE_SMOOTH); // Enable line antialiasing.
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Ask for best line antialiasing.

    //glEnable(GL_POINT_SMOOTH);  // Enable point antialiasing.
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); // Ask for best point antialiasing.


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    
    // Stationary camera with eye-movement
    //gluLookAt(-2.0, 25.0, -10.0, (float)pos_h, 0.0, (float)pos_v, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
    
    // Following camera
    //gluLookAt(player.x-1.0, 3.0, player.y-3.0, player.x, 0.0, player.y, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0, 1.0, 0.1, 100.0); // fov/angle, aspect, near, far

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();



    // Sticky/behind camera 
    //float cam_pos_x = player.x - (player.x * sin(player.angle*PI/180));
    //float cam_pos_y = player.y - (player.y * cos(player.angle*PI/180));
    float cam_pos_x = player.x-(3*sin(player.angle*PI/180));
    float cam_pos_y = player.y-(3*cos(player.angle*PI/180));
    glm::vec2 cam_pos = glm::vec2(cam_pos_x, cam_pos_y);
    glm::normalize(cam_pos);
    gluLookAt(cam_pos[0], 2.0, cam_pos[1], player.x, 1.0, player.y, 0.0, 1.0, 0.0); // eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz

    /*glm::mat4 view = glm::lookAt(
        glm::vec3(cam_pos_h, 10.0, cam_pos_v),
        glm::vec3(player.x, 0.0, player.y),
        glm::vec3(0.0, 1.0, 0.0)
    );*/

    glPopMatrix();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Draw floor
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat0_color);
    /*glBegin(GL_POLYGON);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, MAP_SIZE);
        glVertex3f(MAP_SIZE, 0.0, MAP_SIZE);
        glVertex3f(MAP_SIZE, 0.0, 0.0);
    glEnd();*/


    glEnable(GL_DEPTH_TEST);


    // Cull the back faces of the sphere.
    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);*/

    map.draw();
    player.draw();
    //drawNinja();

    glFlush(); 
    glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
    glClearColor(1.0, 1.0, 1.0, 1.0); 

    // Turn on OpenGL lighting.
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);
	glEnable(GL_BLEND); // Enable blending.
    glEnable(GL_MULTISAMPLE); // Enable multisampling.

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


    map.init((char*)"models/map1.obj");
    player.init((char*)"models/ninja.obj", true, Team::BLUE, 2.0, 2.0);

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
   //std::cout << "main()" << std::endl;
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
   glutSpecialFunc(specialInput);
   glutSpecialUpFunc(specialUpInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 
   
//loadObj((char*)"models/map1.obj");

   glutMainLoop(); 
}
