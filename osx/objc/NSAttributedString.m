#import "swt.h"
/*
 *  NSAttributedString
 */
NSAttributedString* NSAttributedString_attributedStringWithAttachment(NSTextAttachment* attachment) {
    return [NSAttributedString attributedStringWithAttachment:attachment];
}
void NSAttributedString_boundingRectWithSize(NSAttributedString *attrString,NSSize size,NSInteger options,NSRect* result) {
    *result = [attrString boundingRectWithSize:size options:options];
}
void NSAttributedString_doubleClickAtIndex(NSAttributedString *attrString,NSUInteger location,NSRange* result) {
    *result = [attrString doubleClickAtIndex:location];
}
void NSAttributedString_drawAtPoint(NSAttributedString *attrString,NSPoint* point) {
    [attrString drawAtPoint:*point];
}
void NSAttributedString_drawInRect(NSAttributedString *attrString,NSRect* rect) {
    [attrString drawInRect:*rect];
}
NSUInteger NSAttributedString_nextWordFromIndex(NSAttributedString *attrString,NSUInteger location, int isForward) {
    return [attrString nextWordFromIndex:location forward:isForward];
}
void NSAttributedString_size(NSAttributedString *attrString,NSSize* result) {
    *result = [attrString size];
}
NSObject* NSAttributedString_attribute(NSAttributedString *attrString,NSString* attrName,NSUInteger location,NSRange* range) {
    return [attrString attribute:attrName atIndex:location effectiveRange:range];
}
NSAttributedString* NSAttributedString_attributedSubstringFromRange(NSAttributedString *attrString,NSRange* range) {
    return [attrString attributedSubstringFromRange:*range];
}
NSDictionary* NSAttributedString_attributesAtIndex(NSAttributedString *attrString,NSUInteger location,NSRange* range, NSRange* rangeLimit) {
    return [attrString attributesAtIndex:location longestEffectiveRange:range inRange:*rangeLimit];
}
NSAttributedString* NSAttributedString_initWithString(NSString* str) {
    return [[NSAttributedString alloc] initWithString:str];
}
NSAttributedString* NSAttributedString_initWithString_0(NSString* str, NSDictionary* attrs) {
    return [[NSAttributedString alloc] initWithString:str attributes:attrs];
}
NSUInteger NSAttributedString_length(NSAttributedString *attrString) {
    return [attrString length];
}
NSString* NSAttributedString_string(NSAttributedString *attrString) {
    return [attrString string];
}
/*
 *  NSMutableAttributedString
 */
void NSMutableAttributedString_setBaseWritingDirection(NSMutableAttributedString *attrString,NSInteger writingDirection, NSRange* range) {
    [attrString setBaseWritingDirection:writingDirection range:*range];
}
void NSMutableAttributedString_addAttribute(NSMutableAttributedString *attrString,NSString* name, NSObject* value, NSRange* range) {
    [attrString addAttribute:name value:value range:*range];
}
void NSMutableAttributedString_appendAttributedString(NSMutableAttributedString *attrString,NSAttributedString* _attrString) {
    [attrString appendAttributedString:_attrString];
}
void NSMutableAttributedString_beginEditing(NSMutableAttributedString *attrString) {
    [attrString beginEditing];
}
void NSMutableAttributedString_endEditing(NSMutableAttributedString *attrString) {
    [attrString endEditing];
}
NSMutableString* NSMutableAttributedString_mutableString(NSMutableAttributedString *attrString) {
    return [attrString mutableString];
}
void NSMutableAttributedString_removeAttribute(NSMutableAttributedString *attrString,NSString* name, NSRange* range) {
    [attrString removeAttribute:name range:*range];
}
void NSMutableAttributedString_replaceCharactersInRange(NSMutableAttributedString *attrString,NSRange* range, NSString* str) {
    [attrString replaceCharactersInRange:*range withString:str];
}
void NSMutableAttributedString_setAttributedString(NSMutableAttributedString *attrString,NSAttributedString* _attrString) {
    [attrString setAttributedString:_attrString];
}
NSAttributedString* NSMutableAttributedString_attributedStringWithAttachment(NSTextAttachment* attachment) {
    return [NSMutableAttributedString attributedStringWithAttachment:attachment];
}
/*
 *  NSTextStorage
 */
NSTextStorage* NSTextStorage_init(){
    return [[NSTextStorage alloc] init];
}
void NSTextStorage_addLayoutManager(NSTextStorage* textStorage,NSLayoutManager* obj) {
    [textStorage addLayoutManager:obj];
}
NSArray* NSTextStorage_paragraphs(NSTextStorage* textStorage) {
    return [textStorage paragraphs];
}
NSAttributedString* NSTextStorage_attributedStringWithAttachment(NSTextAttachment* attachment) {
    return [NSTextStorage attributedStringWithAttachment:attachment];
}
/*
 *  NSParagraphStyle
 */
