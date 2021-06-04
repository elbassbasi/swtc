#import "swt.h"
NSData* NSImage_TIFFRepresentation(NSImage* image) {
    return [image TIFFRepresentation];
}

void NSImage_addRepresentation(NSImage* image,NSImageRep* imageRep) {
    [image addRepresentation:imageRep];
}

NSImageRep* NSImage_bestRepresentationForDevice(NSImage* image,NSDictionary* deviceDescription) {
    return [image bestRepresentationForDevice:deviceDescription];
}

void NSImage_drawAtPoint(NSImage* image,NSPoint* point, NSRect* fromRect, int op, CGFloat delta) {
    [image drawAtPoint:*point fromRect:*fromRect operation:op fraction:delta];
}

void NSImage_drawInRect(NSImage* image,NSRect* rect, NSRect* fromRect, int op, CGFloat delta) {
    [image drawInRect:*rect fromRect:*fromRect operation:op fraction:delta];
}

NSImage* NSImage_imageNamed(NSString *name) {
    return [NSImage imageNamed:name];
}

NSImage* NSImage_initByReferencingFile(NSString *fileName) {
    return [[NSImage alloc] initByReferencingFile:fileName];
}

NSImage* NSImage_initWithContentsOfFile(NSString* fileName) {
    return [[NSImage alloc] initWithContentsOfFile:fileName];
}

NSImage* NSImage_initWithData(NSData* data) {
    return [[NSImage alloc] initWithData:data];
}

NSImage* NSImage_initWithIconRef(IconRef iconRef) {
    return [[NSImage alloc] initWithIconRef:iconRef];
}

NSImage* NSImage_initWithSize(NSSize* aSize) {
    return [[NSImage alloc] initWithSize:*aSize];
}

void NSImage_lockFocus(NSImage* image) {
    [image lockFocus];
}

void NSImage_removeRepresentation(NSImage* image,NSImageRep* imageRep) {
    [image removeRepresentation:imageRep];
}

NSArray* NSImage_representations(NSImage* image) {
    return [image representations];
}

void NSImage_setCacheMode(NSImage* image,int mode) {
    [image setCacheMode:mode];
}

void NSImage_setScalesWhenResized(NSImage* image,int flag) {
    [image setScalesWhenResized:flag];
}

void NSImage_setSize(NSImage* image,NSSize* aSize) {
   [image setSize:*aSize];
}

void NSImage_size(NSImage* image,NSSize* size) {
    *size = [image size];
}

void NSImage_unlockFocus(NSImage* image) {
    [image unlockFocus];
}
/*
 *  NSImageRep
 */
NSInteger NSImageRep_bitsPerSample(NSImageRep* image) {
    return [image bitsPerSample];
}

NSString* NSImageRep_colorSpaceName(NSImageRep* image) {
    return [image colorSpaceName];
}

int NSImageRep_drawInRect(NSImageRep* image,NSRect* rect) {
    return [image drawInRect:*rect];
}

int NSImageRep_hasAlpha(NSImageRep* image) {
    return [image hasAlpha];
}

NSImageRep* NSImageRep_imageRepWithContentsOfFile(NSString* filename) {
    return [NSImageRep imageRepWithContentsOfFile:filename];
}

NSInteger NSImageRep_pixelsHigh(NSImageRep* image) {
    return [image pixelsHigh];
}

NSInteger NSImageRep_pixelsWide(NSImageRep* image) {
    return [image pixelsWide];
}

void NSImageRep_setAlpha(NSImageRep* image,int flag) {
    [image setAlpha:flag];
}

void NSImageRep_setSize(NSImageRep* image,NSSize* aSize) {
    [image setSize:*aSize];
}
/*
 *  NSBitmapImageRep
 */
NSData* NSBitmapImageRep_TIFFRepresentation(NSBitmapImageRep* bitmap) {
    return [bitmap TIFFRepresentation];
}

unsigned char* NSBitmapImageRep_bitmapData(NSBitmapImageRep* bitmap) {
    return [bitmap bitmapData];
}

NSBitmapFormat* NSBitmapImageRep_bitmapFormat(NSBitmapImageRep* bitmap) {
    return [bitmap bitmapFormat];
}

NSInteger NSBitmapImageRep_bitsPerPixel(NSBitmapImageRep* bitmap) {
    return [bitmap bitsPerPixel];
}

NSInteger NSBitmapImageRep_bytesPerPlane(NSBitmapImageRep* bitmap) {
    return [bitmap bytesPerPlane];
}

NSInteger NSBitmapImageRep_bytesPerRow(NSBitmapImageRep* bitmap) {
    return [bitmap bytesPerRow];
}

NSColor* NSBitmapImageRep_colorAtX(NSBitmapImageRep* bitmap,NSInteger x, NSInteger y) {
    return [bitmap colorAtX:x y:y];
}

void NSBitmapImageRep_getBitmapDataPlanes(NSBitmapImageRep* bitmap,unsigned char* data) {
    [bitmap getBitmapDataPlanes:data];
}

NSBitmapImageRep* NSBitmapImageRep_imageRepWithData(NSData* data) {
    return [NSBitmapImageRep imageRepWithData:data];
}

