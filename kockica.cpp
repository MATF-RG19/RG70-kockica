#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string.h>
#include "image.hpp"

#define TIMER_INTERVAL 20
#define TIMER_ID 0

#define FILENAME "wall.bmp"

static GLuint name[1];

static void on_display();
static void on_reshape(int width, int height);
static void on_keyboard(unsigned char key, int x, int y);
static void on_timer(int id);

static void draw_axis(float len);
static void draw_cube();

float ubrzanje=0.1;
char jedinice='0',desetice='0',stotine='0',hiljade='0';
int rez=0;
int z=0;
int r=0;
int o1z=0,o2z=0,o3z=0,o4z=0;
int bonus1z=0,pokupio_bonus1=0;
int bonus2z=0,pokupio_bonus2=0,bonus2radi=0;
int kraj=0;
float obstacle1_parameter=0,obstacle2_parameter=-5,obstacle3_parameter=0,obstacle4_parameter=-5;
float animation_parameter = 0;
int animation_ongoing = 0;


int main(int argc, char **argv)
{
    srand(time(NULL));
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

    Image *image;
    
    glClearColor(.5,.5,.5,0);
    
    glEnable(GL_TEXTURE_2D);
    
    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);
    
    image = image_init(0, 0);
 
    char *fname=strdup(FILENAME);
    
    image_read(image, fname);

    /* Generisu se identifikatori tekstura. */
    glGenTextures(1, name);

    glBindTexture(GL_TEXTURE_2D, name[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    image_done(image);
    
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
            if(!animation_ongoing && kraj==0)
                glutTimerFunc(TIMER_INTERVAL,on_timer,TIMER_ID);
            animation_ongoing=1;
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
        case 'r':
        case 'R':{
            ubrzanje=0.1;
            jedinice='0';desetice='0';stotine='0';hiljade='0';
            rez=0;
            z=0;
            r=0;
            o1z=0;o2z=0;o3z=0;o4z=0;
            kraj=0;
            obstacle1_parameter=0;obstacle2_parameter=-5;obstacle3_parameter=0;obstacle4_parameter=-5;
            bonus1z=0;
            bonus2z=0;
            animation_parameter = 0;
            animation_ongoing=0;
            break;
        }
        case 27:
            glDeleteTextures(1, name);
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void on_timer(int id) {
    if (id == TIMER_ID) {
        animation_parameter+=1;
        if(bonus2radi>0)
            bonus2radi--;
        if(obstacle1_parameter>10){
            jedinice++;
            if(jedinice==':'){
                jedinice='0';
                desetice++;
            }
            if(desetice==':'){
                desetice='0';
                stotine++;
            }
            if(stotine==':'){
                stotine='0';
                hiljade++;
            }
            
            rez++;
            obstacle1_parameter=0;
            r=rand()%100;
            if(r<33)
                o1z=-2;
            else if(r>=33 && r<=66)
                o1z=0;
            else if(r>66)
                o1z=2;
            
            r=rand()%100;
            if(r<33)
                bonus1z=-2;
            else if(r>=33 && r<=66)
                bonus1z=0;
            else if(r>66)
                bonus1z=2;
            
            pokupio_bonus1=0;
            
            ubrzanje+=0.001;
        }
        if(obstacle2_parameter>10){
            jedinice++;
            if(jedinice==':'){
                jedinice='0';
                desetice++;
            }
            if(desetice==':'){
                desetice='0';
                stotine++;
            }
            
            rez++;
            
            obstacle2_parameter=0;
            r=rand()%100;
            if(r<33)
                o2z=2;
            else if(r>=33 && r<=66)
                o2z=0;
            else if(r>66)
                o2z=-2;
            
            r=rand()%100;
            if(r<33)
                bonus2z=-2;
            else if(r>=33 && r<=66)
                bonus2z=0;
            else if(r>66)
                bonus2z=2;
            
            pokupio_bonus2=0;
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
        
        obstacle1_parameter+=ubrzanje;
        obstacle2_parameter+=ubrzanje;
        obstacle3_parameter+=ubrzanje;
        obstacle4_parameter+=ubrzanje;
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

//funkcija koja crta prepreku
void draw_obstacle(){
    glPushMatrix();
    
    if(bonus2radi==0){
        GLfloat diffuse[] = {1,0,0,0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    }
    else{
        GLfloat diffuse2[] = {1,1,1,0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse2);
    }
    
    glutSolidCube(.5);
    
    GLfloat diffuse1[] = {1,1,1,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);

    glPopMatrix();
}

//funkcija koja crta plavu lopticu za bonus
void draw_bonus1(){
    glPushMatrix();

    GLfloat diffuse[] = {0,0,1,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    
    glutSolidSphere(.25,10,10);

    GLfloat diffuse1[] = {1,1,1,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);
    
    glPopMatrix();
}

//funkcija koja crta zelenu lopticu za bonus
void draw_bonus2(){
    glPushMatrix();

    GLfloat diffuse[] = {0,1,0,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    
    glutSolidSphere(.25,10,10);

    GLfloat diffuse1[] = {1,1,1,0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse1);
    
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

//     draw_axis(5);
    
    glBindTexture(GL_TEXTURE_2D, name[0]);
    glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);

        glTexCoord2f(0, 0);
        glVertex3f(-2, -8, 8);

        glTexCoord2f(20, 0);
        glVertex3f(-2, -8, -8);

        glTexCoord2f(20, 20);
        glVertex3f(-2, 1.6, -8);

        glTexCoord2f(0, 20);
        glVertex3f(-2, 1.6, 8);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glPushMatrix();
        glTranslatef(0,0,z);
        draw_cube(); 
    glPopMatrix();
    
    if(pokupio_bonus1==0){
        glPushMatrix();
            glTranslatef(0,obstacle1_parameter-7,bonus1z);
            draw_bonus1(); 
        glPopMatrix();
    }
    
    if(pokupio_bonus2==0){
        glPushMatrix();
            glTranslatef(0,obstacle2_parameter-7,bonus2z);
            draw_bonus2(); 
        glPopMatrix();
    }
    
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
    
    if(obstacle1_parameter>6.1 && obstacle1_parameter<7.5 && z==o1z && bonus2radi==0){
        animation_ongoing=0;
        kraj=1;
    }
    else if(obstacle2_parameter>6.1 && obstacle2_parameter<7.5 && z==o2z && bonus2radi==0){
        animation_ongoing=0;
        kraj=1;
    }
    else if(obstacle3_parameter>6.1 && obstacle3_parameter<7.5 && z==o3z && bonus2radi==0){
        animation_ongoing=0;
        kraj=1;
    }
    else if(obstacle4_parameter>6.1 && obstacle4_parameter<7.5 && z==o4z && bonus2radi==0){
        animation_ongoing=0;
        kraj=1;
    }
    
    else if(obstacle1_parameter>6.1 && obstacle1_parameter<7.5 && z==bonus1z){
        if(pokupio_bonus1==0){
            desetice++;
            if(desetice==':'){
                desetice='0';
                stotine++;
            }
            if(stotine==':'){
                stotine='0';
                hiljade++;
            }
            rez+=10;
            pokupio_bonus1++;
        }
    }
    
    else if(obstacle2_parameter>6.1 && obstacle2_parameter<7.5 && z==bonus2z){
        if(pokupio_bonus2==0){
            bonus2radi=75;
            pokupio_bonus2++;
        }
    }
    
    if(kraj==1)
        printf("%d\n",rez);
    
    glRasterPos3f(0,2,-2.6);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,jedinice);
    glRasterPos3f(0,2,-2.5);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,desetice);
    glRasterPos3f(0,2,-2.4);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,stotine);
    glRasterPos3f(0,2,-2.3);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,hiljade);
    
    
    glutSwapBuffers();
}





