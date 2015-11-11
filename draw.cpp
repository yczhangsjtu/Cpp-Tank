#include <cassert>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-master/stb_image.h>
#include "3ds.h"
#include "draw.h"
#include "material.h"
#include "effects.h"

#define TANK_BODY  "3ds/tankbody.3ds"
#define TANK_TOWER "3ds/tanktower.3ds"
#define BULLET     "3ds/bullet.3ds"

extern std::vector<Tank>   tanks;
extern std::vector<Bullet> bullets;
extern std::vector<Message> messages;
extern std::vector<Effect> effects;
extern int wall[numberHeight][numberWidth];
extern Rect boundary;
extern int player1;
extern int player2;
extern int offsetx, offsety;
extern int nPlayer;
extern bool smallMap;

const char *texPath = "tex/";

const char *groundTexFiles[] = {"tex/ground1.jpg"};
const char *fireTexFiles[] = {"tex/fire.jpg"};
const char *wallTexFiles[] = {
	"tex/wall1.jpg", "tex/wall2.jpg",
	"tex/wall3.jpg", "tex/wall4.jpg"
};

Model tankbody, tanktower, bullet;

GLuint groundTex, fireTex, wallTex[4];
#if 0
const int groundTextureSize = 512;
GLubyte groundTexture[groundTextureSize][groundTextureSize][3];
const int fireTextureSize = 128;
GLubyte fireTexture[fireTextureSize][fireTextureSize][3];
const int wallTextureSize = 256;
GLubyte wallTexture[4][wallTextureSize][wallTextureSize][3];
#endif

float colorTable[][3] = {{0,0,1},{0,1,1},{1,0,0},{0,1,0},
						 {1,0,1},{1,1,0},{1,1,1},{0,0,0}};
float deadColor[][3] = {{0.1,0.1,0.1},{0.5,0.5,0.5}};

void itoa(int i, char *buf)
{
	sprintf(buf,"%d",i);
}

void load_model()
{
	tankbody.loadFile(TANK_BODY);
	tanktower.loadFile(TANK_TOWER);
	bullet.loadFile(BULLET);
}

void load_textures()
{
	int x,y,n;
	unsigned char *data = stbi_load(groundTexFiles[0], &x, &y, &n, 0);
	if(data==NULL)
	{
		fprintf(stdout,"Can't load texture file!");
		exit(1);
	}
	glGenTextures(1,&groundTex);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, data); // genereate MipMap levels for our texture 

	data = stbi_load(fireTexFiles[0], &x, &y, &n, 0);
	if(data==NULL)
	{
		fprintf(stdout,"Can't load texture file!");
		exit(1);
	}
	glGenTextures(1,&fireTex);
	glBindTexture(GL_TEXTURE_2D, fireTex);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, data); // genereate MipMap levels for our texture 

	for(int k = 0; k < 4; k++)
	{
		data = stbi_load(wallTexFiles[k], &x, &y, &n, 0);
		if(data==NULL)
		{
			fprintf(stdout,"Can't load texture file!");
			exit(1);
		}
		glGenTextures(1,&wallTex[k]);
		glBindTexture(GL_TEXTURE_2D, wallTex[k]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, data); // genereate MipMap levels for our texture 
	}
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
}

// Initialize the scene
void initScene()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glClearColor(0,0,0,1); // Set background color to be black
	
	// Initialize the light
	GLfloat light0_pos[4]  = {0.0, 0.0, 200.0, 1.0};
	GLfloat light0_dir[4]  = {0.0, 0.0, -200.0, 1.0};
	GLfloat light0_dif[4]  = {1.0,1.0,1.0,1.0};
	GLfloat light0_amb[4]  = {0.0,0.0,0.0,1.0};
	GLfloat light0_spec[4] = {1.0,1.0,1.0,1.0};
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_spec);
	glLightfv(GL_LIGHT0, GL_POSITION,light0_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,light0_dif);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION,light0_dir);

	load_model();
	load_textures();
}

void setModelViewMatrix(DrawMode mode)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(mode == DRAW3D_1 || mode == DRAW3D_2)
	{
		int from;
		// Get the position of my tank
		if(mode == DRAW3D_1) from = player1;
		else from = player2;
		
		GLfloat *p = tanks.at(from).rect.pos;
		GLfloat a = tanks.at(from).rect.angle;
		GLfloat h = eyeHeight;
		GLfloat d = eyeDist;
		GLfloat f[3] = {p[0]-20*d*cos(a),p[1]-20*d*sin(a),h};
		GLfloat t[3] = {p[0]+d*cos(a),p[1]+d*sin(a),0.0};
		gluLookAt( f[0],f[1],f[2], t[0],t[1],t[2], 0,0,1 );
	}
}

