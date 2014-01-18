/*
 *  OGLCommon.h
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <OpenGLES/ES1/gl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "OGLTexture.h"


#define OGL_DegreesToRadials(__ANGLE__)		( __ANGLE__ * M_PI / 180 )


typedef struct {
	GLfloat x;
	GLfloat y;
} oglPoint2D;

void oglRectAdjustY (GLfloat *vertices, GLfloat modY);
void oglRectVertices (GLfloat *vertices, GLfloat x, GLfloat y, GLfloat width, GLfloat height);
void oglCircleAdjustXY (GLfloat *vertices, GLuint segments, GLfloat modX, GLfloat modY);
void oglCircleVertices (GLfloat *vertices, GLuint segments, GLfloat x, GLfloat y, GLfloat r);