NSBitmapImageRep* NSBitmapImageRep_initWithBitmapDataPlanes_0(unsigned char** planes, NSInteger width, NSInteger height,NSInteger bps, NSInteger spp, int alpha, int isPlanar, NSString* colorSpaceName, NSInteger bitmapFormat, NSInteger rBytes, NSInteger pBits) {
    return [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:planes pixelsWide:width pixelsHigh:height bitsPerSample:bps samplesPerPixel:spp hasAlpha:alpha isPlanar:isPlanar colorSpaceName:colorSpaceName bitmapFormat:bitmapFormat bytesPerRow:rBytes bitsPerPixel:pBits];
}

NSBitmapImageRep* NSBitmapImageRep_initWithBitmapDataPlanes_1(unsigned char** planes, NSInteger width, NSInteger height, NSInteger bps, NSInteger spp, int alpha, int isPlanar, NSString* colorSpaceName, NSInteger rBytes, NSInteger pBits) {
    return [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:planes pixelsWide:width pixelsHigh:height bitsPerSample:bps samplesPerPixel:spp hasAlpha:alpha isPlanar:isPlanar colorSpaceName:colorSpaceName bytesPerRow:rBytes bitsPerPixel:pBits];
}

NSBitmapImageRep* NSBitmapImageRep_initWithData(NSData* data) {
    return [[NSBitmapImageRep alloc] initWithData:data];
}

NSBitmapImageRep* NSBitmapImageRep_initWithFocusedViewRect(NSRect* rect) {
    return [[NSBitmapImageRep alloc] initWithFocusedViewRect:*rect];
}

int NSBitmapImageRep_isPlanar(NSBitmapImageRep* bitmap) {
    return [bitmap isPlanar];
}

NSInteger NSBitmapImageRep_numberOfPlanes(NSBitmapImageRep* bitmap) {
    return [bitmap numberOfPlanes];
}

NSInteger NSBitmapImageRep_samplesPerPixel(NSBitmapImageRep* bitmap) {
    return [bitmap samplesPerPixel];
}

NSImageRep* NSBitmapImageRep_imageRepWithContentsOfFile(NSString* filename) {
    return [NSBitmapImageRep imageRepWithContentsOfFile:filename];
}
NSString* _NSDeviceRGBColorSpace(){
    return NSDeviceRGBColorSpace;
}
/*
 *  SWTImageView
 */
@interface SWTImageView : NSImageView {
 @public w_widget* widget;
}
@end
@implementation SWTImageView

@end
w_widget* SWTImageView_get_widget(NSView* view){
    if([view isKindOfClass:SWTImageView.class]){
        return ((SWTImageView*)view)->widget;
    }
    return 0;
}
NSImageView* SWTImageView_new(w_widget* widget){
    SWTImageView* ImageView =  [[SWTImageView alloc] init];
    _w_toolkit_registre_class(_NS_CLASS_IMAGEVIEW,ImageView.class,SWTImageView_get_widget);
    ImageView->widget = widget;
    return ImageView;
}
/*
* NSImageView
*/
NSImage* NSImageView_image(NSImageView* view) {
    return [view image];
}

void NSImageView_setImage(NSImageView* view,NSImage* newImage) {
    [view setImage:newImage];
}

void NSImageView_setImageAlignment(NSImageView* view,int newAlign) {
    [view setImageAlignment:newAlign];
}

void NSImageView_setImageScaling(NSImageView* view,int newScaling) {
    [view setImageScaling:newScaling];
}

Class* NSImageView_cellClass() {
    //return [NSImageView cell]
    return 0;
}

void NSImageView_setCellClass(Class* factoryId) {
    //[NSImageView setCellClass:factoryId]
}
/*
 *  NSCursor
 */
void NSCursor_get_systems(NSCursor** cursors){
    cursors[W_CURSOR_HAND] = [NSCursor pointingHandCursor];
    cursors[W_CURSOR_ARROW] = [NSCursor arrowCursor];
 //   cursors[W_CURSOR_WAIT] = [NSCursor busyButClickableCursor];
    cursors[W_CURSOR_CROSS] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_APPSTARTING] = [NSCursor arrowCursor];
    cursors[W_CURSOR_HELP] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZEALL] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENESW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENS] = [NSCursor resizeUpDownCursor];
    cursors[W_CURSOR_SIZENWSE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZEWE] = [NSCursor resizeLeftRightCursor];
    cursors[W_CURSOR_SIZEN] = [NSCursor resizeUpCursor];
    cursors[W_CURSOR_SIZES] = [NSCursor resizeDownCursor];
    cursors[W_CURSOR_SIZEE] = [NSCursor resizeRightCursor];
    cursors[W_CURSOR_SIZEW] = [NSCursor resizeLeftCursor];
    cursors[W_CURSOR_SIZENE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZESE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZESW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_UPARROW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_IBEAM] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_NO] = [NSCursor operationNotAllowedCursor];
}
void NSCursor_set(NSCursor* cursor){
    [cursor set];
}
NSCursor* NSCursor_initWithImage(NSImage* image,NSPoint* hotSpot){
    return [[NSCursor alloc] initWithImage:image hotSpot:*hotSpot];
}
