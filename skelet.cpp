#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>

#define TIMER_INTERVAL 20
#define TIMER_ID 0

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);

static void draw_axis(float len);
static void draw_cube();

float z=0;
int r=0;
int o1z=0,o2z=0,o3z=0,o4z=0;
float obstacle1_parameter=0,obstacle2_parameter=-5,obstacle3_parameter=0,obstacle4_parameter=-5;
float animation_parameter = 0;
int animation_ongoing = 0;


int main(int argc, char **argv)
{
    /* Inicijalizuje se GLUT. */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    /* Kreira se prozor. */
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    /* Registruju se callback funkcije */
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    /* Obavlja se OpenGL inicijalizacija. */
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float light_position[] = {0, 10, 20, 0};
//     float light_ambient[] = {.3f, .3f, .3f, 1};
//     float light_diffuse[] = {.7f, .7f, .7f, 1};
//     float light_specular[] = {.7f, .7f, .7f, 1};
// 
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//     glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//     glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glClearColor(.75,.75,.75,0);
 
    glutMainLoop();

  return 0;
}

/*
 *POMOCNA FUNKCIJA ZA ISCRTAVANJE KOODINATNOG SISTEMA
 */
void draw_axis(float len) {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(0,0,0);
        glVertex3f(len,0,0);

        glColor3f(0,1,0);
        glVertex3f(0,0,0);
        glVertex3f(0,len,0);

        glColor3f(0,0,1);
        glVertex3f(0,0,0);
        glVertex3f(0,0,len);
    glEnd();

    glEnable(GL_LIGHTING);
}

void on_keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'g':
        case 'G':
            animation_ongoing=1;
            glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            break;
        case 'a':
        case 'A':
            if(z<=0){
                z+=2;
            }
            break;
        case 'd':
        case 'D':
            if(z>=0)
                z-=2;
            break;
        case 27:
          exit(0);
          break;
    }
    glutPostRedisplay();
}

void on_timer(int id) {
    if (id == TIMER_ID) {
        animation_parameter+=1;
        if(obstacle1_parameter>10){
            obstacle1_parameter=0;
            r=rand()%100;
            if(r<33)
                o1z=-2;
            else if(r>=33 && r<=66)
                o1z=0;
            else if(r>66)
                o1z=2;
        }
        if(obstacle2_parameter>10){
            obstacle2_parameter=0;
            r=rand()%100;
            if(r<33)
                o2z=2;
            else if(r>=33 && r<=66)
                o2z=0;
            else if(r>66)
                o2z=-2;
        }
        if(obstacle3_parameter>10){
            obstacle3_parameter=0;
            r=rand()%100;
            if(r<33)
                o3z=2;
            else if(r>=33 && r<=66)
                o3z=0;
            else if(r>66)
                o3z=-2;
        }
        if(obstacle4_parameter>10){
            obstacle4_parameter=0;
            r=rand()%100;
            if(r<33)
                o4z=-2;
            else if(r>=33 && r<=66)
                o4z=0;
            else if(r>66)
                o4z=2;
        }
        
        obstacle1_parameter+=0.1;
        obstacle2_parameter+=0.1;
        obstacle3_parameter+=0.1;
        obstacle4_parameter+=0.1;
    }

    glutPostRedisplay();

    if(animation_ongoing)
        glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
   
}

void on_reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, (float) width/height, 1, 1000);
}

void draw_cube(){
    glPushMatrix();

    glutSolidCube(1);

    glPopMatrix();
}

void draw_obstacle(){
    glPushMatrix();

    glutSolidCube(.5);

    glPopMatrix();
}

void on_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(10, 5, 0,
              0, -1, 0,
              0, 1, 0);


    GLfloat diffuse[] = {1,1,1,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

    draw_axis(5);
    glPushMatrix();
        glTranslatef(0,0,z);
        draw_cube(); 
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0,obstacle1_parameter-7,o1z);
        draw_obstacle(); 
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0,obstacle2_parameter-7,o2z);
        draw_obstacle(); 
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0,obstacle3_parameter-7,o3z);
        draw_obstacle(); 
    glPopMatrix();
    
    glPushMatrix();
        glTranslatef(0,obstacle4_parameter-7,o4z);
        draw_obstacle(); 
    glPopMatrix();
    
    
    glutSwapBuffers();
}





