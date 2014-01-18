/*
 *  OGLCommon.c
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OGLCommon.h"

void oglRectAdjustY (GLfloat *vertices, GLfloat modY)
{
	vertices[1] += modY;
	vertices[3] += modY;
	vertices[5] += modY;
	vertices[7] += modY;	
}

void oglRectVertices (GLfloat *vertices, GLfloat x, GLfloat y, GLfloat width, GLfloat height) 
{
	GLfloat x2 = x + width;
	GLfloat y2 = y + height;
	
	vertices[0] = x;
	vertices[1] = y;
	vertices[2] = x2;
	vertices[3] = y;
	vertices[4] = x;
	vertices[5] = y2;
	vertices[6] = x2;
	vertices[7] = y2;
}

void oglCircleAdjustXY (GLfloat *vertices, GLuint segments, GLfloat modX, GLfloat modY) 
{
	for (int i = 0; i < (segments * 2); ) {
		vertices[i++] += modX;
		vertices[i++] += modY;
	}	
}

void oglCircleVertices (GLfloat *vertices, GLuint segments, GLfloat x, GLfloat y, GLfloat r) 
{
	int count = 0;
	for (GLfloat i = 0; i < 360.0f; i+= (360.0f / segments)) 
	{
		vertices[count++] = (cos(OGL_DegreesToRadials(i))) * r + x;
		vertices[count++] = (sin(OGL_DegreesToRadials(i))) * r + y;
	}
}