//
//  MenuView.m
//  Pong
//
//  Created by Wolfgang Schreurs on 2/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MenuView.h"
#import "Pong.h"


@implementation MenuView


- (id)initWithFrame:(CGRect)frame {    
   self = [super initWithFrame:frame];
   if (self) {    
      CGAffineTransform landscapeTransform = CGAffineTransformMakeRotation(M_PI * 1.5);
      landscapeTransform = CGAffineTransformTranslate (landscapeTransform, 0.0, 0.0);
            
      CGRect appFrame = [[UIScreen mainScreen] applicationFrame];

      startButton = [[UIButton alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 80.0f, 30.0f)];
      [startButton setBackgroundColor:[UIColor blackColor]];
      [startButton setTitle:@"Play" forState:UIControlStateNormal];
      [startButton.titleLabel setTextColor:[UIColor whiteColor]];
      [startButton setCenter:CGPointMake(appFrame.size.width / 2, appFrame.size.height / 2)];
      [startButton setTransform:landscapeTransform];
      [startButton addTarget:self action:@selector(startClicked:) forControlEvents:UIControlEventTouchUpInside];
      [startButton setHidden:NO];
      [self addSubview:startButton];
      
      pauseButton = [[UIButton alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 30.0f, 30.0f)];
      [pauseButton setBackgroundColor:[UIColor blackColor]];
      [pauseButton setTitle:@"P" forState:UIControlStateNormal];
      [pauseButton.titleLabel setTextColor:[UIColor whiteColor]];
      [pauseButton setCenter:CGPointMake(appFrame.size.width - 20.0f, 20.0f)];
      [pauseButton setTransform:landscapeTransform];
      [pauseButton addTarget:self action:@selector(pauseClicked:) forControlEvents:UIControlEventTouchUpInside];
      [pauseButton setHidden:YES];
      [self addSubview:pauseButton];
   }
   return self;
}

/*
 // Only override drawRect: if you perform custom drawing.
 // An empty implementation adversely affects performance during animation.
 - (void)drawRect:(CGRect)rect {
 // Drawing code.
 }
 */

- (void)dealloc {
   [startButton release];
   [pauseButton release];
   
   [super dealloc];
}

- (void)startClicked:(id)sender 
{
   [UIView beginAnimations:nil context:NULL];
   [UIView setAnimationDuration:0.5f];
   [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:startButton cache:NO];   
   [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromLeft forView:pauseButton cache:NO];   
   [startButton setHidden:YES];
   [pauseButton setHidden:NO];
   [UIView commitAnimations];

   resumeGame();
}

- (void)pauseClicked:(id)sender 
{
   [UIView beginAnimations:nil context:NULL];
   [UIView setAnimationDuration:0.5f];
   [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight forView:pauseButton cache:NO];
   [UIView setAnimationTransition:UIViewAnimationTransitionFlipFromRight forView:startButton cache:NO];
   [pauseButton setHidden:YES];
   [startButton setHidden:NO];
   [UIView commitAnimations];

   pauseGame();
}

@end
