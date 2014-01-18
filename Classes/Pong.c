/*
 *  Pong.c
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <stdbool.h>

#include "Pong.h"
#include "OALCommon.h"
#include "mplayer.h"

bool _isPaused = true;

void initSound (void) 
{
   playSong("zabumba");
   
   oalInit();
   
   char *files[] = {
      "beep.caf", 
      "click1.caf", 
      "click2.caf",
      "click3.caf",
      "click4.caf",
      NULL
   };
   
   oalBufferAudioFiles(files, 5);
}

void initPaddles (void) 
{
	GLfloat width = board.width / 48;
	GLfloat height = board.height / 4;
	GLfloat paddle_spacing = board.height / 6;
	GLfloat y = (board.height - height) / 2;
   
	paddle1.width = width;
	paddle1.height = height;
	paddle1.x = 0 + paddle_spacing;
	paddle1.y = y;
	paddle1.direction = PaddleDirectionNone;
	oglRectVertices(paddle1.vertices, paddle1.x, paddle1.y, paddle1.width, paddle1.height);
	oglMapTextureCoords(paddle1.texCoords, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0);
	
	paddle2.width = width;
	paddle2.height = height;
	paddle2.x = board.width - paddle_spacing - width;
	paddle2.y = y;
	paddle2.direction = PaddleDirectionNone;
	oglRectVertices(paddle2.vertices, paddle2.x, paddle2.y, paddle2.width, paddle2.height);
	oglMapTextureCoords(paddle2.texCoords, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0);
}

void initBall (void) 
{	
   GLfloat angle = rand() % 150 + 105;
	ball.modX = cos(OGL_DegreesToRadials(angle)) * BALL_SPEED; // calc. horizontal speed
	ball.modY = sin(OGL_DegreesToRadials(angle)) * BALL_SPEED; // calc. vertical speed
   
	ball.x = board.width / 2;
	ball.y = board.height / 2;
	ball.r = 8.0; //board.width / 70;
	oglCircleVertices(ball.vertices, BALL_SEGMENTS, ball.x, ball.y, ball.r);	
	ball.x = ball.x - ball.r; // set x value to center of ball - by default it's on the right edge
	
	oglMapTextureCoords(ball.texCoords, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 0.0);
}

void initBoard (GLuint width, GLuint height) 
{
   initSound();
   
	board.height = height - 20.0, board.width = width;
	oglRectVertices(board.vertices, 0.0, 0.0, board.width, board.height);
	oglMapTextureCoords(board.texCoords, 0.0, 0.76, 1.0, 0.76, 0.0, 0.0, 1.0, 0.0);
	
	initPaddles();
	initBall();
}

void setPaddleDirection (PaddleDirection p) 
{
	paddle1.direction = p;
}

bool ballHitPaddle (void) 
{
	GLfloat halfWidth;
	
	halfWidth = 0.5 * paddle1.width;
	if (ball.x <= paddle1.x + halfWidth && ball.x >= paddle1.x - halfWidth) {
		if (ball.y <= paddle1.y + paddle1.height && ball.y >= paddle1.y) {
			ball.x += ball.x - paddle1.x; // place ball outside paddle 
         oalPlaySound("click2.caf");
			return true;
		}
	}
   
	halfWidth = 0.5 * paddle2.width;
	if (ball.x >= paddle2.x - halfWidth && ball.x <= paddle2.x + halfWidth) {		
		if (ball.y <= paddle2.y + paddle2.height && ball.y >= paddle2.y) {
			ball.x += ball.x - paddle2.x; // place ball outside paddle
         oalPlaySound("click2.caf");
			return true;
		}				
	}
	
	return false;
}

bool ballHitWall (void) 
{
	if (ball.y + ball.r >= board.height || ball.y - ball.r <= 0) {
      oalPlaySound("beep.caf");
      
      return true;
   }
	
	return false;	
}

bool ballExitsField (void) 
{
	if (ball.x < 0 || ball.x - ball.r > board.width) {
		return true;
	}
	
	return false;
}

void updateBall (void) 
{	   
	if (ballHitPaddle() == true) {
		ball.modX = -ball.modX;
	} else if (ballHitWall() == true) {
		ball.modY = -ball.modY;		
	} else if (ballExitsField() == true) {
		initBall(); 		
	}
   
	// update ball vertices and positioning
	oglCircleAdjustXY(ball.vertices, BALL_SEGMENTS, ball.modX, ball.modY);
	ball.x += ball.modX;
	ball.y += ball.modY;
}

Paddle updatePaddle(Paddle paddle) 
{   
	// update vertices for movement ...
	switch (paddle.direction) {
		case PaddleDirectionUp:		
			if (paddle.vertices[7] < board.height) {
				oglRectAdjustY(paddle.vertices, PADDLE_SPEED);		
				paddle.y += PADDLE_SPEED;
			}
			break;
		case PaddleDirectionDown:	
			if (paddle.vertices[1] > 0) {
				oglRectAdjustY(paddle.vertices, -PADDLE_SPEED); 
				paddle.y -= PADDLE_SPEED;
			}
			break;
		default: 
			break;
	}
	
	return paddle;
}

void updatePaddles (void) 
{  
	// calculate cpu movement ...
	if (ball.modX > 0) {
		if (paddle2.y + 0.5 * paddle2.height > ball.y) paddle2.direction = PaddleDirectionDown;
		else if (paddle2.y + 0.5 * paddle2.height < ball.y) paddle2.direction = PaddleDirectionUp;
		else paddle2.direction = PaddleDirectionNone;
	} else {		
		if (paddle2.y + 0.5 * paddle2.height < board.height / 2) paddle2.direction = PaddleDirectionUp;
		else if (paddle2.y + 0.5 * paddle2.height > board.height / 2) paddle2.direction = PaddleDirectionDown;
		else paddle2.direction = PaddleDirectionNone;
	}
	
	paddle1 = updatePaddle(paddle1);
	paddle2 = updatePaddle(paddle2);	
}

void pauseGame (void) 
{
   _isPaused = true;
}

void resumeGame (void)
{
   _isPaused = false;
}

bool isPaused (void) 
{
   return _isPaused;   
}
