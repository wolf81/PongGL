//
//  OGLRenderer.m
//  Pong
//
//  Created by Wolfgang Schreurs on 1/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "OGLRenderer.h"

#include <CoreFoundation/CoreFoundation.h>
#import "OALCommon.h"


@implementation OGLRenderer

- (id)init 
{
	if (self = [super init]) {
		context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		if (context == nil || ![EAGLContext setCurrentContext:context]) {
			[self release];
			return nil;
		}
	}
	
	return self;
}

- (void)dealloc 
{	   
	[self destroyFramebuffer];
	
	if ([EAGLContext currentContext] == context) {
		[EAGLContext setCurrentContext:nil];
	}
	
	[context release];
	context = nil;
		
	[super dealloc];
}

- (BOOL)createFramebufferForLayer:(id <EAGLDrawable>)layer 
{
	glGenFramebuffersOES(1, &defaultFramebuffer);
	glGenRenderbuffersOES(1, &colorRenderbuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, colorRenderbuffer);
	
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
		return NO;
	}
	
	return YES;
}


- (void)destroyFramebuffer 
{
	oglReleaseTextures();
	
	if (defaultFramebuffer) {
		glDeleteFramebuffersOES(1, &defaultFramebuffer);
		defaultFramebuffer = 0;
	}
	
	if (colorRenderbuffer) {
		glDeleteRenderbuffersOES(1, &colorRenderbuffer);
		colorRenderbuffer = 0;
	}
	
	if (depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &depthRenderbuffer);
		depthRenderbuffer = 0;
	}	
}

- (void)render 
{	   
	[EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, defaultFramebuffer);	
	
	// custom draw code starts here ...
   
	// set the color to clear buffers (screen) with and clear buffers  
	//	glClearColor(red, green, blue); 
	glClearColor(0.0, 0.0, 0.0, 1.0);						
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	
	// activate texture support and configure blend mode (texture will not blend, color will blend if it's not opaque)
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_SRC_COLOR);
	
	// start updating vertex arrays and texture coordinate arrays ...
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// draw board 	
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glVertexPointer(2, GL_FLOAT, 0, board.vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, board.texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
   if (!isPaused())
      updatePaddles();
	
	// draw left paddle
	glColor4f(1.0, 0.8, 0.8, 0.9);						
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glVertexPointer(2, GL_FLOAT, 0, paddle1.vertices);	
	glTexCoordPointer(2, GL_FLOAT, 0, paddle1.texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);	
	
	// draw right paddle
	glColor4f(1.0, 1.0, 1.0, 0.9);						
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glVertexPointer(2, GL_FLOAT, 0, paddle2.vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, paddle2.texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
   if (!isPaused())
      updateBall();
	
	// draw ball
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glVertexPointer (2, GL_FLOAT , 0, ball.vertices); 
	glTexCoordPointer(2, GL_FLOAT, 0, ball.texCoords);
	glDrawArrays (GL_TRIANGLE_FAN, 0, BALL_SEGMENTS);
	
	// finish updating vertex arrays and texture coordinate arrays ...
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_2D);		
	
	// present drawing results 	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, colorRenderbuffer);		
    [context presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (void) setupTextureSupport {
	
	
	NSArray *texFiles = [NSArray arrayWithObjects:
						 @"floor_tiles_texture.png", 
						 @"wall_tiles_texture.png", 
						 @"p1.png",
						 @"p2.png",
						 @"green.png",
						 nil];
	
	// first check if every file exists, then load textures.
	// if by some chance a file doesn't exist, the oglCreateTexture method will fail (perhaps crash even!)
	
	oglCreateTextures((CFArrayRef) texFiles);
}

- (void)setupRendererForView:(UIView *)view 
{	
	// switch to PROJECTION matrix, subsequent actions like scaling, movement, will be done to the 'camera' ...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// rotate screen 90 degrees counterclockwise over z-axis:
	//	glRotatef(angle, x, y, z);
   glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	
	// set up orthographic projection, i.e. objects drawn further away are just as small or big as the same object closer to the 'camera'
	//	glOrtho(left coord, right coord, bottom coord, top coord, nearest to display, furthest to display);
	CGRect rect = view.bounds;
	glOrthof(-rect.size.height / 2, rect.size.height / 2, -rect.size.width / 2, rect.size.width / 2, 0.0f, 1.0f);
	
	// set up viewport, in our case viewport is just as big as the projection so pixels are mapped 1:1.
	glViewport(0, 0, rect.size.width, rect.size.height);
	
	// move camera (since we're changing the PROJECTION matrix) - camera is moved half a screen left (x-axis), half a screen down (y-axis)
	//	glTranslatef(x, y, z)
	glTranslatef(-rect.size.height / 2, -rect.size.width / 2, 0);
	
	initBoard(rect.size.height, rect.size.width);
	
	[self setupTextureSupport];
	
	// switch to MODELVIEW matrix, subsequent actions like scaling, movement, will be done to the models ...
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

@end
