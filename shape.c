#include <GL/glut.h>
#include <math.h>

void WireSphere(GLfloat radius, int slices, int stacks) {
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutWireSphere(radius, slices, stacks);
    glPopMatrix();
}

static int t1 = 0, t2 = 0;


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // Draw sun: a yellow sphere of radius 1 centered at the origin.
    glColor3f(1.0, 1.0, 0.0);
    WireSphere(1.0, 15, 15);

    glRotatef((GLfloat)t1, 0.0, 1.0, 0.0);
    glTranslatef (2.0, 0.0, 0.0);
    glRotatef((GLfloat)t2, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 1.0);
    glColor3f(1, 1, 1);
    glLoadIdentity();

    glFlush();
    glutSwapBuffers();
}

static GLfloat u = 0.0;
static GLfloat du = 0.1;

void animate(int v) {
    u += du;
    t1 = (t1 + 1) % 360;
    t2 = (t2 + 2) % 360;
    glLoadIdentity();
    gluLookAt(20*cos(u/8.0)+12,5*sin(u/8.0)+1,10*cos(u/8.0)+2, 0,0,0, 0,1,0);
    glutPostRedisplay();
    glutTimerFunc(1000/60, animate, v);
}

void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 40.0);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sphere");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //glutTimerFunc(100, animate, 0);
    init();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}