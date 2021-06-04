/*
 * NSFont.h
 *
 *  Created on: Jan 25, 2021
 *      Author: azeddine
 */

#ifndef OSX_OBJC_NSFONT_H_
#define OSX_OBJC_NSFONT_H_
#include "NSArray.h"
typedef struct NSFont NSFont;
typedef struct NSFontManager NSFontManager;
/*
 * NSFont
 */
int* SWTFont_get_var(NSFont* font);
CGFloat NSFont_ascender(NSFont* font);
NSFont* NSFont_controlContentFontOfSize(CGFloat fontSize);
CGFloat NSFont_descender(NSFont* font);
NSString* NSFont_displayName(NSFont* font);
NSString* NSFont_familyName(NSFont* font);
NSString* NSFont_fontName(NSFont* font);
NSFont* NSFont_fontWithName(NSString* fontName, CGFloat fontSize);
CGFloat NSFont_leading(NSFont* font);
NSFont* NSFont_menuBarFontOfSize(CGFloat fontSize);
NSFont* NSFont_menuFontOfSize(CGFloat fontSize);
CGFloat NSFont_pointSize(NSFont* font);
CGFloat NSFont_smallSystemFontSize();
NSFont* NSFont_systemFontOfSize(CGFloat fontSize);
CGFloat NSFont_systemFontSize();
CGFloat NSFont_systemFontSizeForControlSize(int controlSize);
/*
 *  NSFontManager
 */
NSArray* NSFontManager_availableFontFamilies(NSFontManager* fontmanager);
NSArray* NSFontManager_availableFonts(NSFontManager* fontmanager);
NSArray* NSFontManager_availableMembersOfFontFamily(NSFontManager* fontmanager,NSString* fam);
NSFont* NSFontManager_convertFont(NSFontManager* fontmanager,NSFont* fontObj, int trait);
NSFont* NSFontManager_fontWithFamily(NSFontManager* fontmanager,NSString* family, int traits,int weight, CGFloat size);
NSFontManager* NSFontManager_sharedFontManager();
int NSFontManager_traitsOfFont(NSFontManager* fontmanager,NSFont* fontObj);
int NSFontManager_weightOfFont(NSFontManager* fontmanager,NSFont* fontObj);

#endif /* OSX_OBJC_NSFONT_H_ */
