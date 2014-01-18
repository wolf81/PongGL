//
//  MusicPlayer.h
//  Pong
//
//  Created by Wolfgang Schreurs on 2/8/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>


@interface MusicPlayer : NSObject <AVAudioPlayerDelegate> {
   AVAudioPlayer *thePlayer;
}

- (void)playSong:(NSString *)name;
+ (MusicPlayer *)sharedMusicPlayer;

@end
