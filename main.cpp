/*
Info: In the videogame Fortnite, at some given times the player has to move towards a specific
zone on the map, (marked with a circle on the minimap). To guide the player to this zone, the game
draws a line on the minimap, from the player to the center of the said zone, with the particularity
that the line is drawn from the player until it reaches the circle (not all the way to the center of it).

Drawing a line from the player to the center of a circle is very easy. But how do you draw a
line from the player to the bounds of a circle? This program demonstrates how to do just that.

Use the mouse to move the player around the map.
Use A and D to decrease / increase the circle's radius.

As a side exercise, the program also demonstrates how to make the peak of a triangle always point
towards a specific point, no matter where the triangle is located. (In this particular case the player
is drawn as a triangle and one of its peaks will always point towards the center of the circle)
*/

#include "GL/glut.h"
#include <glm/glm.hpp>
#include <iostream>

using namespace glm;
using namespace std;

//screen constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

const GLfloat TWICE_PI = 2.0f * M_PI;

//variables
int boundsRadius = 100;
float playerRotation = 0.0f;
vec2 centerPoint(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
vec2 playerPos = centerPoint;
vec2 intersectionPoint(0, 0);

void changeBoundsRadiusBy(int amount)
{
    boundsRadius += amount;

    if(boundsRadius < 10)
        boundsRadius = 10;
}

void refreshData()
{
	//for intersection point with bounds circle
	//create a vector from the center of the circle to the player, normalize it,
	//and scale it to the circle's radius
	intersectionPoint = normalize(playerPos - centerPoint) * float(boundsRadius);

	//for player rotation
	//create a vector from the center of the circle to the player, and calculate its angle with the x-ax
	vec2 centerToPlayer = playerPos - centerPoint; //calculate a vector from the center of the circle to the player

	cout << "---------------" << endl;

	//calculate the angle (in radians) that the centerToPlayer vector forms with the x-axis
	playerRotation = atan2(centerToPlayer.y, centerToPlayer.x);
    cout << "angle (rad): " << playerRotation << endl;

    //convert the previous angle to degrees
	playerRotation = degrees(playerRotation);
	cout << "angle (deg): " << playerRotation << endl;

	//add 180 degrees to the rotation
	//(because i want the peak of the triangle to point
    //towards the center of the circle, not the opposite way)
	playerRotation = 180.f + playerRotation;
	cout << "angle (+180 deg): " << playerRotation << endl;
}

// Keyboard function
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a':
		case 'A':
			changeBoundsRadiusBy(-5);
			break;
		case 'd':
		case 'D':
			changeBoundsRadiusBy(5);
			break;
		default:
			break;
	}

	refreshData();
	glutPostRedisplay();
}

void motionCallBack(int x, int y)
{
	playerPos.x = x;
	playerPos.y = SCREEN_HEIGHT - y;

	refreshData();

	glutPostRedisplay();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);
	glEnd();
}

void drawFilledPolygon(GLfloat x, GLfloat y, GLfloat radius, int sides)
{
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i <= sides; i++)
			glVertex2f(x + (radius * cos(i *  TWICE_PI / sides)), y + (radius * sin(i * TWICE_PI / sides)));
	glEnd();
}

void drawStrokePolygon(GLfloat x, GLfloat y, GLfloat radius, int sides)
{
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i <= sides; i++)
			glVertex2f(x + (radius * cos(i *  TWICE_PI / sides)), y + (radius * sin(i * TWICE_PI / sides)));
	glEnd();
}

void display()
{
	glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);

	//bounds circle
	glColor3f(0, 0, 0);
	drawStrokePolygon(centerPoint.x, centerPoint.y, boundsRadius, 30);

	//center of bounds
	glColor3f(1, 0, 0);
	drawFilledPolygon(centerPoint.x, centerPoint.y, 5, 10);

    //only draw the line from the center to the player if the player is outside the bounds
	if(length(playerPos - centerPoint) > float(boundsRadius))
	{
		glColor3f(0, 0, 1);
		drawLine(centerPoint.x + intersectionPoint.x, centerPoint.y + intersectionPoint.y, playerPos.x, playerPos.y);
	}

	//draw player (as a triangle)
	glColor3f(0, 1, 0);
	glPushMatrix();
        //rotate around center
		glTranslatef(playerPos.x, playerPos.y, 0);
		glRotatef(playerRotation, 0.0f, 0.0f, 1.0f);
		glTranslatef(-playerPos.x, -playerPos.y, 0);
		drawFilledPolygon(playerPos.x, playerPos.y, 10, 3);
	glPopMatrix();

    glFlush();
}

void init()
{
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

void resize(int width, int height)
{
    // we ignore the params and do:
    glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Fortnite Map Logic");
	glutDisplayFunc(display);
	init();
	glutMotionFunc(motionCallBack);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutMainLoop();
}
