//
//  PongAppDelegate.h
//  Pong
//
//  Created by Wolfgang Schreurs on 1/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

#import "OGLView.h"


@interface PongAppDelegate : NSObject <UIApplicationDelegate> {
   UIWindow *window;
	OGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

