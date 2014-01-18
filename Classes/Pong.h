/*
 *  Pong.h
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OGLCommon.h"


#define BALL_SEGMENTS	15

#ifdef __i386__	
	#define BALL_SPEED		2.0
	#define PADDLE_SPEED	2.2
#else
	#define BALL_SPEED		7.0
	#define PADDLE_SPEED	8.0
#endif

typedef enum {
	PaddleDirectionUp,
	PaddleDirectionNone,
	PaddleDirectionDown,
} PaddleDirection;

typedef struct {
	GLuint width;
	GLuint height;
	GLfloat vertices[8];
	GLfloat texCoords[8];
	GLuint texName;
} Board;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat r;
	GLfloat modX;
	GLfloat modY;
	GLfloat vertices[BALL_SEGMENTS * 2];
	GLfloat texCoords[8];
} Ball;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat width;
	GLfloat height;
	GLfloat vertices[8];
	GLfloat texCoords[8];
	PaddleDirection direction;
} Paddle;

Board    board;
Paddle   paddle1;
Paddle   paddle2;
Ball     ball;

void initBoard (GLuint width, GLuint height);
void setPaddleDirection (PaddleDirection p);
void updatePaddles (void);
void updateBall (void);

void pauseGame (void);
void resumeGame (void);
bool isPaused (void);
