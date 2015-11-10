#include <windows.h>
#include <GL\glut.h>
#include <ctime>
#include <iostream>

#include "main.h"
#include "tank.h"
#include "collision.h"
#include "draw.h"
#include "control.h"
#include "ai.h"

///////////////////////////////////////////////////////////////////////////////
// Here are environment variables that indicate the state of the game//////////
std::vector<Tank>    tanks;
std::vector<Bullet>  bullets;
std::vector<Message> messages; // Message list
std::vector<Effect>  effects; // Effect list

int wall[numberHeight][numberWidth] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,4,0,4,4,0,4,0,0},
	{0,0,4,3,0,0,3,4,0,0},
	{0,0,4,0,0,0,0,4,0,0},
	{0,0,4,0,3,3,0,4,0,0},
	{0,0,4,0,0,0,0,4,0,0},
	{0,0,0,0,4,4,0,0,0,0},
	{0,3,4,4,0,0,4,4,3,0},
	{0,0,0,3,0,0,3,0,0,0},
	{0,0,0,0,0,0,0,0,0,0}
};

Rect boundary = {{scaleWidth/2,scaleHeight/2},0,scaleWidth,scaleHeight};

int mousex;
int mousey;

time_t timer1 = 0;
time_t timer2 = 0;
time_t timer3 = 0;

int player1 = 0;
int player2 = 1;
int nPlayer = 1;

int ww, hh, offsetx, offsety;

bool smallMap = false;
bool going    = true;

///////////////////////// Basic Functions /////////////////////////////////////
void init()
{
	initScene();
	srand((unsigned)time(0));
	addFreeTank(0);
	if(nPlayer == 2) addFreeTank(1);
	addFreeTank(2);
	addFreeTank(2);
	addFreeTank(2);
	addFreeTank(2);
	
	initAI();
	addCommandLink('a',player1,turnLeft,player1);
	addCommandLink('d',player1,turnRight,player1);
	addCommandLink('w',player1,moveForward,player1);
	addCommandLink('s',player1,moveBack,player1);
	addCommandLink('A',player1,stopTurning,player1);
	addCommandLink('D',player1,stopTurning,player1);
	addCommandLink('W',player1,stopMoving,player1);
	addCommandLink('S',player1,stopMoving,player1);
	addCommandLink('q',player1,towerLeft,player1);
	addCommandLink('e',player1,towerRight,player1);
	addCommandLink('Q',player1,towerStop,player1);
	addCommandLink('E',player1,towerStop,player1);
	addCommandLink(' ',player1,fire,player1);
	
	addCommandLink('j',player2,turnLeft,player2);
	addCommandLink('l',player2,turnRight,player2);
	addCommandLink('i',player2,moveForward,player2);
	addCommandLink('k',player2,moveBack,player2);
	addCommandLink('J',player2,stopTurning,player2);
	addCommandLink('L',player2,stopTurning,player2);
	addCommandLink('I',player2,stopMoving,player2);
	addCommandLink('K',player2,stopMoving,player2);
	addCommandLink('u',player2,towerLeft,player2);
	addCommandLink('o',player2,towerRight,player2);
	addCommandLink('U',player2,towerStop,player2);
	addCommandLink('O',player2,towerStop,player2);
	addCommandLink('h',player2,fire,player2);
	
	timer1 = clock();
	timer2 = clock();
	timer3 = clock();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_LIGHTING);
	setModelViewMatrix(DRAW3D_1);
	setProjectionMatrix(ww,hh,DRAW3D_1);
	drawGround();
	drawWalls3d();
	drawTanks3d();
	drawBullets3d();
	drawEffects();
	if(nPlayer==2)
	{
		setModelViewMatrix(DRAW3D_2);
		setProjectionMatrix(ww,hh,DRAW3D_2);
		drawGround();
		drawWalls3d();
		drawTanks3d();
		drawBullets3d();
		drawEffects();
	}
	glDisable(GL_LIGHTING);
	if(smallMap)
	{
		setModelViewMatrix(DRAW_MAP);
		setProjectionMatrix(ww,hh,DRAW_MAP);
		drawWalls2d();
		drawTanks2d();
		drawBullets2d();
	}
	setModelViewMatrix(DRAW_INFO);
	setProjectionMatrix(ww,hh,DRAW_INFO);
	showHP();
	showBullet();
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	// Keep the ratio
	ww = w; hh = h;
	if((float)ww/hh > whRatio * nPlayer) ww = hh * whRatio * nPlayer;
	else hh = ww / (whRatio*nPlayer);
	offsetx = (w-ww)/2;
	offsety = (h-hh)/2;
	glutPostRedisplay();
}
///////////////////////////////////////////////////////////////////////////////

/////////////////////////// Mouse and Keyboard ////////////////////////////////

void keyboard(unsigned char keyCode, int x, int y)
{
	if(keyCode == 'm') smallMap = !smallMap;
	if(keyCode == 'p') going    = !going;
	if(keyCode == 'z') tanks.at(player1).HP = 0;
	if(going) controlTank(keyCode);
}

void keyboardUp(unsigned char keyCode, int x, int y)
{
	if(going)
	{
		if('a'<=keyCode && keyCode <= 'z')
			controlTank(toupper(keyCode));
	}
}

void mouse(int btn, int state, int x, int y)
{
	if(btn == GLUT_LEFT_BUTTON) // Left mouse
	{
		controlTank((fire<<8)|1);
	}
	else if(btn == GLUT_RIGHT_BUTTON) // Right mouse
	{
	}
	else // Middle button
	{
	}
	mousex = x;
	mousey = y;
}

// Mouse move with button down
void motion(int x, int y)
{
	mousex = x;
	mousey = y;
}

// Mouse move with button up
void passiveMotion(int x, int y)
{
	/*
	if(x < ww/6) controlTank('q');
	else if(x < ww/3) controlTank('Q');
	else controlTank('e');
	*/
	mousex = x;
	mousey = y;
}

///////////////////// Timer ///////////////////////////////////////////////////
void idle()
{
	// Count the length of time passed
	int dt = clock() - timer1;
	while(dt > minInterval)
	{
		timer1 = clock();
		// Move on everything
		moveon(dt);
		dt -= minInterval;
	}
	
	dt = clock() - timer2;
	while(dt > aiInterval)
	{
		timer2 = clock();
		if(going)
			for(int i = nPlayer; i < tanks.size(); i++)
			{
				if(!aiAutoAim(i,player1))
				{
					if(nPlayer==2)
					{
						if(!aiAutoAim(i,player2))
							aiRandom(i);
					}
					else
						aiRandom(i);
				}
				
			}
		dt -= aiInterval;
	}
	
	dt = clock() - timer3;
	while(dt > addInterval)
	{
		timer3 = clock();
		if(going)
		{
			bool norevive = true;
			for(int i = 1; i < tanks.size(); i++)
			{
				if(tanks.at(i).HP <= 0)
				{
					reviveTank(i);
					norevive = false;
					break;
				}
			}
			//if(norevive) addFreeTank(2);
		}
		dt -= addInterval;
	}
	
	if(tanks.at(player1).HP <= 0)
	{
		reviveTank(player1);
		addFlickerEffect(0);
	}
	if(nPlayer == 2)
	{
		if(tanks.at(player2).HP <= 0)
		{
			reviveTank(player2);
			addFlickerEffect(1);
		}
	}
	
	glutPostRedisplay();
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500*nPlayer,500);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Tank");

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutIdleFunc(idle);
	glutMainLoop();
}