/*
 * Name:        gdip.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef WIN32_GRAPHICS_GDIP_H_
#define WIN32_GRAPHICS_GDIP_H_
#include <stddef.h>
#include <math.h>
#include <windef.h>
#include <wingdi.h>
#include <basetyps.h>
#define WINGDIPAPI __stdcall
#define GDIPCONST const
#ifdef __cplusplus
extern "C" {
#endif
typedef struct GpTexture GpTexture;
typedef struct GpStringFormat GpStringFormat;
typedef struct GpRegion GpRegion;
typedef struct GpGraphics GpGraphics;
typedef struct GpPen GpPen;
typedef struct GpBrush GpBrush;
typedef struct GpSolidFill GpSolidFill;
typedef struct GpFont GpFont;
typedef struct GpFontCollection GpFontCollection;
typedef struct GpFontFamily GpFontFamily;
typedef struct GpMatrix GpMatrix;
typedef struct GpPath GpPath;
typedef struct GpImage GpImage;
typedef struct GpImageAttributes GpImageAttributes;
typedef enum GpStatus {
	Ok = 0,
	GenericError = 1,
	InvalidParameter = 2,
	OutOfMemory = 3,
} GpStatus;
typedef GpStatus Status;
typedef float REAL;
typedef DWORD GraphicsState;
typedef SHORT INT16;
typedef WORD UINT16;
typedef DWORD ARGB;
typedef INT PixelFormat;

#define PixelFormatIndexed ((INT) 0x00010000)
#define PixelFormatGDI ((INT) 0x00020000)
#define PixelFormatAlpha ((INT) 0x00040000)
#define PixelFormatPAlpha ((INT) 0x00080000)
#define PixelFormatExtended ((INT) 0x00100000)
#define PixelFormatCanonical ((INT) 0x00200000)
#define PixelFormatUndefined ((INT) 0)
#define PixelFormatDontCare ((INT) 0)
#define PixelFormat1bppIndexed ((INT) \
	(1 | (1<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat4bppIndexed ((INT) \
	(2 | (4<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat8bppIndexed ((INT) \
	(3 | (8<<8) | PixelFormatIndexed | PixelFormatGDI))
#define PixelFormat16bppGrayScale ((INT) \
	(4 | (16<<8) | PixelFormatExtended))
#define PixelFormat16bppRGB555 ((INT) \
	(5 | (16<<8) | PixelFormatGDI))
#define PixelFormat16bppRGB565 ((INT) \
	(6 | (16<<8) | PixelFormatGDI))
#define PixelFormat16bppARGB1555 ((INT) \
	(7 | (16<<8) | PixelFormatAlpha | PixelFormatGDI))
#define PixelFormat24bppRGB ((INT) \
	(8 | (24<<8) | PixelFormatGDI))
#define PixelFormat32bppRGB ((INT) \
	(9 | (32<<8) | PixelFormatGDI))
#define PixelFormat32bppARGB ((INT) \
	(10 | (32<<8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical))
#define PixelFormat32bppPARGB ((INT) \
	(11 | (32<<8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI))
#define PixelFormat48bppRGB ((INT) \
	(12 | (48<<8) | PixelFormatExtended))
#define PixelFormat64bppARGB ((INT) \
	(13 | (64<<8) | PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended))
#define PixelFormat64bppPARGB ((INT) \
	(14 | (64<<8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended))
#define PixelFormatMax ((INT) 15)
typedef struct GpPoint {
	INT X;
	INT Y;
} GpPoint;
typedef struct GpPointF {
	REAL X;
	REAL Y;
} GpPointF;
typedef struct GpRectF {
	REAL X;
	REAL Y;
	REAL Width;
	REAL Height;
} GpRectF;
typedef struct GpRect {
	INT X;
	INT Y;
	INT Width;
	INT Height;
} GpRect;
typedef enum DebugEventLevel {
	DebugEventLevelFatal, DebugEventLevelWarning
} DebugEventLevel;

typedef VOID(WINAPI* DebugEventProc)(DebugEventLevel level, CHAR* message);

typedef Status(WINAPI* NotificationHookProc)(OUT ULONG_PTR* token);
typedef VOID(WINAPI* NotificationUnhookProc)(ULONG_PTR token);

typedef struct GdiplusStartupInput {
	UINT32 GdiplusVersion;
	DebugEventProc DebugEventCallback;
	BOOL SuppressBackgroundThread;
	BOOL SuppressExternalCodecs;
} GdiplusStartupInput;

typedef struct GdiplusStartupOutput {
	NotificationHookProc NotificationHook;
	NotificationUnhookProc NotificationUnhook;
} GdiplusStartupOutput;
GpStatus WINGDIPAPI GdiplusStartup(ULONG_PTR*,GDIPCONST GdiplusStartupInput*,GdiplusStartupOutput*);
typedef enum GpUnit {
	UnitWorld = 0,
	UnitDisplay = 1,
	UnitPixel = 2,
	UnitPoint = 3,
	UnitInch = 4,
	UnitDocument = 5,
	UnitMillimeter = 6
} GpUnit;
typedef enum GpFillMode {
	FillModeAlternate = 0,
	FillModeWinding = 1
} GpFillMode;
/* Font functions */
typedef enum FontStyle {
	FontStyleRegular = 0,
	FontStyleBold = 1,
	FontStyleItalic = 2,
	FontStyleBoldItalic = 3,
	FontStyleUnderline = 4,
	FontStyleStrikeout = 8
} FontStyle;
typedef enum StringFormatFlags {
	StringFormatFlagsDirectionRightToLeft = 0x00000001,
	StringFormatFlagsDirectionVertical = 0x00000002,
	StringFormatFlagsNoFitBlackBox = 0x00000004,
	StringFormatFlagsDisplayFormatControl = 0x00000020,
	StringFormatFlagsNoFontFallback = 0x00000400,
	StringFormatFlagsMeasureTrailingSpaces = 0x00000800,
	StringFormatFlagsNoWrap = 0x00001000,
	StringFormatFlagsLineLimit = 0x00002000,
	StringFormatFlagsNoClip = 0x00004000
} StringFormatFlags;
typedef enum HotkeyPrefix {
	HotkeyPrefixNone = 0,
	HotkeyPrefixShow = 1,
	HotkeyPrefixHide = 2
} HotkeyPrefix;
typedef enum ColorAdjustType {
	ColorAdjustTypeDefault = 0,
	ColorAdjustTypeBitmap = 1,
	ColorAdjustTypeBrush = 2,
	ColorAdjustTypePen = 3,
	ColorAdjustTypeText = 4,
	ColorAdjustTypeCount = 5,
	ColorAdjustTypeAny = 6
} ColorAdjustType;

