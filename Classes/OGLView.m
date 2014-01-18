//
//  OGLView.m
//  Pong
//
//  Created by Wolfgang Schreurs on 1/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <OpenGLES/EAGLDrawable.h>

#import "OGLView.h"

@implementation OGLView

#define MAXSAMPLES 100
int tickindex=0;
int ticksum=0;
int ticklist[MAXSAMPLES];

/* need to zero out the ticklist array before starting */
/* average will ramp up until the buffer is full */
/* returns average ticks per frame over the MAXSAMPPLES last frames */

double CalcAverageTick(int newtick)
{
   ticksum-=ticklist[tickindex];  /* subtract value falling off */
   ticksum+=newtick;              /* add new value */
   ticklist[tickindex]=newtick;   /* save new value so it can be subtracted later */
   if(++tickindex==MAXSAMPLES)    /* inc buffer index */
      tickindex=0;
	
   /* return average */
   return((double)ticksum/MAXSAMPLES);
}


/*
 This method is required so we can use the drawableProperties method correctly. 
 */

+ (Class)layerClass {
   return [CAEAGLLayer class];
}

- (id)initWithFrame:(CGRect)frame {
   if ((self = [super initWithFrame:frame])) {
		CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer; 
		eaglLayer.opaque = TRUE;
      eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                      [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, 
                                      kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, 
                                      nil];
		
		renderer = [[OGLRenderer alloc] init];
		if (!renderer) {
			[self release];
			return nil;
		}
      
		[self startAnimation];
      
      theMenuView = [[MenuView alloc] initWithFrame:frame];
      NSLog(@"%@", NSStringFromCGRect(frame));
      [self addSubview:theMenuView];
   }
   
   return self;
}

- (void)drawView:(id)sender {
   /*	NSDate *currentDate = [NSDate date];
    NSTimeInterval interval = [currentDate timeIntervalSinceDate:oldDate];
    double fps = CalcAverageTick(interval * 100000);
    NSLog(@"fps: %f", fps);
    
    [oldDate release];
    oldDate = [currentDate retain];
    */	
	[renderer render];
}

- (void)layoutSubviews {
	[renderer destroyFramebuffer];
	[renderer createFramebufferForLayer:(CAEAGLLayer *)self.layer];
	[renderer setupRendererForView:self];

	[self drawView:nil];
}

- (void)startAnimation {
	animationTimer = [NSTimer scheduledTimerWithTimeInterval:1/30 target:self selector:@selector(drawView:) userInfo:nil repeats:YES];
   
	oldDate = [[NSDate date] retain];
}

- (void)stopAnimation {
	[animationTimer release];
	animationTimer = nil;
}

- (void)dealloc {
	[self stopAnimation];
   [theMenuView release];
	
   [super dealloc];
}

/*
 Schermaanrakingen worden hier afgevangen.
 */

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {	
	UITouch *touch = [touches anyObject];
   CGPoint currentTouch = [touch locationInView:self];	
   
	if (currentTouch.y > 420 && currentTouch.x < 60) {
		setPaddleDirection(PaddleDirectionUp);
	} else if (currentTouch.y > 420 && currentTouch.x > 260) {
		setPaddleDirection(PaddleDirectionDown);
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {	
	setPaddleDirection(PaddleDirectionNone);
}

@end
