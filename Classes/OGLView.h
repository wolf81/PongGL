//
//  OGLView.h
//  Pong
//
//  Created by Wolfgang Schreurs on 1/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "OGLRenderer.h"
#import "MenuView.h"


@interface OGLView : UIView {
	OGLRenderer *renderer;
	
	NSTimer *animationTimer;
	NSDate *oldDate;   
   MenuView *theMenuView;
}

- (void)startAnimation;
- (void)stopAnimation;

@end
