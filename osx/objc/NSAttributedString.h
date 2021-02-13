/*
 * NSAttributedString.h
 *
 *  Created on: Jan 24, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSATTRIBUTEDSTRING_H_
#define OSX_OBJC_NSATTRIBUTEDSTRING_H_
#include "NSDictionary.h"
#include "NSLayoutManager.h"
typedef struct NSAttributedString NSAttributedString;
typedef struct NSMutableAttributedString NSMutableAttributedString;
typedef struct NSTextAttachment NSTextAttachment;
typedef struct NSMutableString NSMutableString;
/*
 *  NSAttributedString
 */
NSAttributedString* NSAttributedString_attributedStringWithAttachment(NSTextAttachment* attachment);
void NSAttributedString_boundingRectWithSize(NSAttributedString *attrString,NSSize size,NSInteger options,NSRect* result);
void NSAttributedString_doubleClickAtIndex(NSAttributedString *attrString,NSUInteger location,NSRange* result);
void NSAttributedString_drawAtPoint(NSAttributedString *attrString,NSPoint* point);
void NSAttributedString_drawInRect(NSAttributedString *attrString,NSRect* rect);
NSUInteger NSAttributedString_nextWordFromIndex(NSAttributedString *attrString,NSUInteger location, int isForward);
void NSAttributedString_size(NSAttributedString *attrString,NSSize* result);
NSObject* NSAttributedString_attribute(NSAttributedString *attrString,NSString* attrName,NSUInteger location,NSRange* range);
NSAttributedString* NSAttributedString_attributedSubstringFromRange(NSAttributedString *attrString,NSRange* range);
NSDictionary* NSAttributedString_attributesAtIndex(NSAttributedString *attrString,NSUInteger location,NSRange* range, NSRange* rangeLimit);
NSAttributedString* NSAttributedString_initWithString(NSString* str);
NSAttributedString* NSAttributedString_initWithString_0(NSString* str, NSDictionary* attrs);
NSUInteger NSAttributedString_length(NSAttributedString *attrString);
NSString* NSAttributedString_string(NSAttributedString *attrString);
/*
 *  NSMutableAttributedString
 */
void NSMutableAttributedString_setBaseWritingDirection(NSMutableAttributedString *attrString,NSInteger writingDirection, NSRange* range);
void NSMutableAttributedString_addAttribute(NSMutableAttributedString *attrString,NSString* name, NSObject* value, NSRange* range);
void NSMutableAttributedString_appendAttributedString(NSMutableAttributedString *attrString,NSAttributedString* _attrString);
void NSMutableAttributedString_beginEditing(NSMutableAttributedString *attrString);
void NSMutableAttributedString_endEditing(NSMutableAttributedString *attrString);
NSMutableString* NSMutableAttributedString_mutableString(NSMutableAttributedString *attrString);
void NSMutableAttributedString_removeAttribute(NSMutableAttributedString *attrString,NSString* name, NSRange* range);
void NSMutableAttributedString_replaceCharactersInRange(NSMutableAttributedString *attrString,NSRange* range, NSString* str);
void NSMutableAttributedString_setAttributedString(NSMutableAttributedString *attrString,NSAttributedString* _attrString);
NSAttributedString* NSMutableAttributedString_attributedStringWithAttachment(NSTextAttachment* attachment);
/*
 *  NSTextStorage
 */
NSTextStorage* NSTextStorage_init();
void NSTextStorage_addLayoutManager(NSTextStorage* textStorage,NSLayoutManager* obj);
NSArray* NSTextStorage_paragraphs(NSTextStorage* textStorage);
NSAttributedString* NSTextStorage_attributedStringWithAttachment(NSTextAttachment* attachment);
/*
 *  NSAttributedStringKey
 */
NSString* _NSFontAttributeName();
NSString* _NSParagraphStyleAttributeName();
NSString* _NSForegroundColorAttributeName();
NSString* _NSBackgroundColorAttributeName();
NSString* _NSLigatureAttributeName();
NSString* _NSKernAttributeName();
NSString* _NSStrikethroughStyleAttributeName();
NSString* _NSUnderlineStyleAttributeName();
NSString* _NSStrokeColorAttributeName();
NSString* _NSStrokeWidthAttributeName();
NSString* _NSShadowAttributeName();
NSString* _NSTextEffectAttributeName();
NSString* _NSAttachmentAttributeName();
NSString* _NSLinkAttributeName();
NSString* _NSBaselineOffsetAttributeName();
NSString* _NSUnderlineColorAttributeName();
NSString* _NSStrikethroughColorAttributeName();
NSString* _NSObliquenessAttributeName();
NSString* _NSExpansionAttributeName();
NSString* _NSWritingDirectionAttributeName();
NSString* _NSVerticalGlyphFormAttributeName();
NSString* _NSCursorAttributeName();
NSString* _NSToolTipAttributeName();
NSString* _NSMarkedClauseSegmentAttributeName();
NSString* _NSTextAlternativesAttributeName();
NSString* _NSSpellingStateAttributeName();
NSString* _NSSuperscriptAttributeName();
NSString* _NSGlyphInfoAttributeName();

#endif /* OSX_OBJC_NSATTRIBUTEDSTRING_H_ */
