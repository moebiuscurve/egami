#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
enum {
  SQUARE,
  RIGHT,
  FOLDED
} States;

int motion = 1;
int state = SQUARE;
int waitingtime = 0;
int direction;
float angle = 0;
float xrot = 0;
float yrot = 0;
float zrot = 0;

typedef GLfloat Point[2];
Point triangle_left[] =
{
  {-1, 1},
  {-1, -1},
  {1,-1}
};
Point triangle_right[] =
{
  {-1, 1},
  {1, 1},
  {1,-1}
};


void polygonlist(int index, int num, Point polygon[])
{
  int i;
  glNewList(index, GL_COMPILE);
  glBegin(GL_POLYGON);
  for (i = 0; i < num; i++) {
    glVertex2fv(&polygon[i][0]);
  }
  glEnd();
  glEndList();
}

void
idle(void)
{
  switch (state) {
  case SQUARE:
    waitingtime++;
    if (waitingtime >= 80) {
      waitingtime = 0;
      state = RIGHT;
      glutSetWindowTitle("fold");
      direction = 1;
    }
    break;
  case RIGHT:
    angle += 2 * direction;
    if (angle >= 180) {
      state = FOLDED;
    }
    else if (angle <= 0) {
      state = SQUARE;
    }
    break;
  case FOLDED:
    waitingtime++;
    if (waitingtime >= 80) {
      waitingtime = 0;
      glutSetWindowTitle("unfold");
      direction = -1;
      state = RIGHT;
    }
    break;
  }
  glutPostRedisplay();
}

void
fold_square(void)
{
	glPushMatrix();
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);
	glRotatef(zrot,0,0,1);
	glCallList(1);
	glPushMatrix();
		glRotatef(-angle, -1, 1, 0);
		glCallList(2);
	glPopMatrix();
	glPopMatrix();
}

void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3ub(67, 205, 128);
  fold_square();
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor3ub(255, 255, 255);
  glPushMatrix();
  glTranslatef(0, 0, .05);
  fold_square();
  glPopMatrix();
  glutSwapBuffers();
}

void
visible(int state)
{
  if (state == GLUT_VISIBLE) {
    if (motion)
      glutIdleFunc(idle);
  } else {
    glutIdleFunc(NULL);
  }
}

void mouseMotion(int x,int y)
{
		xrot+=(x/300);
		yrot+=(y/300);
	glFlush();
}

void
menuItem(int value)
{
  switch (value) {
  case 1:
    direction = -direction;
    if (direction > 0) {
      glutSetWindowTitle("fold");
    } else {
      glutSetWindowTitle("unfold");
    }
    break;
  case 2:
    motion = 1 - motion;
    if (motion) {
      glutIdleFunc(idle);
    } else {
      glutIdleFunc(NULL);
    }
    break;
  case 6:
    exit(0);
  }
}

int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(660,660);
  glutCreateWindow("Origami");
  glutDisplayFunc(display);
  glutVisibilityFunc(visible);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(40.0, 1.0, 0.1, 10.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(0, 0, 5.5, 0, 0, 0, 0, 1, 0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glLineWidth(2.0);
  polygonlist(1, sizeof(triangle_left) / sizeof(Point), triangle_left);
  polygonlist(2, sizeof(triangle_right) / sizeof(Point), triangle_right);
  glutCreateMenu(menuItem);
  glutAddMenuEntry("Change direction", 1);
  glutAddMenuEntry("Motion control", 2);
  glutAddMenuEntry("Quit", 6);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMotionFunc(mouseMotion);
  glutMainLoop();
  return 0;
}

