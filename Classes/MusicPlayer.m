//
//  MusicPlayer.m
//  Pong
//
//  Created by Wolfgang Schreurs on 2/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "MusicPlayer.h"
#import "mplayer.h"


static MusicPlayer *sharedMusicPlayer = nil;

#pragma mark C methods

void playSong (char *name)
{
   NSString *aName = [NSString stringWithCString:name encoding:NSUTF8StringEncoding];
   [sharedMusicPlayer playSong:aName];  
}

#pragma mark -

@implementation MusicPlayer

- (void)playSong:(NSString *)name {
   NSURL *url = [[NSBundle mainBundle] URLForResource:name withExtension:@"mp3"];   
   if (url && !thePlayer) 
   {
      NSError *error = nil;
      thePlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];

      if (error) 
      {
         NSLog(@"%@", error);   
         return;
      }
      
      [thePlayer setDelegate:self];
      [thePlayer setNumberOfLoops:-1]; // infinite ...
                                       //      [thePlayer play];
   }
}

#pragma mark -
#pragma mark AVAudioPlayerDelegate

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {}
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag {}
- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player {}
- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player {}
- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player withFlags:(NSUInteger)flags {}

#pragma mark -
#pragma mark Singleton methods

+ (MusicPlayer *)sharedMusicPlayer
{
   @synchronized(self)
   {
      if (sharedMusicPlayer == nil) 
      {
         sharedMusicPlayer = [[MusicPlayer alloc] init];
      }
   }
   return sharedMusicPlayer;
}

+ (id)allocWithZone:(NSZone *)zone 
{
   @synchronized(self) 
   {
      if (sharedMusicPlayer == nil) 
      {
         sharedMusicPlayer = [super allocWithZone:zone];
         return sharedMusicPlayer;
      }
   }
   return nil; 
}

- (id)copyWithZone:(NSZone *)zone {return self;}
- (id)retain {return self;}
- (NSUInteger)retainCount {return UINT_MAX;}
- (void)release {}
- (id)autorelease {return self;}

@end