typedef struct ColorMatrix {
	REAL m[5][5];
} ColorMatrix;
typedef enum ColorMatrixFlags {
	ColorMatrixFlagsDefault = 0,
	ColorMatrixFlagsSkipGrays = 1,
	ColorMatrixFlagsAltGray = 2
} ColorMatrixFlags;
GpStatus WINGDIPAPI GdipDeleteFont(GpFont*);
GpStatus WINGDIPAPI GdipCreateFontFromLogfontW(HDC,GDIPCONST LOGFONTW*,GpFont**);
GpStatus WINGDIPAPI GdipCreateFont(GDIPCONST GpFontFamily*,REAL,INT,GpUnit,GpFont**);
GpStatus WINGDIPAPI GdipGetLogFontW(GpFont*,GpGraphics*,LOGFONTW*);
GpStatus WINGDIPAPI GdipGetFamily(GpFont*,GpFontFamily**);
GpStatus WINGDIPAPI GdipGetFontSize(GpFont*,REAL*);
GpStatus WINGDIPAPI GdipGetFontStyle(GpFont*,INT*);
/* FontFamily functions */
GpStatus WINGDIPAPI GdipCreateFontFamilyFromName(GDIPCONST WCHAR*,GpFontCollection*,GpFontFamily**);
GpStatus WINGDIPAPI GdipDeleteFontFamily(GpFontFamily*);
/* Text functions */
GpStatus WINGDIPAPI GdipDrawString(GpGraphics*,GDIPCONST WCHAR*,INT,GDIPCONST GpFont*,GDIPCONST GpRectF*,GDIPCONST GpStringFormat*,GDIPCONST GpBrush*);
GpStatus WINGDIPAPI GdipMeasureString(GpGraphics*,GDIPCONST WCHAR*,INT,GDIPCONST GpFont*,GDIPCONST GpRectF*,GDIPCONST GpStringFormat*,GpRectF*,INT*,INT*);
GpStatus WINGDIPAPI GdipDrawDriverString(GpGraphics*,GDIPCONST UINT16*,INT,GDIPCONST GpFont*,GDIPCONST GpBrush*,GDIPCONST GpPointF*,INT,GDIPCONST GpMatrix*);
GpStatus WINGDIPAPI GdipMeasureDriverString(GpGraphics*,GDIPCONST UINT16*,INT,GDIPCONST GpFont*,GDIPCONST GpPointF*,INT,GDIPCONST GpMatrix*,GpRectF*);
GpStatus WINGDIPAPI GdipStringFormatGetGenericTypographic(GpStringFormat**);
GpStatus WINGDIPAPI GdipCloneStringFormat(GDIPCONST GpStringFormat*,GpStringFormat**);
GpStatus WINGDIPAPI GdipGetStringFormatFlags(GDIPCONST GpStringFormat*,INT*);
GpStatus WINGDIPAPI GdipSetStringFormatFlags(GpStringFormat*,INT);
GpStatus WINGDIPAPI GdipSetStringFormatTabStops(GpStringFormat*,REAL,INT,GDIPCONST REAL*);
GpStatus WINGDIPAPI GdipSetStringFormatHotkeyPrefix(GpStringFormat*,INT);
GpStatus WINGDIPAPI GdipDeleteStringFormat(GpStringFormat*);
/* Matrix functions */
typedef enum GpMatrixOrder {
	MatrixOrderPrepend = 0,
	MatrixOrderAppend = 1
} GpMatrixOrder;
GpStatus WINGDIPAPI GdipCreateMatrix2(REAL,REAL,REAL,REAL,REAL,REAL,GpMatrix**);
GpStatus WINGDIPAPI GdipDeleteMatrix(GpMatrix*);
GpStatus WINGDIPAPI GdipTransformMatrixPoints(GpMatrix*,GpPointF*,INT);
GpStatus WINGDIPAPI GdipSetMatrixElements(GpMatrix*,REAL,REAL,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipMultiplyMatrix(GpMatrix*,GpMatrix*,GpMatrixOrder);
GpStatus WINGDIPAPI GdipInvertMatrix(GpMatrix*);
GpStatus WINGDIPAPI GdipRotateMatrix(GpMatrix*,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipScaleMatrix(GpMatrix*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipShearMatrix(GpMatrix*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipTranslateMatrix(GpMatrix*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipIsMatrixIdentity(GDIPCONST GpMatrix*,BOOL*);
GpStatus WINGDIPAPI GdipGetMatrixElements(GDIPCONST GpMatrix*,REAL*);
/* Brush functions */
typedef enum GpBrushType {
	BrushTypeSolidColor = 0,
	BrushTypeHatchFill = 1,
	BrushTypeTextureFill = 2,
	BrushTypePathGradient = 3,
	BrushTypeLinearGradient = 4
} GpBrushType;
GpStatus WINGDIPAPI GdipCloneBrush(GpBrush*,GpBrush**);
GpStatus WINGDIPAPI GdipDeleteBrush(GpBrush*);
GpStatus WINGDIPAPI GdipGetBrushType(GpBrush*,GpBrushType*);
/* SolidBrush functions */
GpStatus WINGDIPAPI GdipCreateSolidFill(ARGB,GpBrush**);
/* TextureBrush functions */
typedef enum GpWrapMode {
	WrapModeTile = 0,
	WrapModeTileFlipX = 1,
	WrapModeTileFlipY = 2,
	WrapModeTileFlipXY = 3,
	WrapModeClamp = 4
} GpWrapMode;
GpStatus WINGDIPAPI GdipCreateTexture(GpImage*,GpWrapMode,GpBrush**);
GpStatus WINGDIPAPI GdipCreateTexture2(GpImage*,GpWrapMode,REAL,REAL,REAL,REAL,GpBrush**);
GpStatus WINGDIPAPI GdipCreateTexture2I(GpImage*,GpWrapMode,INT,INT,INT,INT,GpBrush**);
GpStatus WINGDIPAPI GdipScaleTextureTransform(GpBrush*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipTranslateTextureTransform(GpBrush*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipResetTextureTransform(GpBrush*);
/* LinearGradientBrush functions */
GpStatus WINGDIPAPI GdipCreateLineBrush(GDIPCONST GpPointF*,GDIPCONST GpPointF*,ARGB,ARGB,GpWrapMode,GpBrush**);
GpStatus WINGDIPAPI GdipScaleLineTransform(GpBrush*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipTranslateLineTransform(GpBrush*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipResetLineTransform(GpBrush*);
/* Pen functions */
typedef enum GpDashStyle {
	DashStyleSolid = 0,
	DashStyleDash = 1,
	DashStyleDot = 2,
	DashStyleDashDot = 3,
	DashStyleDashDotDot = 4,
	DashStyleCustom = 5
} GpDashStyle;
typedef enum GpLineJoin {
	LineJoinMiter = 0,
	LineJoinBevel = 1,
	LineJoinRound = 2,
	LineJoinMiterClipped = 3
} GpLineJoin;
typedef enum GpLineCap {
	LineCapFlat = 0,
	LineCapSquare = 1,
	LineCapRound = 2,
	LineCapTriangle = 3,
	LineCapNoAnchor = 16,
	LineCapSquareAnchor = 17,
	LineCapRoundAnchor = 18,
	LineCapDiamondAnchor = 19,
	LineCapArrowAnchor = 20,
	LineCapCustom = 255
} GpLineCap;
typedef enum GpDashCap {
	DashCapFlat = 0,
	DashCapRound = 2,
	DashCapTriangle = 3
} GpDashCap;
GpStatus WINGDIPAPI GdipCreatePen2(GpBrush*,REAL,GpUnit,GpPen**);
GpStatus WINGDIPAPI GdipDeletePen(GpPen*);
GpStatus WINGDIPAPI GdipSetPenBrushFill(GpPen*,GpBrush*);
GpStatus WINGDIPAPI GdipSetPenWidth(GpPen*,REAL);
GpStatus WINGDIPAPI GdipSetPenDashArray(GpPen*,GDIPCONST REAL*,INT);
GpStatus WINGDIPAPI GdipSetPenDashStyle(GpPen*,GpDashStyle);
GpStatus WINGDIPAPI GdipSetPenDashOffset(GpPen*,REAL);
GpStatus WINGDIPAPI GdipSetPenDashStyle(GpPen*,GpDashStyle);
GpStatus WINGDIPAPI GdipSetPenMiterLimit(GpPen*,REAL);
GpStatus WINGDIPAPI GdipSetPenLineJoin(GpPen*,GpLineJoin);
GpStatus WINGDIPAPI GdipSetPenLineCap197819(GpPen*,GpLineCap,GpLineCap,GpDashCap);
/* Graphics functions */
typedef enum SmoothingMode {
	SmoothingModeDefault = 0,
	SmoothingModeHighSpeed = 1,
	SmoothingModeHighQuality = 2,
	SmoothingModeNone = 3,
	SmoothingModeAntiAlias8x4 = 4,
	SmoothingModeAntiAlias = 4,
	SmoothingModeAntiAlias8x8 = 5
} SmoothingMode;
typedef enum InterpolationMode {
	InterpolationModeInvalid = -1,
	InterpolationModeDefault = 0,
	InterpolationModeLowQuality = 1,
	InterpolationModeHighQuality = 2,
	InterpolationModeBilinear = 3,
	InterpolationModeBicubic = 4,
	InterpolationModeNearestNeighbor = 5,
	InterpolationModeHighQualityBilinear = 6,
	InterpolationModeHighQualityBicubic = 7
} InterpolationMode;
typedef enum GpFlushIntention {
	FlushIntentionFlush = 0,
	FlushIntentionSync = 1
} GpFlushIntention;
typedef enum TextRenderingHint {
	TextRenderingHintSystemDefault = 0,
	TextRenderingHintSingleBitPerPixelGridFit = 1,
	TextRenderingHintSingleBitPerPixel = 2,
	TextRenderingHintAntiAliasGridFit = 3,
	TextRenderingHintAntiAlias = 4,
	TextRenderingHintClearTypeGridFit = 5
} TextRenderingHint;
typedef enum PixelOffsetMode {
	PixelOffsetModeInvalid = -1,
	PixelOffsetModeDefault = 0,
	PixelOffsetModeHighSpeed = 1,
	PixelOffsetModeHighQuality = 2,
	PixelOffsetModeNone = 3,
	PixelOffsetModeHalf = 4
} PixelOffsetMode;
typedef enum CombineMode {
	CombineModeReplace = 0,
	CombineModeIntersect = 1,
	CombineModeUnion = 2,
	CombineModeXor = 3,
	CombineModeExclude = 4,
	CombineModeComplement = 5
} CombineMode;
GpStatus WINGDIPAPI GdipDeleteGraphics(GpGraphics*);
GpStatus WINGDIPAPI GdipReleaseDC(GpGraphics*,HDC);
GpStatus WINGDIPAPI GdipGetDC(GpGraphics*,HDC*);
GpStatus WINGDIPAPI GdipCreateFromHDC(HDC,GpGraphics**);
GpStatus WINGDIPAPI GdipSetPageUnit(GpGraphics*,GpUnit);
GpStatus WINGDIPAPI GdipFlush(GpGraphics*,GpFlushIntention);
GpStatus WINGDIPAPI GdipGetWorldTransform(GpGraphics*,GpMatrix*);
GpStatus WINGDIPAPI GdipTranslateWorldTransform(GpGraphics*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipDrawArc(GpGraphics*,GpPen*,REAL,REAL,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipDrawLine(GpGraphics*,GpPen*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipDrawLines(GpGraphics*,GpPen*,GDIPCONST GpPointF*,INT);
GpStatus WINGDIPAPI GdipDrawLinesI(GpGraphics*,GpPen*,GDIPCONST GpPoint*,INT);
GpStatus WINGDIPAPI GdipDrawPath(GpGraphics*,GpPen*,GpPath*);
GpStatus WINGDIPAPI GdipDrawEllipse(GpGraphics*,GpPen*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipFillRectangle(GpGraphics*,GpBrush*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipDrawPolygon(GpGraphics*,GpPen*,GDIPCONST GpPointF*,INT);
GpStatus WINGDIPAPI GdipDrawPolygonI(GpGraphics*,GpPen*,GDIPCONST GpPoint*,INT);
GpStatus WINGDIPAPI GdipDrawRectangle(GpGraphics*,GpPen*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipDrawRectangleI(GpGraphics*,GpPen*,INT,INT,INT,INT);
GpStatus WINGDIPAPI GdipSaveGraphics(GpGraphics*,GraphicsState*);
GpStatus WINGDIPAPI GdipRestoreGraphics(GpGraphics*,GraphicsState);
GpStatus WINGDIPAPI GdipScaleWorldTransform(GpGraphics*,REAL,REAL,GpMatrixOrder);
GpStatus WINGDIPAPI GdipSetSmoothingMode(GpGraphics*,SmoothingMode);
GpStatus WINGDIPAPI GdipGetSmoothingMode(GpGraphics*,SmoothingMode*);
GpStatus WINGDIPAPI GdipFillPie(GpGraphics*,GpBrush*,REAL,REAL,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipFillPolygon(GpGraphics*,GpBrush*,GDIPCONST GpPointF*,INT,GpFillMode);
GpStatus WINGDIPAPI GdipFillPolygonI(GpGraphics*,GpBrush*,GDIPCONST GpPoint*,INT,GpFillMode);
GpStatus WINGDIPAPI GdipGetInterpolationMode(GpGraphics*,InterpolationMode*);
GpStatus WINGDIPAPI GdipSetInterpolationMode(GpGraphics*,InterpolationMode);
GpStatus WINGDIPAPI GdipSetTextRenderingHint(GpGraphics*,TextRenderingHint);
GpStatus WINGDIPAPI GdipGetTextRenderingHint(GpGraphics*,TextRenderingHint*);
GpStatus WINGDIPAPI GdipSetPixelOffsetMode(GpGraphics*,PixelOffsetMode);
GpStatus WINGDIPAPI GdipGetClip(GpGraphics*,GpRegion*);
GpStatus WINGDIPAPI GdipFillEllipse(GpGraphics*,GpBrush*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipSetWorldTransform(GpGraphics*,GpMatrix*);
GpStatus WINGDIPAPI GdipResetClip(GpGraphics*);
GpStatus WINGDIPAPI GdipDrawImageRect(GpGraphics*,GpImage*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipDrawImageRectRect(GpGraphics*,GpImage*,REAL,REAL,REAL,REAL,REAL,REAL,REAL,REAL,GpUnit,GDIPCONST GpImageAttributes*,int,VOID*);
GpStatus WINGDIPAPI GdipSetClipPath(GpGraphics*,GpPath*,CombineMode);
GpStatus WINGDIPAPI GdipGetVisibleClipBounds(GpGraphics*,GpRectF*);
/* GraphicsPath functions */
typedef enum PathPointType {
	PathPointTypeStart = 0x00,
	PathPointTypeLine = 0x01,
	PathPointTypeBezier = 0x03,
	PathPointTypeBezier3 = 0x03,
	PathPointTypePathTypeMask = 0x07,
	PathPointTypePathDashMode = 0x10,
	PathPointTypePathMarker = 0x20,
	PathPointTypeCloseSubpath = 0x80
} PathPointType;
GpStatus WINGDIPAPI GdipCreatePath(GpFillMode,GpPath**);
GpStatus WINGDIPAPI GdipDeletePath(GpPath*);
GpStatus WINGDIPAPI GdipAddPathArc(GpPath*,REAL,REAL,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipTransformPath(GpPath*,GpMatrix*);
GpStatus WINGDIPAPI GdipClosePathFigure(GpPath*);
GpStatus WINGDIPAPI GdipFillPath(GpGraphics*,GpBrush*,GpPath*);
GpStatus WINGDIPAPI GdipSetPathFillMode(GpPath*,GpFillMode);
GpStatus WINGDIPAPI GdipAddPathString(GpPath*,GDIPCONST WCHAR*,INT,GDIPCONST GpFontFamily*,INT,REAL,GDIPCONST GpRectF*,GDIPCONST GpStringFormat*);
GpStatus WINGDIPAPI GdipGetPathLastPoint(GpPath*,GpPointF*);
GpStatus WINGDIPAPI GdipAddPathBezier(GpPath*,REAL,REAL,REAL,REAL,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipAddPathBeziers(GpPath*,GDIPCONST GpPointF*,INT);
GpStatus WINGDIPAPI GdipStartPathFigure(GpPath*);
GpStatus WINGDIPAPI GdipAddPathLine(GpPath*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipAddPathRectangle(GpPath*,REAL,REAL,REAL,REAL);
GpStatus WINGDIPAPI GdipClonePath(GpPath*,GpPath**);
GpStatus WINGDIPAPI GdipFlattenPath(GpPath*,GpMatrix*,REAL);
GpStatus WINGDIPAPI GdipAddPathPath(GpPath*,GDIPCONST GpPath*,BOOL);
GpStatus WINGDIPAPI GdipIsOutlineVisiblePathPoint(GpPath*,REAL,REAL,GpPen*,GpGraphics*,BOOL*);
GpStatus WINGDIPAPI GdipIsVisiblePathPoint(GpPath*,REAL,REAL,GpGraphics*,BOOL*);
GpStatus WINGDIPAPI GdipGetPathWorldBounds(GpPath*,GpRectF*,GDIPCONST GpMatrix*,GDIPCONST GpPen*);
GpStatus WINGDIPAPI GdipGetPointCount(GpPath*,INT*);
GpStatus WINGDIPAPI GdipGetPathTypes(GpPath*,BYTE*,INT);
GpStatus WINGDIPAPI GdipGetPathPoints(GpPath*,GpPointF*,INT);
/* Region functions */
GpStatus WINGDIPAPI GdipCreateRegion(GpRegion**);
GpStatus WINGDIPAPI GdipIsInfiniteRegion(GpRegion*,GpGraphics*,BOOL*);
GpStatus WINGDIPAPI GdipDeleteRegion(GpRegion*);
GpStatus WINGDIPAPI GdipCreateRegionHrgn(HRGN,GpRegion**);
GpStatus WINGDIPAPI GdipSetClipRegion(GpGraphics*,GpRegion*,CombineMode);
GpStatus WINGDIPAPI GdipGetRegionHRgn(GpRegion*,GpGraphics*,HRGN*);
/* Image functions */
typedef enum ImageLockMode {
	ImageLockModeRead = 1,
	ImageLockModeWrite = 2,
	ImageLockModeUserInputBuf = 4
} ImageLockMode;
typedef struct BitmapData {
	UINT Width;
	UINT Height;
	INT Stride;
	INT PixelFormat;  /* MSDN: "PixelFormat PixelFormat;" */
	VOID *Scan0;
	UINT_PTR Reserved;
} BitmapData;
GpStatus WINGDIPAPI GdipDisposeImage(GpImage*);
GpStatus WINGDIPAPI GdipCreateHICONFromBitmap(GpImage*,HICON*);
GpStatus WINGDIPAPI GdipCreateBitmapFromStream(IStream*,GpImage**);
GpStatus WINGDIPAPI GdipCreateBitmapFromFile(GDIPCONST WCHAR*,GpImage**);
GpStatus WINGDIPAPI GdipCloneImage(GpImage*,GpImage**);
GpStatus WINGDIPAPI GdipGetImageWidth(GpImage*,UINT*);
GpStatus WINGDIPAPI GdipGetImageHeight(GpImage*,UINT*);
GpStatus WINGDIPAPI GdipGetImageGraphicsContext(GpImage*,GpGraphics**);
GpStatus WINGDIPAPI GdipGetImagePixelFormat(GpImage*,PixelFormat*);
/* Bitmap functions */
GpStatus WINGDIPAPI GdipCreateBitmapFromHBITMAP(HBITMAP,HPALETTE,GpImage**);
GpStatus WINGDIPAPI GdipCreateHBITMAPFromBitmap(GpImage*,HBITMAP*,ARGB);
GpStatus WINGDIPAPI GdipCreateBitmapFromHICON(HICON,GpImage**);
GpStatus WINGDIPAPI GdipCreateBitmapFromScan0(INT,INT,INT,PixelFormat,BYTE*,GpImage**);
GpStatus WINGDIPAPI GdipBitmapLockBits(GpImage*,GDIPCONST GpRect*,UINT,PixelFormat,BitmapData*);
GpStatus WINGDIPAPI GdipBitmapUnlockBits(GpImage*,BitmapData*);
/* ImageAttributes functions */
GpStatus WINGDIPAPI GdipCreateImageAttributes(GpImageAttributes**);
GpStatus WINGDIPAPI GdipCloneImageAttributes(GDIPCONST GpImageAttributes*,GpImageAttributes**);
GpStatus WINGDIPAPI GdipDisposeImageAttributes(GpImageAttributes*);
GpStatus WINGDIPAPI GdipSetImageAttributesWrapMode(GpImageAttributes*,GpWrapMode,ARGB,BOOL);
GpStatus WINGDIPAPI GdipSetImageAttributesColorMatrix(GpImageAttributes*,ColorAdjustType,BOOL,GDIPCONST ColorMatrix*,GDIPCONST ColorMatrix*,ColorMatrixFlags);


#ifdef __cplusplus
}
#endif
#endif /* WIN32_GRAPHICS_GDIP_H_ */
