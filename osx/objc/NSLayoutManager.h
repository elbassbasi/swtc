/*
 * NSLayoutManager.h
 *
 *  Created on: Jan 24, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSLAYOUTMANAGER_H_
#define OSX_OBJC_NSLAYOUTMANAGER_H_
#include "NSFont.h"
typedef struct NSLayoutManager NSLayoutManager;
typedef struct NSTextContainer NSTextContainer;
typedef struct NSTypesetter NSTypesetter;
typedef struct NSTextStorage NSTextStorage;
/*
 * NSLayoutManager
 */
NSLayoutManager* NSLayoutManager_init();
void NSLayoutManager_addTemporaryAttribute(NSLayoutManager* layout,NSString* attrName, NSObject* value, NSRange* charRange);
void NSLayoutManager_addTextContainer(NSLayoutManager* layout,NSTextContainer* container);
void NSLayoutManager_boundingRectForGlyphRange(NSLayoutManager* layout,NSRange* glyphRange, NSTextContainer* container,NSRect* result);
int NSLayoutManager_characterIndexForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex);
CGFloat NSLayoutManager_defaultBaselineOffsetForFont(NSLayoutManager* layout,NSFont* theFont);
CGFloat NSLayoutManager_defaultLineHeightForFont(NSLayoutManager* layout,NSFont* theFont);
void NSLayoutManager_drawBackgroundForGlyphRange(NSLayoutManager* layout,NSRange* glyphsToShow, NSPoint* origin);
void NSLayoutManager_drawGlyphsForGlyphRange(NSLayoutManager* layout,NSRange* glyphsToShow, NSPoint* origin);
int NSLayoutManager_getGlyphs(NSLayoutManager* layout,unsigned int* glyphArray, NSRange* glyphRange);
int NSLayoutManager_getGlyphsInRange(NSLayoutManager* layout,NSRange* glyphRange, unsigned int* glyphBuffer, NSUInteger* charIndexBuffer, NSUInteger* inscribeBuffer, signed char * elasticBuffer,unsigned char * bidiLevelBuffer);
int NSLayoutManager_glyphIndexForCharacterAtIndex(NSLayoutManager* layout,int charIndex);
int NSLayoutManager_glyphIndexForPoint(NSLayoutManager* layout,NSPoint* point, NSTextContainer* container,CGFloat* partialFraction);
void NSLayoutManager_glyphRangeForCharacterRange(NSLayoutManager* layout,NSRange* charRange, int actualCharRange,NSRange* result);
void NSLayoutManager_glyphRangeForTextContainer(NSLayoutManager* layout,NSTextContainer* container,NSRange* result);
void NSLayoutManager_lineFragmentUsedRectForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex, NSRange* effectiveGlyphRange,NSRect* result);
void NSLayoutManager_lineFragmentUsedRectForGlyphAtIndex_0(NSLayoutManager* layout,NSUInteger glyphIndex, NSRange* effectiveGlyphRange, int flag,NSRect* result);
void NSLayoutManager_locationForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex,NSPoint* result);
int NSLayoutManager_numberOfGlyphs(NSLayoutManager* layout);
int NSLayoutManager_rectArrayForCharacterRange(NSLayoutManager* layout,NSRange* charRange, NSRange* selCharRange, NSTextContainer* container, NSUInteger* rectCount);
int NSLayoutManager_rectArrayForGlyphRange(NSLayoutManager* layout,NSRange glyphRange, NSRange selGlyphRange, NSTextContainer* container,NSUInteger* rectCount);
void NSLayoutManager_removeTemporaryAttribute(NSLayoutManager* layout,NSString* attrName, NSRange* charRange);
void NSLayoutManager_setBackgroundLayoutEnabled(NSLayoutManager* layout,int flag);
void NSLayoutManager_setLineFragmentRect(NSLayoutManager* layout,NSRect* fragmentRect, NSRange* glyphRange, NSRect* usedRect);
void NSLayoutManager_setTextStorage(NSLayoutManager* layout,NSTextStorage* textStorage);
void NSLayoutManager_setUsesScreenFonts(NSLayoutManager* layout,int flag);
NSTypesetter* NSLayoutManager_typesetter(NSLayoutManager* layout);
void NSLayoutManager_usedRectForTextContainer(NSLayoutManager* layout,NSTextContainer *container,NSRect* result);
/*
 *  NSTextContainer
 */
void NSTextContainer_containerSize(NSTextContainer* container,NSSize* result);
NSTextContainer* NSTextContainer_initWithContainerSize(NSSize* size);
void NSTextContainer_setContainerSize(NSTextContainer* container,NSSize* size);
void NSTextContainer_setLineFragmentPadding(NSTextContainer* container,CGFloat pad);
void NSTextContainer_setWidthTracksTextView(NSTextContainer* container,int flag);
#endif /* OSX_OBJC_NSLAYOUTMANAGER_H_ */