void setProjectionMatrix(int w, int h, DrawMode mode)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 3D
	if(mode == DRAW3D_1)
	{
		glViewport(offsetx, offsety, w/nPlayer, h*(1-mapRatio));
		glFrustum(-wallSize/8,wallSize/8,
				  0,wallSize/8*(1-mapRatio),
				  scaleNear,scaleFar);
	}
	else if(mode == DRAW3D_2)
	{
		glViewport(offsetx+w/nPlayer+10, offsety, w/nPlayer, h*(1-mapRatio));
		glFrustum(-wallSize/8,wallSize/8,
				  0,wallSize/8*(1-mapRatio),
				  scaleNear,scaleFar);
	}
	else if(mode == DRAW_MAP) // 2D
	{
		glViewport (w/nPlayer*(nPlayer-mapRatio)+offsetx,
					h*(1-mapRatio)+offsety,
					w/nPlayer*mapRatio, h*mapRatio);
		gluOrtho2D (0.0, scaleWidth, 0.0, scaleHeight);
	}
	else if(mode ==DRAW_INFO)
	{
		glViewport (0, h*(1-mapRatio)+offsety,
					w/nPlayer*(nPlayer-mapRatio), h*mapRatio);
		gluOrtho2D (0.0, 100*(nPlayer-mapRatio), 0.0, 100*mapRatio);
	}
}

// 2D Drawing functions --------------------------------------------------------
// Draw rectangle
void drawRect2d(const Rect & rc, float dh)
{
	float v[4][2];
	getVertices(rc,v);
	glBegin(GL_POLYGON);
		glVertex3f(v[0][0],v[0][1],dh);
		glVertex3f(v[1][0],v[1][1],dh);
		glVertex3f(v[2][0],v[2][1],dh);
		glVertex3f(v[3][0],v[3][1],dh);
	glEnd();
}

// Draw a single tank
void drawTank2d(const Tank & tank)
{
	if(tank.HP <= 0) return;// if it's dead
	// Draw the body
	glColor3fv(colorTable[tank.commander]);
	drawRect2d(tank.rect,0.1);
	// Draw the tower
	float x = tank.rect.pos[0];
	float y = tank.rect.pos[1];
	float a = tank.tower.angle;
	float w = 3.0, l = tankLength;
	Rect barrel = createRect(x,y,a,l,w);
	moveAlongWidth(barrel,tankLength/3);
	glColor3fv(colorTable[0]);
	drawRect2d(barrel,0.2);
	Rect tower = createRect(x,y,a,tankLength/2,tankWidth/2);
	glColor3fv(colorTable[5]);
	drawRect2d(tower,0.3);
}

// Draw all tanks
void drawTanks2d()
{
	for(int i = 0; i < tanks.size(); i++)
		drawTank2d(tanks.at(i));
}

// Draw a single bullet
void drawBullet2d(const Bullet & bullet)
{
	glBegin(GL_POINTS);
		glVertex2fv(bullet.pos);
	glEnd();
}

// Draw all the bullets
void drawBullets2d()
{
	glColor3f(0,0,0);
	glPointSize(3);
	for(int i = 0; i < bullets.size(); i++)
		drawBullet2d(bullets.at(i));
}

// Draw a piece of wall
void drawWall2d(int i, int j)
{
	assert(i>=0 && i<numberHeight && j>=0 && j<numberWidth);

	Rect wRc = getWallRect(i,j); // Get the rectangle of wall
	// Set color of the wall based on the value of wall
	glColor3fv(colorTable[wall[i][j]+3]);
	drawRect2d(wRc);
}

// Draw all wall pieces
void drawWalls2d()
{
	for(int i = 0; i < numberHeight; i++)
		for(int j = 0; j < numberWidth; j++)
			drawWall2d(i,j);
}

// Draw heart
void drawHeart(GLfloat x, GLfloat y, GLfloat size)
{
	float sq3 = sqrt(3.0);
	glColor3f(1.0,0.0,0.0); // Red
	glPolygonMode(GL_FRONT, GL_FILL);
	// Draw two circles
	glBegin(GL_POLYGON);
		for(int i = 0; i < 10; i++)
		{
			float t = 2*i*M_PI/10;
			glVertex2f(x-size+size*cos(t),y+size*sin(t));
		}
	glEnd();
	glBegin(GL_POLYGON);
		for(int i = 0; i < 10; i++)
		{
			float t = 2*i*M_PI/10;
			glVertex2f(x+size+size*cos(t),y+size*sin(t));
		}
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2f(x,y);
		glVertex2f(x-size*1.5,y-size/2.0*sq3);
		glVertex2f(x,y-size*sq3);
		glVertex2f(x+size*1.5,y-size/2.0*sq3);
	glEnd();
}

