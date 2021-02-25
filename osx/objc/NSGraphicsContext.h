/*
 * NSGraphicsContext.h
 *
 *  Created on: Jan 14, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSGRAPHICSCONTEXT_H_
#define OSX_OBJC_NSGRAPHICSCONTEXT_H_
#include "NSBezierPath.h"
#include "NSColor.h"
#include "NSAffineTransform.h"
#include "NSAttributedString.h"
#include "NSImage.h"
typedef struct NSGraphicsContext NSGraphicsContext;
typedef struct NSCompositingOperation NSCompositingOperation;
typedef struct NSBitmapImageRep NSBitmapImageRep;

NSGraphicsContext* NSGraphicsContext_currentContext();
void NSGraphicsContext_flushGraphics(NSGraphicsContext* graphicsContext);
NSGraphicsContext* NSGraphicsContext_graphicsContextWithBitmapImageRep(NSBitmapImageRep* bitmapRep);
NSGraphicsContext* NSGraphicsContext_graphicsContextWithGraphicsPort(void* graphicsPort, int initialFlippedState);
NSGraphicsContext* NSGraphicsContext_graphicsContextWithWindow(NSWindow* window);
void* NSGraphicsContext_graphicsPort(NSGraphicsContext* graphicsContext);
NSUInteger NSGraphicsContext_imageInterpolation(NSGraphicsContext* graphicsContext);
int NSGraphicsContext_isDrawingToScreen(NSGraphicsContext* graphicsContext);
int NSGraphicsContext_isFlipped(NSGraphicsContext* graphicsContext);
void NSGraphicsContext_static_restoreGraphicsState();
void NSGraphicsContext_restoreGraphicsState(NSGraphicsContext* graphicsContext);
void NSGraphicsContext_static_saveGraphicsState();
void NSGraphicsContext_saveGraphicsState(NSGraphicsContext* graphicsContext);
void NSGraphicsContext_setCompositingOperation(NSGraphicsContext* graphicsContext,NSCompositingOperation* operation);
void NSGraphicsContext_setCurrentContext(NSGraphicsContext *context);
void NSGraphicsContext_setImageInterpolation(NSGraphicsContext* graphicsContext,NSUInteger interpolation);
void NSGraphicsContext_setPatternPhase(NSGraphicsContext* graphicsContext,NSPoint* phase);
void NSGraphicsContext_setShouldAntialias(NSGraphicsContext* graphicsContext,int antialias);
int NSGraphicsContext_shouldAntialias(NSGraphicsContext* graphicsContext);
#endif /* OSX_OBJC_NSGRAPHICSCONTEXT_H_ */
