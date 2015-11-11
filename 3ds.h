/* 
Program name:Combat of Death
Description header file: this class loads 3ds models
    Copyright (C) 2005  Hylke Donker
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
 
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
 
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/
 
/**
\file 3DS.h
\brief The 3ds model loader
 
All models are loaded and renderd using this class
*/
 
#ifndef HEADER_3DS
#define HEADER_3DS
 
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <GL/freeglut.h>
#include <lib3ds/file.h>
#include <lib3ds/node.h>
#include <lib3ds/mesh.h>
#include <lib3ds/vector.h>
#include <lib3ds/matrix.h>
#include <lib3ds/material.h>
#include <lib3ds/light.h>
 
/**
\brief 3DS model loader and render
 
This class can load a model, and then apply a texture on it to finally render it
*/
class Model
{
public:
/**
\brief Constructor of this class
 
Model() is the constructor of this class, and enables texture generation and sets how that should be done, and sets the current frame variable to 0
*/
	Model();
/**
\brief Destructor of this class
 
The destructor disables texture generation mode and free's the memory of the model, if it's not already freed.
*/
	~Model();
/**
\brief Loads the file
 
It loads the file 'name', sets the current frame to 0 and if the model has textures, it will be applied to the model
 
\param name Contains the name of the to be loaded file
*/
	void loadFile(const char *name);
/**
\brief create lighting list
 
Creates a display list that sets all the information for the lights.
*/
	void CreateLightList();
/**
\brief renders nodes
 
It renders the node specified by the argument and sets material properties to the node if nescesary
 
\param node Contains the node to be renderd
*/
	void renderNode(Lib3dsNode *node);
/**
\brief Enable lights
 
Enables all lights in the model if not already enabled
*/
	void EnableLights();
/**
\brief Enable lights
 
Disables all lights in the model if not already disabled
*/
	void DisableLights();
/**
\brief Actually renders the model
 
It renders the model, by rendering node by node using the renderNode function.But before it's renderd it's translated to (x,y,z) and then rotates it angle degrees
 
\sa renderNode()
*/
	void RenderModel();
/**
\brief Apply the texture
 
It applies a texture to mesh ,according to the data that mesh contains
 
\param mesh That's the mesh on which the texture should be applied
*/
	void ApplyTexture(Lib3dsMesh *mesh);
	Lib3dsFile * get3DSPointer();
	std::string getFilename();
private:
	Lib3dsFile *file; /**< file holds the data of the model */
	const char *filename; /**< It's the filename of the model */
	int curFrame; /**< curFrame keeps track of the current frame that shuold be renderd */
	std::vector<GLuint> textureIndices; /**< this variable holds the texture indicies */
	std::vector<std::string> textureFilenames; /**< Holds the filenames of the textures, so I can see wheter a texture is used multiple times or not */
	bool lightEnabled; /**< wheter light was enabled before this class. */
	GLuint lightListIndex;
};
 
#endif