void drawChar(char c, GLfloat x, GLfloat y, GLfloat *color, GLfloat size)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(x,y,0);
	GLfloat ratio = size/glutStrokeWidth(GLUT_STROKE_MONO_ROMAN,c);
	glScalef(ratio,ratio,1);
	glColor3fv(color);
	glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN,c);
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void drawString(GLfloat x, GLfloat y, GLfloat size,
				const char *str, GLfloat *color, int n)
{
	GLfloat curx, cury;
	curx = x; cury = y;
	for(int i = 0; i<n; i++, curx+=size)
	{
		drawChar(str[i],curx,cury,color,size);
		if(str[i]=='\n')
		{
			cury-= 2*size;
			curx = x-size;
		}
	}
}

// Show HP
void showHP()
{
	GLfloat color[3] = {1.0,1.0,1.0};
	drawString(0,15,5,"HP:",color,3);
	for(int i = 0; i < tanks.at(player1).HP; i++)
		drawHeart(i*10+20,17,2);
	if(nPlayer==2)
	{
		drawString(100,15,5,"HP:",color,3);
		for(int i = 0; i < tanks.at(player2).HP; i++)
			drawHeart(i*10+20+100,17,2);
	}
}

void showBullet()
{
	GLfloat color[3] = {1.0,1.0,1.0};
	char buffer[25] = "BULLET: ";
	itoa(tanks.at(player1).tower.bullet,buffer+strlen(buffer));
	drawString(0,5,5,buffer,color,strlen(buffer));
	if(nPlayer==2)
	{
		buffer[8]='\0';
		itoa(tanks.at(player2).tower.bullet,buffer+strlen(buffer));
		drawString(100,5,5,buffer,color,strlen(buffer));
	}
}

////////////////////////////////////////////////////////////////////////////////

