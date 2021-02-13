#import "swt.h"
/*
 * NSFont
 */
CGFloat NSFont_ascender(NSFont* font) {
    return [font ascender];
}
NSFont* NSFont_controlContentFontOfSize(CGFloat fontSize) {
    return [NSFont controlContentFontOfSize:fontSize];
}
CGFloat NSFont_descender(NSFont* font) {
    return [font descender];
}
NSString* NSFont_displayName(NSFont* font) {
    return [font displayName];
}
NSString* NSFont_familyName(NSFont* font) {
    return [font familyName];
}
NSString* NSFont_fontName(NSFont* font) {
    return [font fontName];
}
NSFont* NSFont_fontWithName(NSString* fontName, CGFloat fontSize) {
    return [NSFont fontWithName:fontName size:fontSize];
}
CGFloat NSFont_leading(NSFont* font) {
    return [font leading];
}
NSFont* NSFont_menuBarFontOfSize(CGFloat fontSize) {
    return [NSFont menuBarFontOfSize:fontSize];
}
NSFont* NSFont_menuFontOfSize(CGFloat fontSize) {
    return [NSFont menuFontOfSize:fontSize];
}
CGFloat NSFont_pointSize(NSFont* font) {
    return [font pointSize];
}
CGFloat NSFont_smallSystemFontSize() {
    return [NSFont smallSystemFontSize];
}
NSFont* NSFont_systemFontOfSize(CGFloat fontSize) {
    return [NSFont systemFontOfSize:fontSize];
}
CGFloat NSFont_systemFontSize() {
    return [NSFont systemFontSize];
}
CGFloat NSFont_systemFontSizeForControlSize(int controlSize) {
    return [NSFont systemFontSizeForControlSize:controlSize];
}
/*
 *  NSFontManager
 */
NSArray* NSFontManager_availableFontFamilies(NSFontManager* fontmanager) {
    return [fontmanager availableFontFamilies];
}
NSArray* NSFontManager_availableFonts(NSFontManager* fontmanager) {
    return [fontmanager availableFonts];
}
NSArray* NSFontManager_availableMembersOfFontFamily(NSFontManager* fontmanager,NSString* fam) {
    return [fontmanager availableMembersOfFontFamily:fam];
}
NSFont* NSFontManager_convertFont(NSFontManager* fontmanager,NSFont* fontObj, int trait) {
    return [fontmanager convertFont:fontObj toHaveTrait:trait];
}
NSFont* NSFontManager_fontWithFamily(NSFontManager* fontmanager,NSString* family, int traits,int weight, CGFloat size) {
    return [fontmanager fontWithFamily:family traits:traits weight:weight size:size];
}
NSFontManager* NSFontManager_sharedFontManager() {
    return [NSFontManager sharedFontManager];
}
int NSFontManager_traitsOfFont(NSFontManager* fontmanager,NSFont* fontObj) {
    return [fontmanager traitsOfFont:fontObj];
}
int NSFontManager_weightOfFont(NSFontManager* fontmanager,NSFont* fontObj) {
    return [fontmanager weightOfFont:fontObj];
}
