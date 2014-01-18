/*
 *  OALCommon.h
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>


#define OAL_AUDIO_CHANNELS 32

void oalInit (void);
AudioFileID oalGetAudioFileID (CFURLRef filePath);
UInt32 oalGetFileSize (AudioFileID fileDescriptor);
ALuint oalCreateBufferWithAFID (AudioFileID fileDescriptor, UInt32 fileSize, unsigned char *outData);
ALuint oalCreateSourceWithBuffer (ALuint bufferID, bool shouldLoop);
void oalRelease (void); 
void oalBufferAudioFiles (char * files[], int count);
void oalPlaySound (char * file);

void openALInterruptionListener (void *inClientData, UInt32 inInterruptionState);

/*
 TODO : add code to figure out errors happening in OpenAL, like this (should be used after every important call):
 
ALEnum err = alGetError();
if (err != 0) NSlog(@"Error Calling alSourceQueueBuffers: %d",err);
 */