// 3D Drawing functions---------------------------------------------------------
void drawGround()
{
	GLfloat v[4][3] = {{0,0,0},{scaleWidth,0,0},
					   {scaleWidth,scaleHeight,0},
					   {0,scaleHeight,0}};
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,groundTex);
	useMaterial(1.0,1.0,1.0,1.0,1.0);
	glNormal3f(0,0,1);
	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0,0.0);
		glVertex3fv(v[0]);
		glTexCoord2f(0.0,1.0);
		glVertex3fv(v[1]);
		glTexCoord2f(1.0,1.0);
		glVertex3fv(v[2]);
		glTexCoord2f(1.0,0.0);
		glVertex3fv(v[3]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawRect3d(const Rect & rc, float height, int k)
{
	float v[4][2];
	getVertices(rc,v);
	if(glIsEnabled(GL_LIGHTING))
		useMaterial(1.0,1.0,1.0,1.0,1.0);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,wallTex[k]);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glNormal3f(0,0,1);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0);
		glVertex3f(v[0][0],v[0][1],height);
		glTexCoord2f(0.0,1.0);
		glVertex3f(v[1][0],v[1][1],height);
		glTexCoord2f(1.0,1.0);
		glVertex3f(v[2][0],v[2][1],height);
		glTexCoord2f(1.0,0.0);
		glVertex3f(v[3][0],v[3][1],height);
	glEnd();
	
	glNormal3f(v[0][0]-v[3][0],v[0][1]-v[3][1],0);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0);
		glVertex3f(v[0][0],v[0][1],0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(v[1][0],v[1][1],0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(v[1][0],v[1][1],height);
		glTexCoord2f(1.0,0.0);
		glVertex3f(v[0][0],v[0][1],height);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glNormal3f(v[1][0]-v[0][0],v[1][1]-v[0][1],0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0);
		glVertex3f(v[1][0],v[1][1],0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(v[2][0],v[2][1],0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(v[2][0],v[2][1],height);
		glTexCoord2f(1.0,0.0);
		glVertex3f(v[1][0],v[1][1],height);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glNormal3f(v[2][0]-v[1][0],v[2][1]-v[1][1],0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0);
		glVertex3f(v[2][0],v[2][1],0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(v[3][0],v[3][1],0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(v[3][0],v[3][1],height);
		glTexCoord2f(1.0,0.0);
		glVertex3f(v[2][0],v[2][1],height);
	glEnd();
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glNormal3f(v[3][0]-v[2][0],v[3][1]-v[2][1],0);
	glBegin(GL_POLYGON);
		glTexCoord2f(0.0,0.0);
		glVertex3f(v[3][0],v[3][1],0);
		glTexCoord2f(0.0,1.0);
		glVertex3f(v[0][0],v[0][1],0);
		glTexCoord2f(1.0,1.0);
		glVertex3f(v[0][0],v[0][1],height);
		glTexCoord2f(1.0,0.0);
		glVertex3f(v[3][0],v[3][1],height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

// Draw a piece of wall
void drawWall3d(int i, int j)
{
	assert(i>=0 && i<numberHeight && j>=0 && j<numberWidth);

	Rect wRc = getWallRect(i,j); // Get the rectangle of wall
	// Set color of the wall based on the value of wall
	glColor3fv(colorTable[wall[i][j]+3]);
	if(wall[i][j])
		drawRect3d(wRc,wallHeight,wall[i][j]-1);
}

// Draw all wall pieces
void drawWalls3d()
{
	for(int i = 0; i < numberHeight; i++)
		for(int j = 0; j < numberWidth; j++)
			drawWall3d(i,j);
}

void drawTank3d(const Tank & tank, int idx)
{
	if(tank.HP <= 0) return;// if it's dead
	// Draw the body
	GLfloat *color = colorTable[tank.commander];
	//GLfloat color[3] = {1,1,1};
	int effid;
	if((effid=findEffectI(effShine, idx))>=0)
	{
		Effect eff = effects.at(effid);
		if(((eff.j/10)%2)==0)
			useMaterialv(color,1.0,1.0);
		else
			useMaterialv(color,0.2,0.2);
	}
	else if((effid=findEffectI(effFlicker, idx))>=0 )
	{
		Effect eff = effects.at(effid);
		if(((eff.j/200)%2)==0)
		{
			return;
		}
		else
			useMaterialv(color,0.2,0.2);
	}
	else
		useMaterialv(color,0.2,0.2);
	
	const GLfloat *p = tank.rect.pos;
	GLfloat a1 = tank.rect.angle;
	GLfloat a2 = tank.tower.angle;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(p[0],p[1],0.0);
	glScalef(0.3,0.3,0.3);
	glTranslatef(30.0,1.0,0.0);
	glRotatef(90+a1/M_PI*180,0,0,1);
	glTranslatef(-30.0,-1.0,0.0);
	tankbody.RenderModel();
	glPopMatrix();
	
	// Draw the tower
	glPushMatrix();
	glTranslatef(p[0],p[1],0.0);
	glScalef(0.3,0.3,0.3);
	glTranslatef(30.0,1.0,0.0);
	glRotatef(90+a2/M_PI*180,0,0,1);
	glTranslatef(-30.0,-1.0,0.0);
	tanktower.RenderModel();
	glPopMatrix();
}

void drawTanks3d()
{
	for(int i = 0; i < tanks.size(); i++)
		drawTank3d(tanks.at(i),i);
}

// Draw a single bullet
void drawBullet3d(const Bullet & blt)
{
	GLfloat a = blt.angle;
	const GLfloat *p= blt.pos;
	glPushMatrix();
	glTranslatef(p[0],p[1],tankHeight);
	glScalef(0.05,0.05,0.05);
	glRotatef(a/M_PI*180,0,0,1);
	useMaterial(1.0,1.0,1.0,1.0,1.0);
	bullet.RenderModel();
	glPopMatrix();
}

// Draw all the bullets
void drawBullets3d()
{
	glColor3f(0,0,0);
	glPointSize(3);
	for(int i = 0; i < bullets.size(); i++)
		drawBullet3d(bullets.at(i));
}

// Draw special effects
void drawEffects()
{
	for(int i = 0; i < effects.size(); i++)
	{
		Effect eff = effects.at(i);
		switch(eff.type)
		{
		case effExplosion:
		{
			glDisable(GL_LIGHTING);
			float r = eff.j/10.0;
			// Draw random particles
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glTranslatef(eff.x,eff.y,eff.z);
			//useMaterial(1.0,0.8,0.0,0.7,0.7);
			glColor3f(1.0,1.0,1.0);
			glEnable(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D,fireTex);

			//useMaterial(1.0,1.0,1.0,1.0,1.0);
			for(int k = 0; k < 30; k++)
			{
				float t = 2 * M_PI * rand() / RAND_MAX;
				float p = M_PI * rand() / RAND_MAX - M_PI/2;
				float x = r * cos(t) * cos(p);
				float y = r * sin(t) * cos(p);
				float z = r * sin(p);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0,0.0);
					glVertex3f(0.0,2.0,0.0);
					glTexCoord2f(0.0,1.0);
					glVertex3f(x-2.0,y,z);
					glTexCoord2f(1.0,1.0);
					glVertex3f(0.0,-2.0,0.0);
					glTexCoord2f(1.0,0.0);
					glVertex3f(x+2.0,y,z);
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			glEnable(GL_LIGHTING);
		}
		break;
		}
	}
}