NSUInteger NSParagraphStyle_alignment(NSParagraphStyle* style) {
    return [style alignment];
}

NSArray* NSParagraphStyle_tabStops(NSParagraphStyle* style) {
    return [style tabStops];
}
/*
 *  NSMutableParagraphStyle
 */
NSMutableParagraphStyle* NSMutableParagraphStyle_init() {
    return [[NSMutableParagraphStyle alloc] init];
}
void NSMutableParagraphStyle_addTabStop(NSMutableParagraphStyle* style,NSTextTab* anObject) {
    [style addTabStop:anObject];
}

void NSMutableParagraphStyle_setAlignment(NSMutableParagraphStyle* style,NSUInteger alignment) {
    [style setAlignment:alignment];
}

void NSMutableParagraphStyle_setBaseWritingDirection(NSMutableParagraphStyle* style,NSUInteger writingDirection) {
    [style setBaseWritingDirection:writingDirection];
}

void NSMutableParagraphStyle_setDefaultTabInterval(NSMutableParagraphStyle* style,CGFloat aFloat) {
    [style setDefaultTabInterval:aFloat];
}

void NSMutableParagraphStyle_setFirstLineHeadIndent(NSMutableParagraphStyle* style,CGFloat aFloat) {
    [style setFirstLineHeadIndent:aFloat];
}

void NSMutableParagraphStyle_setHeadIndent(NSMutableParagraphStyle* style,CGFloat aFloat) {
    [style setHeadIndent:aFloat];
}

void NSMutableParagraphStyle_setLineBreakMode(NSMutableParagraphStyle* style,NSUInteger mode) {
    [style setLineBreakMode:mode];
}

void NSMutableParagraphStyle_setLineSpacing(NSMutableParagraphStyle* style,CGFloat aFloat) {
    [style setLineSpacing:aFloat];
}
void NSMutableParagraphStyle_setTabStops(NSMutableParagraphStyle* style,NSArray* array) {
    [style setTabStops:array];
}

/*
 *  NSAttributedStringKey
 */
NSString* _NSFontAttributeName(){
    return NSFontAttributeName;
}
NSString* _NSParagraphStyleAttributeName(){
    return NSParagraphStyleAttributeName;
}
NSString* _NSForegroundColorAttributeName(){
    return NSForegroundColorAttributeName;
}
NSString* _NSBackgroundColorAttributeName(){
    return NSBackgroundColorAttributeName;
}
NSString* _NSLigatureAttributeName(){
    return NSLigatureAttributeName;
}
NSString* _NSKernAttributeName(){
    return NSKernAttributeName;
}
NSString* _NSStrikethroughStyleAttributeName(){
    return NSStrikethroughStyleAttributeName;
}
NSString* _NSUnderlineStyleAttributeName(){
    return NSUnderlineStyleAttributeName;
}
NSString* _NSStrokeColorAttributeName(){
    return NSStrokeColorAttributeName;
}
NSString* _NSStrokeWidthAttributeName(){
    return NSStrokeWidthAttributeName;
}
NSString* _NSShadowAttributeName(){
    return NSShadowAttributeName;
}
NSString* _NSTextEffectAttributeName(){
    return NSTextEffectAttributeName;
}
NSString* _NSAttachmentAttributeName(){
    return NSAttachmentAttributeName;
}
NSString* _NSLinkAttributeName(){
    return NSLinkAttributeName;
}
NSString* _NSBaselineOffsetAttributeName(){
    return NSBaselineOffsetAttributeName;
}
NSString* _NSUnderlineColorAttributeName(){
    return NSUnderlineColorAttributeName;
}
NSString* _NSStrikethroughColorAttributeName(){
    return NSStrikethroughColorAttributeName;
}
NSString* _NSObliquenessAttributeName(){
    return NSObliquenessAttributeName;
}
NSString* _NSExpansionAttributeName(){
    return NSExpansionAttributeName;
}
NSString* _NSWritingDirectionAttributeName(){
    return NSWritingDirectionAttributeName;
}
NSString* _NSVerticalGlyphFormAttributeName(){
    return NSVerticalGlyphFormAttributeName;
}
NSString* _NSCursorAttributeName(){
    return NSCursorAttributeName;
}
NSString* _NSToolTipAttributeName(){
    return NSToolTipAttributeName;
}
NSString* _NSMarkedClauseSegmentAttributeName(){
    return NSMarkedClauseSegmentAttributeName;
}
NSString* _NSTextAlternativesAttributeName(){
    return NSTextAlternativesAttributeName;
}
NSString* _NSSpellingStateAttributeName(){
    return NSSpellingStateAttributeName;
}
NSString* _NSSuperscriptAttributeName(){
    return NSSuperscriptAttributeName;
}
NSString* _NSGlyphInfoAttributeName(){
    return NSGlyphInfoAttributeName;
}
