/*
 *  OALCommon.c
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 2/2/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OALCommon.h"


static ALuint           sourceIndex = 0;
ALCcontext              *alContext;
ALCdevice               *alDevice;
CFMutableDictionaryRef  oalBufferedSounds;
ALuint                  oalSources[OAL_AUDIO_CHANNELS];


void oalBufferAudioFiles (char * files[], int count) 
{
   for (int i = 0; i < count; i++) 
   {
      char *name = files[i];
      CFStringRef file = CFStringCreateWithCString(kCFAllocatorDefault, name, kCFStringEncodingUTF8);
      CFBundleRef bundle = CFBundleGetMainBundle();
		CFURLRef filePath = CFBundleCopyResourceURL(bundle, file, NULL, NULL);

      AudioFileID outAFID = oalGetAudioFileID(filePath);
      UInt32 fileSize = oalGetFileSize(outAFID);
      
      unsigned char *outData = malloc(fileSize);
      ALuint bufferID = oalCreateBufferWithAFID(outAFID, fileSize, outData);
      free(outData);
      
      CFNumberRef num = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &bufferID);
      CFDictionaryAddValue(oalBufferedSounds, file, num);
      CFRelease(num);
      CFRelease(file);
   }
}

void oalPlaySound (char * file) 
{
   
   if (sourceIndex == OAL_AUDIO_CHANNELS) {
      ALuint sourceID = oalSources[0];
      alDeleteSources(1, &sourceID);
      sourceIndex -= 1;
   }
   
   CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, file, kCFStringEncodingUTF8);
   CFNumberRef number = CFDictionaryGetValue(oalBufferedSounds, key);
   CFRelease(key);
   
   ALuint bufferID;
   CFNumberGetValue(number, kCFNumberIntType, &bufferID);
   ALuint sourceID = oalCreateSourceWithBuffer(bufferID, false);

   oalSources[sourceIndex] = sourceID;
   alSourcePlay(sourceID);   
   sourceIndex += 1;
}

void oalInit (void) 
{
   alDevice = alcOpenDevice(NULL);
   if (alDevice) 
   {
      alContext = alcCreateContext(alDevice, NULL);

      OSStatus result = noErr;
      void * data; // could probably be NULL, should test before removing
      result = AudioSessionInitialize(NULL, NULL, openALInterruptionListener, &data);
      if (result != 0) 
      {
         printf("failed to initialize audio session");
      } 
       
      UInt32 category = kAudioSessionCategory_AmbientSound;
      result = noErr;
      result = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(category), &category);
      if (result != 0) 
      {
         printf("failed to set property for audio session");
      } 
//      AudioSessionSetActive(true);  // TODO: check if required, probably not

      alcMakeContextCurrent(alContext);

      oalBufferedSounds = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
   } 
   else 
   {
      printf("Error: failed to initialize OpenAL context, could not open device.");
   }
}

AudioFileID oalGetAudioFileID (CFURLRef filePath)
{
	AudioFileID outAFID;
   
#if TARGET_OS_IPHONE
   OSStatus result = AudioFileOpenURL(filePath, kAudioFileReadPermission, 0, &outAFID);
#else
   OSStatus result = AudioFileOpenURL(filePath, fsRdPerm, 0, &outAFID);
#endif
      
   if (result != 0)
   {
      char *string;
      CFIndex buffer = 100;
      CFStringRef file = CFURLGetString(filePath);
      while (!CFStringGetCString(file, string, buffer, kCFStringEncodingMacRoman)) 
      {
         buffer += 10;
      } 
   
      printf("Can't open file: %s", string);  
   }
   
   return outAFID;
}

UInt32 oalGetFileSize (AudioFileID fileDescriptor)
{
	UInt64 outDataSize = 0;
	UInt32 thePropSize = sizeof(UInt64);

	OSStatus result = AudioFileGetProperty(fileDescriptor, kAudioFilePropertyAudioDataByteCount, &thePropSize, &outDataSize);
	if (result != 0) 
   {
      printf("Can't find file size.");
   }
   
   return (UInt32)outDataSize;
}

ALuint oalCreateBufferWithAFID (AudioFileID fileDescriptor, UInt32 fileSize, unsigned char *outData)
{
   OSStatus result = noErr;
   result = AudioFileReadBytes(fileDescriptor, false, 0, &fileSize, outData);
   AudioFileClose(fileDescriptor);
   if (result != 0) 
   {
      printf("Can't load sound effect"); // should show filename as well ...
   }
   
   ALuint bufferID;
   alGenBuffers(1, &bufferID);
   alBufferData(bufferID, AL_FORMAT_STEREO16, outData, fileSize, 44100);

   return bufferID;
}

ALuint oalCreateSourceWithBuffer (ALuint bufferID, bool shouldLoop) 
{
   ALuint sourceID;
   alGenSources(1, &sourceID);
   
   alSourcei(sourceID, AL_BUFFER, bufferID);
   alSourcef(sourceID, AL_PITCH, 1.0f);
   alSourcef(sourceID, AL_GAIN, 1.0f);
   
   if (shouldLoop) 
   {
      alSourcei(sourceID, AL_LOOPING, AL_TRUE);
   }
   
   return sourceID;
}

void oalRelease (void) 
{   
   for (int i = 0; i < sourceIndex; i++) 
   {
      ALuint sourceID = oalSources[i];
      alDeleteSources(1, &sourceID);
   }

   int bufferCount = CFDictionaryGetCount(oalBufferedSounds);
   CFNumberRef values[bufferCount];
   CFDictionaryGetKeysAndValues(oalBufferedSounds, NULL, (const void **)&values);
   for (int i = 0; i < bufferCount; i++) 
   {
      CFNumberRef num = values[i];
      ALuint bufferID;
      CFNumberGetValue(num, kCFNumberIntType, &bufferID);
      alDeleteBuffers(1, &bufferID);
   }
   
   CFRelease(oalBufferedSounds);
   
   alcDestroyContext(alContext);
   alcCloseDevice(alDevice);
}



#pragma mark Interruption Callback

void openALInterruptionListener (void *inClientData, UInt32 inInterruptionState) 
{
   if (inInterruptionState == kAudioSessionBeginInterruption) {
      alcMakeContextCurrent (NULL);
      alcSuspendContext(alContext);
      printf("audio > going to sleep ...");
   } else if (inInterruptionState == kAudioSessionEndInterruption) {
      alcMakeContextCurrent (alContext);
      alcProcessContext(alContext);
      printf("audio > awaking ...");
      
   }
}

