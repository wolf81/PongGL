/*
 *  OGLTexture.c
 *  Pong
 *
 *  Created by Wolfgang Schreurs on 1/26/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "OGLTexture.h"

void oglMapTextureCoords (GLfloat vertices[], GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4) 
{
	/*
	 mapping values range from 0.0 to 1.0, indexes correspond as follows:
		x1, y1: top left
		x2, y2: top right
		x3, y3: bottom left
		x4, y4: bottom right
	 */
	
	vertices[0] = x1;
	vertices[1] = y1;
	vertices[2] = x2;
	vertices[3] = y2;
	vertices[4] = x3;
	vertices[5] = y3;
	vertices[6] = x4;
	vertices[7] = y4;
}

void oglReleaseTextures (void)
{
	free(textures);	
}

void oglCreateTexture (CGImageRef image, GLuint index) 
{
	glBindTexture(GL_TEXTURE_2D, textures[index]); 	
	
	// set parameters for the texture, the following 2 parameters are always required for the iOS devices if we don't use the default setting (mipmaps)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // we'll use linear scaling when a texture size is shrunk 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // we'll use linear scaling when a texture size is grown
	
	GLuint width = CGImageGetWidth(image);
	GLuint height = CGImageGetHeight(image);
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	void *imageData = malloc (height * width * 4); // hight * width * 4 bits per pixel (RGB)
	
	// draw imagedata from the imagefile to an appropriate context (correct memorysize, width, height, colorspace, etc...) for the texture
	CGContextRef context = CGBitmapContextCreate (imageData, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big); // this CGContextRef acts like an offscreen buffer to draw upon
	CGColorSpaceRelease (colorSpace);
	CGContextClearRect (context, CGRectMake(0, 0, width, height));
	CGContextTranslateCTM (context, 0, height - height); // perform a translate action on the Current Transformation Matrix changing y-coordinates of texture
	CGContextDrawImage (context, CGRectMake (0, 0, width, height), image);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData); // specify 2D texture image
	
	CGContextRelease(context);		
	free (imageData);
}

void oglCreateTextures (CFArrayRef files) 
{		
	// reallocate the textures array so it can contain all 'texture names' (numbers), generate 'texture names' as well
	int count = CFArrayGetCount(files);
	textures = realloc(textures, count * sizeof (GLuint)); 
	glGenTextures(count, textures); 

	// process all texture files in array
	for (int i = 0; i < count; i++) {
		CFStringRef file = CFArrayGetValueAtIndex(files, i);
		CFBundleRef bundle = CFBundleGetMainBundle();
		CFURLRef fileURL = CFBundleCopyResourceURL(bundle, file, NULL, NULL);
		
		CGDataProviderRef dataProvider = CGDataProviderCreateWithURL(fileURL);
		CGImageRef image = CGImageCreateWithPNGDataProvider(dataProvider, NULL, false, kCGRenderingIntentDefault);
		
		oglCreateTexture (image, i);
		
		CFRelease(fileURL);		
	}
}