#include <GL\glut.h>
#include "material.h"

void useMaterial(GLfloat R,GLfloat G,GLfloat B,GLfloat specular,GLfloat shininess)
{
	materialStruct material = {
		{0.3*R, 0.3*G, 0.3*B, 1.0},
		{0.8*R, 0.8*G, 0.8*B, 1.0},
		{specular, specular, specular, 1.0},
		shininess
	};
	glMaterialfv(GL_FRONT,GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,material.specular);
	glMaterialf(GL_FRONT,GL_SHININESS,material.shininess);
}

void useMaterialv(GLfloat *color, GLfloat specular, GLfloat shininess)
{
	useMaterial(color[0],color[1],color[2],specular,shininess);
}