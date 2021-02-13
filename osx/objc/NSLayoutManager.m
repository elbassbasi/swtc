#import "swt.h"
/*
 * NSLayoutManager
 */
NSLayoutManager* NSLayoutManager_init() {
    return [[NSLayoutManager alloc] init];
}
void NSLayoutManager_addTemporaryAttribute(NSLayoutManager* layout,NSString* attrName, NSObject* value, NSRange* charRange) {
    [layout addTemporaryAttribute:attrName value:value forCharacterRange:*charRange];
}
void NSLayoutManager_addTextContainer(NSLayoutManager* layout,NSTextContainer* container) {
    [layout addTextContainer:container];
}
void NSLayoutManager_boundingRectForGlyphRange(NSLayoutManager* layout,NSRange* glyphRange, NSTextContainer* container,NSRect* result) {
    *result = [layout boundingRectForGlyphRange:*glyphRange inTextContainer:container];
}
int NSLayoutManager_characterIndexForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex) {
    return [layout characterIndexForGlyphAtIndex:glyphIndex];
}
CGFloat NSLayoutManager_defaultBaselineOffsetForFont(NSLayoutManager* layout,NSFont* theFont) {
    return [layout defaultBaselineOffsetForFont:theFont];
}
CGFloat NSLayoutManager_defaultLineHeightForFont(NSLayoutManager* layout,NSFont* theFont) {
    return [layout defaultLineHeightForFont:theFont];
}
void NSLayoutManager_drawBackgroundForGlyphRange(NSLayoutManager* layout,NSRange* glyphsToShow, NSPoint* origin) {
    [layout drawBackgroundForGlyphRange:*glyphsToShow atPoint:*origin];
}
void NSLayoutManager_drawGlyphsForGlyphRange(NSLayoutManager* layout,NSRange* glyphsToShow, NSPoint* origin) {
    [layout drawGlyphsForGlyphRange:*glyphsToShow atPoint:*origin];
}
int NSLayoutManager_getGlyphs(NSLayoutManager* layout,unsigned int* glyphArray, NSRange* glyphRange) {
    return [layout getGlyphs:glyphArray range:*glyphRange];
}
int NSLayoutManager_getGlyphsInRange(NSLayoutManager* layout,NSRange* glyphRange, unsigned int* glyphBuffer, NSUInteger* charIndexBuffer, NSUInteger* inscribeBuffer, signed char * elasticBuffer,unsigned char * bidiLevelBuffer) {
    return [layout getGlyphsInRange:*glyphRange glyphs:glyphBuffer characterIndexes:charIndexBuffer glyphInscriptions:inscribeBuffer elasticBits:elasticBuffer bidiLevels:bidiLevelBuffer];
}
int NSLayoutManager_glyphIndexForCharacterAtIndex(NSLayoutManager* layout,int charIndex) {
    return [layout glyphIndexForCharacterAtIndex:charIndex];
}
int NSLayoutManager_glyphIndexForPoint(NSLayoutManager* layout,NSPoint* point, NSTextContainer* container,CGFloat* partialFraction) {
    return [layout glyphIndexForPoint:*point inTextContainer:container fractionOfDistanceThroughGlyph:partialFraction];
}
void NSLayoutManager_glyphRangeForCharacterRange(NSLayoutManager* layout,NSRange* charRange, int actualCharRange,NSRange* result) {
    *result = [layout glyphRangeForCharacterRange:*charRange actualCharacterRange:actualCharRange];
}
void NSLayoutManager_glyphRangeForTextContainer(NSLayoutManager* layout,NSTextContainer* container,NSRange* result) {
    *result = [layout glyphRangeForTextContainer:container];
}
void NSLayoutManager_lineFragmentUsedRectForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex, NSRange* effectiveGlyphRange,NSRect* result) {
    *result = [layout lineFragmentUsedRectForGlyphAtIndex:glyphIndex effectiveRange:effectiveGlyphRange];
}
void NSLayoutManager_lineFragmentUsedRectForGlyphAtIndex_0(NSLayoutManager* layout,NSUInteger glyphIndex, NSRange* effectiveGlyphRange, int flag,NSRect* result) {
    *result = [layout lineFragmentUsedRectForGlyphAtIndex:glyphIndex effectiveRange:effectiveGlyphRange withoutAdditionalLayout:flag];
}
void NSLayoutManager_locationForGlyphAtIndex(NSLayoutManager* layout,NSUInteger glyphIndex,NSPoint* result) {
    *result = [layout locationForGlyphAtIndex:glyphIndex];
}
int NSLayoutManager_numberOfGlyphs(NSLayoutManager* layout) {
    return [layout numberOfGlyphs];
}
int NSLayoutManager_rectArrayForCharacterRange(NSLayoutManager* layout,NSRange* charRange, NSRange* selCharRange, NSTextContainer* container, NSUInteger* rectCount) {
    return [layout rectArrayForCharacterRange:*charRange withinSelectedCharacterRange:*selCharRange inTextContainer:container rectCount:rectCount];
}
int NSLayoutManager_rectArrayForGlyphRange(NSLayoutManager* layout,NSRange glyphRange, NSRange selGlyphRange, NSTextContainer* container,NSUInteger* rectCount) {
    return [layout rectArrayForGlyphRange:glyphRange withinSelectedGlyphRange:selGlyphRange inTextContainer:container rectCount:rectCount];
}
void NSLayoutManager_removeTemporaryAttribute(NSLayoutManager* layout,NSString* attrName, NSRange* charRange) {
    [layout removeTemporaryAttribute:attrName forCharacterRange:*charRange];
}
void NSLayoutManager_setBackgroundLayoutEnabled(NSLayoutManager* layout,int flag) {
    [layout setBackgroundLayoutEnabled:flag];
}
void NSLayoutManager_setLineFragmentRect(NSLayoutManager* layout,NSRect* fragmentRect, NSRange* glyphRange, NSRect* usedRect) {
    [layout setLineFragmentRect:*fragmentRect forGlyphRange:*glyphRange usedRect:*usedRect];
}
void NSLayoutManager_setTextStorage(NSLayoutManager* layout,NSTextStorage* textStorage) {
    [layout setTextStorage:textStorage];
}
void NSLayoutManager_setUsesScreenFonts(NSLayoutManager* layout,int flag) {
    [layout setUsesScreenFonts:flag];
}
NSTypesetter* NSLayoutManager_typesetter(NSLayoutManager* layout) {
    return [layout typesetter];
}
void NSLayoutManager_usedRectForTextContainer(NSLayoutManager* layout,NSTextContainer *container,NSRect* result) {
    *result = [layout usedRectForTextContainer:container];
}
/*
 *  NSTextContainer
 */
void NSTextContainer_containerSize(NSTextContainer* container,NSSize* result) {
    *result = [container containerSize];
}
NSTextContainer* NSTextContainer_initWithContainerSize(NSSize* size) {
    return [[NSTextContainer alloc] initWithContainerSize:*size];
}
void NSTextContainer_setContainerSize(NSTextContainer* container,NSSize* size) {
    [container setContainerSize:*size];
}
void NSTextContainer_setLineFragmentPadding(NSTextContainer* container,CGFloat pad) {
    [container setLineFragmentPadding:pad];
}
void NSTextContainer_setWidthTracksTextView(NSTextContainer* container,int flag) {
    [container setWidthTracksTextView:flag];
}
