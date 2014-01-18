//
//  OGLRenderer.h
//  Pong
//
//  Created by Wolfgang Schreurs on 1/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/EAGLDrawable.h>
#import <QuartzCore/QuartzCore.h>

#import "Pong.h"


@interface OGLRenderer : NSObject {
	EAGLContext *context;
	
	GLint backingWidth, backingHeight;
	GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;
}

- (void)render;
- (void)setupRendererForView:(UIView *)view;
- (BOOL)createFramebufferForLayer:(id <EAGLDrawable>)layer;
- (void)destroyFramebuffer;

@end
