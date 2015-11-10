#ifndef MATERIAL_H
#define MATERIAL_H


typedef struct materialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess;
} materialStruct;

void useMaterial(GLfloat R,GLfloat G,GLfloat B,GLfloat specular,GLfloat shininess);
void useMaterialv(GLfloat *color,GLfloat specular,GLfloat shininess);

#endif