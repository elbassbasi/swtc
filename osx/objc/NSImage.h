/*
 * NSImage.h
 *
 *  Created on: Feb 14, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSIMAGE_H_
#define OSX_OBJC_NSIMAGE_H_
#include "NSDictionary.h"
typedef struct NSImage NSImage;
typedef struct NSImageRep NSImageRep;
typedef struct NSImageView NSImageView;
typedef struct OpaqueIconRef *IconRef;
typedef struct NSData NSData;
typedef struct NSBitmapFormat NSBitmapFormat;
typedef struct NSBitmapImageRep NSBitmapImageRep;
typedef struct NSCursor NSCursor;
/*
 *  NSImage
 */
NSData* NSImage_TIFFRepresentation(NSImage *image);
void NSImage_addRepresentation(NSImage *image, NSImageRep *imageRep);
NSImageRep* NSImage_bestRepresentationForDevice(NSImage *image,
		NSDictionary *deviceDescription);
void NSImage_drawAtPoint(NSImage *image, NSPoint *point, NSRect *fromRect,
		int op, CGFloat delta);
void NSImage_drawInRect(NSImage *image, NSRect *rect, NSRect *fromRect, int op,
		CGFloat delta);
NSImage* NSImage_imageNamed(NSString *name);
NSImage* NSImage_initByReferencingFile(NSString *fileName);
NSImage* NSImage_initWithContentsOfFile(NSString *fileName);
NSImage* NSImage_initWithData(NSData *data);
NSImage* NSImage_initWithIconRef(IconRef iconRef);
NSImage* NSImage_initWithSize(NSSize *aSize);
void NSImage_lockFocus(NSImage *image);
void NSImage_removeRepresentation(NSImage *image, NSImageRep *imageRep);
NSArray* NSImage_representations(NSImage *image);
void NSImage_setCacheMode(NSImage *image, int mode);
void NSImage_setScalesWhenResized(NSImage *image, int flag);
void NSImage_setSize(NSImage *image, NSSize *aSize);
void NSImage_size(NSImage *image, NSSize *size);
void NSImage_unlockFocus(NSImage *image);
/*
 *  NSImageRep
 */
NSInteger NSImageRep_bitsPerSample(NSImageRep *image);
NSString* NSImageRep_colorSpaceName(NSImageRep *image);
int NSImageRep_drawInRect(NSImageRep *image, NSRect *rect);
int NSImageRep_hasAlpha(NSImageRep *image);
NSImageRep* NSImageRep_imageRepWithContentsOfFile(NSString *filename);
NSInteger NSImageRep_pixelsHigh(NSImageRep *image);
NSInteger NSImageRep_pixelsWide(NSImageRep *image);
void NSImageRep_setAlpha(NSImageRep *image, int flag);
void NSImageRep_setSize(NSImageRep *image, NSSize *aSize);
/*
 *  NSBitmapImageRep
 */
NSData* NSBitmapImageRep_TIFFRepresentation(NSBitmapImageRep *bitmap);
unsigned char* NSBitmapImageRep_bitmapData(NSBitmapImageRep *bitmap);
NSBitmapFormat* NSBitmapImageRep_bitmapFormat(NSBitmapImageRep *bitmap);
NSInteger NSBitmapImageRep_bitsPerPixel(NSBitmapImageRep *bitmap);
NSInteger NSBitmapImageRep_bytesPerPlane(NSBitmapImageRep *bitmap);
NSInteger NSBitmapImageRep_bytesPerRow(NSBitmapImageRep *bitmap);
NSColor* NSBitmapImageRep_colorAtX(NSBitmapImageRep *bitmap, NSInteger x,
		NSInteger y);
void NSBitmapImageRep_getBitmapDataPlanes(NSBitmapImageRep *bitmap,
		unsigned char *data);
NSBitmapImageRep* NSBitmapImageRep_imageRepWithData(NSData *data);
NSBitmapImageRep* NSBitmapImageRep_initWithBitmapDataPlanes_0(
		unsigned char **planes, NSInteger width, NSInteger height,
		NSInteger bps, NSInteger spp, int alpha, int isPlanar,
		NSString *colorSpaceName, NSInteger bitmapFormat, NSInteger rBytes,
		NSInteger pBits);
NSBitmapImageRep* NSBitmapImageRep_initWithBitmapDataPlanes_1(
		unsigned char **planes, NSInteger width, NSInteger height,
		NSInteger bps, NSInteger spp, int alpha, int isPlanar,
		NSString *colorSpaceName, NSInteger rBytes, NSInteger pBits);
NSBitmapImageRep* NSBitmapImageRep_initWithData(NSData *data);
NSBitmapImageRep* NSBitmapImageRep_initWithFocusedViewRect(NSRect *rect);
int NSBitmapImageRep_isPlanar(NSBitmapImageRep *bitmap);
NSInteger NSBitmapImageRep_numberOfPlanes(NSBitmapImageRep *bitmap);
NSInteger NSBitmapImageRep_samplesPerPixel(NSBitmapImageRep *bitmap);
NSImageRep* NSBitmapImageRep_imageRepWithContentsOfFile(NSString *filename);
NSString* _NSDeviceRGBColorSpace();
/*
 *  NSImageView
 */
w_widget* SWTImageView_get_widget(NSView* view);
NSImageView* SWTImageView_new(w_widget* widget);
NSImage* NSImageView_image(NSImageView *view);
void NSImageView_setImage(NSImageView *view, NSImage *newImage);
void NSImageView_setImageAlignment(NSImageView *view, int newAlign);
void NSImageView_setImageScaling(NSImageView *view, int newScaling);
Class* NSImageView_cellClass();
void NSImageView_setCellClass(Class *factoryId);
/*
 *  NSCursor
 */
void NSCursor_get_systems(NSCursor** cursors);
void NSCursor_set(NSCursor* cursor);
NSCursor* NSCursor_initWithImage(NSImage* image,NSPoint* hotSpot);
#endif /* OSX_OBJC_NSIMAGE_H_ */
