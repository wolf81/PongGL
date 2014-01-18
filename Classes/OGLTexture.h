/*
 *  OGLTexture.h
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#import "OGLCommon.h"

GLuint *textures;

void oglCreateTextures (CFArrayRef files);
void oglReleaseTextures (void);
void oglMapTextureCoords (GLfloat vertices[], GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);