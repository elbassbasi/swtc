/*
 * Name:        gc.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "gc.h"
#include "../widgets/toolkit.h"
#include "gdip.h"
GpMatrix* _w_graphics_identity(w_graphics *gc);
void w_graphics_init(w_graphics *gc) {
	_W_GRAPHICS(gc)->handle = 0;
}
void _w_graphics_init(w_graphics *gc, HDC handle) {
	memset(gc, 0, sizeof(_w_graphics));
	_W_GRAPHICS(gc)->handle = handle;
	_W_GRAPHICS(gc)->alpha = 0xFF;
}
unsigned char LINE_DOT_ZERO[] = { 3, 3 };
unsigned char LINE_DASH_ZERO[] = { 18, 6 };
unsigned char LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6 };
unsigned char LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3 };

int _cos(int angle, int length) {
	return (int) (cosf(angle * (M_PI / 180)) * length);
}

int _sin(int angle, int length) {
	return (int) (sinf(angle * (M_PI / 180)) * length);
}
wresult w_graphics_check(w_graphics *gc, int mask) {
	if (gc == 0 || _W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	int state = _W_GRAPHICS(gc)->state;
	if ((state & mask) == mask)
		return 1;
	state = (state ^ mask) & mask;
	_W_GRAPHICS(gc)->state |= mask;
	GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
	if (gdipGraphics != 0) {
		GpPen *pen = _W_GRAPHICS(gc)->gdipPen;
		float width = _W_GRAPHICS(gc)->line.width;
		if ((state & GRAPHICS_STATE_FOREGROUND) != 0
				|| (pen == 0
						&& (state
								& (GRAPHICS_STATE_LINE_WIDTH
										| GRAPHICS_STATE_LINE_STYLE
										| GRAPHICS_STATE_LINE_MITERLIMIT
										| GRAPHICS_STATE_LINE_JOIN
										| GRAPHICS_STATE_LINE_CAP)) != 0)) {
			if (_W_GRAPHICS(gc)->gdipFgBrush != 0)
				GdipDeleteBrush(_W_GRAPHICS(gc)->gdipFgBrush);
			_W_GRAPHICS(gc)->gdipFgBrush = 0;
			GpBrush *brush = 0;
			w_pattern *pattern = _W_GRAPHICS(gc)->foregroundPattern;
			if (pattern != 0) {
				brush = _W_PATTERN(pattern)->handle;
				if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
					GpBrushType type = BrushTypeSolidColor;
					GpBrush *newbrush;
					if (brush != 0) {
						GdipGetBrushType(brush, &type);
						if (type == BrushTypeTextureFill) {
							newbrush = 0;
							GdipCloneBrush((GpBrush*) brush,
									(GpBrush**) &newbrush);
							if (newbrush == 0)
								return W_ERROR_NO_HANDLES;
							brush = newbrush;
							GdipScaleTextureTransform(brush, -1, 1,
									MatrixOrderPrepend);
							_W_GRAPHICS(gc)->gdipFgBrush = brush;
						}
					}
				}
			} else {
				int foreground = _W_GRAPHICS(gc)->foreground;
				int rgb = ((foreground >> 16) & 0xFF) | (foreground & 0xFF00)
						| ((foreground & 0xFF) << 16);
				ARGB color = _W_GRAPHICS(gc)->alpha << 24 | rgb;
				GdipCreateSolidFill(color, &brush);
				_W_GRAPHICS(gc)->gdipFgBrush = brush;
			}
			if (pen != 0) {
				GdipSetPenBrushFill(pen, brush);
			} else {
				GdipCreatePen2(brush, width, UnitWorld, &pen);
				_W_GRAPHICS(gc)->gdipPen = pen;
			}
		}
		if ((state & GRAPHICS_STATE_LINE_WIDTH) != 0) {
			GdipSetPenWidth(pen, width);
			switch (_W_GRAPHICS(gc)->line.style) {
			case W_LINE_CUSTOM:
				state |= GRAPHICS_STATE_LINE_STYLE;
			}
		}
		if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
			//float dashesOffset = 0;
			float *dashes = 0;
			int dashes_length;
			float width = _W_GRAPHICS(gc)->line.width;
			int is_custom_dash = W_FALSE;
			//float[] dashes = null;
			float dashOffset = 0;
			GpDashStyle dashStyle = DashStyleSolid;
			switch (_W_GRAPHICS(gc)->line.style) {
			case W_LINE_SOLID:
				break;
			case W_LINE_DOT:
				dashStyle = DashStyleDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DOT_ZERO)
							/ sizeof(LINE_DOT_ZERO[0]);
					dashes = (float*) LINE_DOT_ZERO;
				}
				break;
			case W_LINE_DASH:
				dashStyle = DashStyleDash;
				if (width == 0) {

					dashes_length = sizeof(LINE_DASH_ZERO)
							/ sizeof(LINE_DASH_ZERO[0]);
					dashes = (float*) LINE_DASH_ZERO;
				}
				break;
			case W_LINE_DASHDOT:
				dashStyle = DashStyleDashDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DASHDOT_ZERO)
							/ sizeof(LINE_DASHDOT_ZERO[0]);
					dashes = (float*) LINE_DASHDOT_ZERO;
				}
				break;
			case W_LINE_DASHDOTDOT:
				dashStyle = DashStyleDashDotDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DASHDOTDOT_ZERO)
							/ sizeof(LINE_DASHDOTDOT_ZERO[0]);
					dashes = (float*) LINE_DASHDOTDOT_ZERO;
				}
				break;
			case W_LINE_CUSTOM: {
				if (_W_GRAPHICS(gc)->line.dashes != 0) {
					is_custom_dash = W_TRUE;
					int m = width;
					if (m < 1)
						m = 1;
					dashOffset = _W_GRAPHICS(gc)->line.dashOffset / m;
					dashes_length = _W_GRAPHICS(gc)->line.length * 2;
					dashes = malloc(
					_W_GRAPHICS(gc)->line.length * 2 * sizeof(float));
					for (int i = 0; i < _W_GRAPHICS(gc)->line.length; i++) {
						float dash = _W_GRAPHICS(gc)->line.dashes[i] / m;
						dashes[i] = dash;
						dashes[i + _W_GRAPHICS(gc)->line.length] = dash;
					}
				}
			}
			}
			if (dashes != 0) {
				REAL *_dashes;
				if (is_custom_dash) {
					_dashes = dashes;
				} else {
					REAL dashesArray[6];
					for (int i = 0; i < dashes_length; i++) {
						dashesArray[i] = ((unsigned char*) dashes)[i];
					}
					_dashes = dashesArray;
				}
				GdipSetPenDashArray(pen, _dashes, dashes_length);
				GdipSetPenDashStyle(pen, DashStyleCustom);
				GdipSetPenDashOffset(pen, dashOffset);
				if (is_custom_dash) {
					free(dashes);
				}
			} else {
				GdipSetPenDashStyle(pen, dashStyle);
			}
		}
		if ((state & GRAPHICS_STATE_LINE_MITERLIMIT) != 0) {
			GdipSetPenMiterLimit(pen,
			_W_GRAPHICS(gc)->line.miterLimit);
		}
		if ((state & GRAPHICS_STATE_LINE_JOIN) != 0) {
			GpLineJoin joinStyle = LineJoinMiter;
			switch (_W_GRAPHICS(gc)->line.join) {
			case W_JOIN_MITER:
				joinStyle = LineJoinMiter;
				break;
			case W_JOIN_BEVEL:
				joinStyle = LineJoinBevel;
				break;
			case W_JOIN_ROUND:
				joinStyle = LineJoinRound;
				break;
			}
			GdipSetPenLineJoin(pen, joinStyle);
		}
		if ((state & GRAPHICS_STATE_LINE_CAP) != 0) {
			GpLineCap capStyle = LineCapFlat;
			GpDashCap dashCap = DashCapFlat;
			switch (_W_GRAPHICS(gc)->line.cap) {
			case W_CAP_FLAT:
				capStyle = LineCapFlat;
				break;
			case W_CAP_ROUND:
				capStyle = LineCapRound;
				dashCap = DashCapRound;
				break;
			case W_CAP_SQUARE:
				capStyle = LineCapSquare;
				break;
			}
			GdipSetPenLineCap197819(pen, capStyle, capStyle, dashCap);
		}
		if ((state & GRAPHICS_STATE_BACKGROUND) != 0) {
			if (_W_GRAPHICS(gc)->gdipBgBrush != 0) {
				GdipDeleteBrush(_W_GRAPHICS(gc)->gdipBgBrush);
			}
			_W_GRAPHICS(gc)->gdipBgBrush = 0;
			w_pattern *pattern = _W_GRAPHICS(gc)->backgroundPattern;
			if (pattern != 0) {
				_W_GRAPHICS(gc)->gdipBrush = _W_PATTERN(pattern)->handle;
				if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
					GpBrushType type = BrushTypeSolidColor;
					GdipGetBrushType(_W_GRAPHICS(gc)->gdipBrush, &type);
					GpBrush *brush;
					if (type == BrushTypeTextureFill) {
						brush = 0;
						GdipCloneBrush(_W_GRAPHICS(gc)->gdipBrush, &brush);
						if (brush == 0)
							return W_ERROR_NO_HANDLES;
						GdipScaleTextureTransform(brush, -1, 1,
								MatrixOrderPrepend);
						_W_GRAPHICS(gc)->gdipBrush =
						_W_GRAPHICS(gc)->gdipBgBrush = brush;
					}
				}
			} else {
				GpBrush *brush = 0;
				int background = _W_GRAPHICS(gc)->background;
				int rgb = ((background >> 16) & 0xFF) | (background & 0xFF00)
						| ((background & 0xFF) << 16);
				int color = _W_GRAPHICS(gc)->alpha << 24 | rgb;
				GdipCreateSolidFill(color, &brush);
				_W_GRAPHICS(gc)->gdipBrush = _W_GRAPHICS(gc)->gdipBgBrush =
						brush;
			}
		}
		if ((state & GRAPHICS_STATE_FONT) != 0) {
			w_font *font = _W_GRAPHICS(gc)->font;
			if (font != 0) {
				SelectObject(_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle);
				HFONT hFont = 0;
				GpFont *gdipFont = w_graphics_create_gdip_font(
				_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle,
				_W_GRAPHICS(gc)->gdipGraphics, win_toolkit->fontCollection, 0,
						&hFont);
				if (hFont != 0)
					SelectObject(_W_GRAPHICS(gc)->handle, hFont);
				if (_W_GRAPHICS(gc)->hGDIFont != 0)
					DeleteObject(_W_GRAPHICS(gc)->hGDIFont);
				_W_GRAPHICS(gc)->hGDIFont = hFont;
				if (_W_GRAPHICS(gc)->gdipFont != 0)
					GdipDeleteFont(_W_GRAPHICS(gc)->gdipFont);
				_W_GRAPHICS(gc)->gdipFont = gdipFont;
			}
		}
		if ((state & GRAPHICS_STATE_DRAW_OFFSET) != 0) {
			_W_GRAPHICS(gc)->gdipXOffset = _W_GRAPHICS(gc)->gdipYOffset = 0;
			GpMatrix *matrix = 0;
			GpPointF point;
			point.X = point.Y = 1;
			GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
			if (matrix != 0) {
				GdipGetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, matrix);
				GdipTransformMatrixPoints(matrix, &point, 1);
				GdipDeleteMatrix(matrix);
			}
			float scaling = point.X;
			if (scaling < 0)
				scaling = -scaling;
			float penWidth = _W_GRAPHICS(gc)->line.width * scaling;
			if (penWidth == 0 || ((int) penWidth % 2) == 1) {
				_W_GRAPHICS(gc)->gdipXOffset = 0.5 / scaling;
			}
			scaling = point.Y;
			if (scaling < 0)
				scaling = -scaling;
			penWidth = _W_GRAPHICS(gc)->line.width * scaling;
			if (penWidth == 0 || ((int) penWidth % 2) == 1) {
				_W_GRAPHICS(gc)->gdipYOffset = 0.5 / scaling;
			}
		}
	} else {
		if ((state
				& (GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_LINE_CAP
						| GRAPHICS_STATE_LINE_JOIN | GRAPHICS_STATE_LINE_STYLE
						| GRAPHICS_STATE_LINE_WIDTH)) != 0) {
			int color = _W_GRAPHICS(gc)->foreground;
			int width = (int) _W_GRAPHICS(gc)->line.width;
			DWORD *dashes = 0;
			int dashes_length = 0;
			int lineStyle = PS_SOLID;
			switch (_W_GRAPHICS(gc)->line.style) {
			case W_LINE_SOLID:
				break;
			case W_LINE_DASH:
				lineStyle = PS_DASH;
				break;
			case W_LINE_DOT:
				lineStyle = PS_DOT;
				break;
			case W_LINE_DASHDOT:
				lineStyle = PS_DASHDOT;
				break;
			case W_LINE_DASHDOTDOT:
				lineStyle = PS_DASHDOTDOT;
				break;
			case W_LINE_CUSTOM: {
				if (_W_GRAPHICS(gc)->line.dashes != 0) {
					lineStyle = PS_USERSTYLE;
					dashes_length = _W_GRAPHICS(gc)->line.length;
					dashes = malloc(dashes_length * sizeof(DWORD));
					for (int i = 0; i < dashes_length; i++) {
						dashes[i] = (int) _W_GRAPHICS(gc)->line.dashes[i];
					}
				}
				break;
			}
			}
			if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
				SetBkMode(_W_GRAPHICS(gc)->handle,
				_W_GRAPHICS(gc)->line.style == W_LINE_SOLID ?
				OPAQUE :
																TRANSPARENT);
			}
			int joinStyle = 0;
			switch (_W_GRAPHICS(gc)->line.join) {
			case W_JOIN_MITER:
				joinStyle = PS_JOIN_MITER;
				break;
			case W_JOIN_ROUND:
				joinStyle = PS_JOIN_ROUND;
				break;
			case W_JOIN_BEVEL:
				joinStyle = PS_JOIN_BEVEL;
				break;
			}
			int capStyle = 0;
			switch (_W_GRAPHICS(gc)->line.cap) {
			case W_CAP_ROUND:
				capStyle = PS_ENDCAP_ROUND;
				break;
			case W_CAP_FLAT:
				capStyle = PS_ENDCAP_FLAT;
				break;
			case W_CAP_SQUARE:
				capStyle = PS_ENDCAP_SQUARE;
				break;
			}
			int style = lineStyle | joinStyle | capStyle;
			/*
			 * Feature in Windows.  Windows does not honour line styles other then
			 * PS_SOLID for pens wider than 1 pixel created with CreatePen().  The fix
			 * is to use ExtCreatePen() instead.
			 */
			HPEN newPen;
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = color & 0x00FFFFFF;
			logBrush.lbHatch = 0;
			/* Feature in Windows. PS_GEOMETRIC pens cannot have zero width. */
			int m = width;
			if (m < 1)
				m = 1;
			newPen = ExtCreatePen(style | PS_GEOMETRIC, m, &logBrush,
					dashes != 0 ? dashes_length : 0, dashes);
			SelectObject(_W_GRAPHICS(gc)->handle, newPen);
			_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_PEN;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_NULL_PEN;
			if (_W_GRAPHICS(gc)->hPen != 0)
				DeleteObject(_W_GRAPHICS(gc)->hPen);
			_W_GRAPHICS(gc)->hPen = _W_GRAPHICS(gc)->hOldPen = newPen;
			if (dashes != 0)
				free(dashes);
		} else if ((state & GRAPHICS_STATE_PEN) != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, _W_GRAPHICS(gc)->hOldPen);
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_NULL_PEN;
		} else if ((state & GRAPHICS_STATE_NULL_PEN) != 0) {
			_W_GRAPHICS(gc)->hOldPen = (HPEN) SelectObject(
			_W_GRAPHICS(gc)->handle, GetStockObject(NULL_PEN));
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_PEN;
		}
		if ((state & GRAPHICS_STATE_BACKGROUND) != 0) {
			HBRUSH newBrush = CreateSolidBrush(
			_W_GRAPHICS(gc)->background & 0x00FFFFFF);
			SelectObject(_W_GRAPHICS(gc)->handle, newBrush);
			_W_GRAPHICS(gc)->state |= GRAPHICS_STATE_BRUSH;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_NULL_BRUSH;
			if (_W_GRAPHICS(gc)->hBrush != 0)
				DeleteObject(_W_GRAPHICS(gc)->hBrush);
			_W_GRAPHICS(gc)->hOldBrush = _W_GRAPHICS(gc)->hBrush = newBrush;
		} else if ((state & GRAPHICS_STATE_BRUSH) != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, _W_GRAPHICS(gc)->hOldBrush);
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_NULL_BRUSH;
		} else if ((state & GRAPHICS_STATE_NULL_BRUSH) != 0) {
			_W_GRAPHICS(gc)->hOldBrush = (HBRUSH) SelectObject(
			_W_GRAPHICS(gc)->handle, GetStockObject(NULL_BRUSH));
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_BRUSH;
		}
		if ((state & GRAPHICS_STATE_BACKGROUND_TEXT) != 0) {
			SetBkColor(_W_GRAPHICS(gc)->handle,
			_W_GRAPHICS(gc)->background & 0x00FFFFFF);
		}
		if ((state & GRAPHICS_STATE_FOREGROUND_TEXT) != 0) {
			SetTextColor(_W_GRAPHICS(gc)->handle,
			_W_GRAPHICS(gc)->foreground & 0x00FFFFFF);
		}
		if ((state & GRAPHICS_STATE_FONT) != 0) {
			w_font *font = _W_GRAPHICS(gc)->font;
			if (font != 0) {
				SelectObject(_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle);
			}
		}
	}
	return W_TRUE;
}
GpFont* w_graphics_create_gdip_font(HDC hDC, HFONT hFont, GpGraphics *graphics,
		GpFontCollection *fontCollection, GpFontFamily **outFamily,
		HFONT *outFont) {
	GpStatus status;
	LOGFONTW logFont;
	GetObjectW(hFont, sizeof(logFont), &logFont);
	GpFont *font = 0;
	GdipCreateFontFromLogfontW(hDC, &logFont, &font);
	if (font == 0)
		return 0;
	GpFontFamily *family = 0;
	if (font != 0) {
		GdipDeleteFont(font);
		LOGFONTW logFont;
		GetObjectW(hFont, sizeof(logFont), &logFont);
		int size = abs(logFont.lfHeight);
		int style = FontStyleRegular;
		if (logFont.lfWeight == 700)
			style |= FontStyleBold;
		if (logFont.lfItalic != 0)
			style |= FontStyleItalic;
		WCHAR *chars;
		chars = logFont.lfFaceName;
		WCHAR *name = chars;
		if (lstrcmpiW(name, L"Courier")) { //$NON-NLS-1$
			name = L"Courier New"; //$NON-NLS-1$
		}
		if (fontCollection != 0) {
			GdipCreateFontFamilyFromName(name, fontCollection, &family);
			if (family != 0) {
				GdipDeleteFontFamily(family);
				GdipCreateFontFamilyFromName(name, 0, &family);
				if (family != 0) {
					GdipDeleteFontFamily(family);
					family = 0;
				}
			}
		}
		if (family != 0) {
			GdipCreateFont(family, size, style, UnitPixel, &font);
		} else {
			GpFontFamily *_family = 0;
			GdipCreateFontFamilyFromName(name, NULL, &_family);
			GdipCreateFont(_family, size, style, UnitPixel, &font);
			GdipDeleteFontFamily(_family);
		}
		if (outFont != 0 && font != 0) {
			LOGFONTW pLogFont;
			GdipGetLogFontW(font, graphics, &pLogFont);
			outFont[0] = CreateFontIndirectW(&pLogFont);
		}
	}
	if (outFamily != 0 && font != 0) {
		if (family == 0) {
			GdipGetFamily(font, &family);
		}
		outFamily[0] = family;
	} else {
		if (family != 0)
			GdipDeleteFontFamily(family);
	}
//if (font == 0) SWT.error(SWT.ERROR_NO_HANDLES);
	return font;
}
wresult w_graphics_create(w_graphics *gc) {
	w_graphics_dispose(gc);
	HDC hdc = GetDC(NULL);
	if (hdc == NULL)
		return W_ERROR_NO_HANDLES;
	_w_graphics_init(gc, hdc);
	return W_TRUE;
}
wresult w_graphics_is_ok(w_graphics *gc) {
	if (gc == 0)
		return W_FALSE;
	return _W_GRAPHICS(gc)->handle != 0;
}
wresult w_graphics_copy_area_to_image(w_graphics *gc, w_surface *surface, int x,
		int y) {
	return W_FALSE;
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wresult paint) {
	return W_FALSE;
}
void _w_graphics_destroy_gdip_brush(GpBrush *brush) {
	GdipDeleteBrush(brush);
}
void _w_graphics_dispose_gdip(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->gdipPen != 0)
		GdipDeletePen(_W_GRAPHICS(gc)->gdipPen);
	if (_W_GRAPHICS(gc)->gdipBgBrush != 0)
		_w_graphics_destroy_gdip_brush(_W_GRAPHICS(gc)->gdipBgBrush);
	if (_W_GRAPHICS(gc)->gdipFgBrush != 0)
		_w_graphics_destroy_gdip_brush(_W_GRAPHICS(gc)->gdipFgBrush);
	if (_W_GRAPHICS(gc)->gdipFont != 0)
		GdipDeleteFont(_W_GRAPHICS(gc)->gdipFont);
	if (_W_GRAPHICS(gc)->hGDIFont != 0)
		DeleteObject(_W_GRAPHICS(gc)->hGDIFont);
	if (_W_GRAPHICS(gc)->gdipGraphics != 0)
		GdipDeleteGraphics(_W_GRAPHICS(gc)->gdipGraphics);
	_W_GRAPHICS(gc)->gdipGraphics = 0;
	_W_GRAPHICS(gc)->gdipBrush = 0;
	_W_GRAPHICS(gc)->gdipBgBrush = 0;
	_W_GRAPHICS(gc)->gdipFgBrush = 0;
	_W_GRAPHICS(gc)->gdipFont = 0;
	_W_GRAPHICS(gc)->gdipPen = 0;
	_W_GRAPHICS(gc)->hGDIFont = 0;
}
void w_graphics_dispose(w_graphics *gc) {
	if (gc != 0 && _W_GRAPHICS(gc)->handle != 0) {
		int gdip = _W_GRAPHICS(gc)->gdipGraphics != 0;
		_w_graphics_dispose_gdip(gc);
		if (gdip && (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			SetLayout(_W_GRAPHICS(gc)->handle,
					GetLayout(_W_GRAPHICS(gc)->handle) | LAYOUT_RTL);
		}
		if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_GDIPRELEASEDC) {
			GdipReleaseDC(_W_GRAPHICS(gc)->gdipGraphics,
			_W_GRAPHICS(gc)->handle);
			_W_GRAPHICS(gc)->gdipGraphics = 0;
		}
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GdipDeleteGraphics(_W_GRAPHICS(gc)->gdipGraphics);
		}
		if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_RELEASEDC) {
			ReleaseDC(_W_GRAPHICS(gc)->hwnd, _W_GRAPHICS(gc)->handle);
		}
		if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_DELETEDC) {
			DeleteDC(_W_GRAPHICS(gc)->handle);
		}
		_W_GRAPHICS(gc)->handle = 0;
	}
}
wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return result;
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			if (width == height) {
				GdipDrawArc(gdipGraphics, _W_GRAPHICS(gc)->gdipPen, x, y, width,
						height, -startAngle, -arcAngle);
			} else {
				GpPath *path = 0;
				GdipCreatePath(FillModeAlternate, &path);
				if (path != 0) {
					GpMatrix *matrix = 0;
					GdipCreateMatrix2(width, 0, 0, height, x, y, &matrix);
					if (matrix != 0) {
						GdipAddPathArc(path, 0, 0, 1, 1, -startAngle,
								-arcAngle);
						GdipTransformPath(path, matrix);
						GdipDrawPath(_W_GRAPHICS(gc)->gdipGraphics,
						_W_GRAPHICS(gc)->gdipPen, path);
						GdipDeleteMatrix(matrix);
					}
					GdipDeletePath(path);
				}
			}
			GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
					x--;
			}
			int x1, y1, x2, y2, tmp;
			int isNegative;
			if (arcAngle >= 360 || arcAngle <= -360) {
				x1 = x2 = x + width;
				y1 = y2 = y + height / 2;
			} else {
				isNegative = arcAngle < 0;

				arcAngle = arcAngle + startAngle;
				if (isNegative) {
					// swap angles
					tmp = startAngle;
					startAngle = arcAngle;
					arcAngle = tmp;
				}
				x1 = _cos(startAngle, width) + x + width / 2;
				y1 = -1 * _sin(startAngle, height) + y + height / 2;

				x2 = _cos(arcAngle, width) + x + width / 2;
				y2 = -1 * _sin(arcAngle, height) + y + height / 2;
			}
			Arc(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1,
					x1, y1, x2, y2);
		}
	}
	return result;
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image, w_rect *src,
		w_rect *dest, int state) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (!w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GpImage *img = 0;
		_w_image_get_gpimage(image, &img);
		if (img != 0) {
			UINT imgWidth;
			UINT imgHeight;
			GdipGetImageWidth(img, &imgWidth);
			GdipGetImageHeight(img, &imgHeight);
			GpRect rect;
			rect.X = destX;
			rect.Y = destY;
			rect.Width = destWidth;
			rect.Height = destHeight;
			/*
			 * Note that if the wrap mode is not WrapModeTileFlipXY, the scaled image
			 * is translucent around the borders.
			 */
			GpImageAttributes *attrib = 0;
			GdipCreateImageAttributes(&attrib);
			GdipSetImageAttributesWrapMode(attrib, WrapModeTileFlipXY, 0,
			FALSE);
			if (_W_GRAPHICS(gc)->alpha != 0xFF) {
				ColorMatrix matrix;
				memset(&matrix, 0, sizeof(matrix));
				matrix.m[0][0] = 1;
				matrix.m[1][1] = 1;
				matrix.m[2][2] = 1;
				matrix.m[3][3] = _W_GRAPHICS(gc)->alpha / (float) 0xFF;
				matrix.m[4][4] = 1;
				GdipSetImageAttributesColorMatrix(attrib, ColorAdjustTypeBitmap,
				FALSE, &matrix, 0, ColorMatrixFlagsDefault);
			}
			GraphicsState gstate = 0;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				GdipSaveGraphics(_W_GRAPHICS(gc)->gdipGraphics, &gstate);
				GdipScaleWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, -1, 1,
						MatrixOrderPrepend);
				GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
						-2 * destX - destWidth, 0, MatrixOrderPrepend);
			}
			GdipDrawImageRectRect(_W_GRAPHICS(gc)->gdipGraphics, img, rect.X,
					rect.Y, rect.Width, rect.Height, srcX, srcY, srcWidth,
					srcHeight, UnitPixel, attrib, 0, 0);
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				GdipRestoreGraphics(_W_GRAPHICS(gc)->gdipGraphics, gstate);
			}
			GdipDisposeImageAttributes(attrib);
			_w_image_dispose_gpimage(image, img);
		}
	} else {
		_w_image_hbitmap img;
		_w_image_get_hbitmap(image, &img);
		if (img.hbmColor != 0) {
			BITMAP bm;
			GetObjectW(img.hbmColor, sizeof(bm), &bm);
			int imgWidth = bm.bmWidth;
			int imgHeight = bm.bmHeight;
			if (srcWidth < 0 || srcWidth >= imgWidth)
				srcWidth = imgWidth;
			if (srcHeight < 0 || srcHeight >= imgHeight)
				srcHeight = imgHeight;
			if (destWidth < 0)
				destWidth = imgWidth;
			if (destHeight < 0)
				destHeight = imgHeight;
			//int mustRestore = W_FALSE;
			HDC srcHdc = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
			HBITMAP oldSrcBitmap = SelectObject(srcHdc, img.hbmColor);
			if (bm.bmBitsPixel == 32) {
				BLENDFUNCTION blend;
				blend.BlendOp = AC_SRC_OVER;
				blend.BlendFlags = 0;
				blend.SourceConstantAlpha = 0xFF;
				blend.AlphaFormat = AC_SRC_ALPHA;
				AlphaBlend(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
						destHeight, srcHdc, srcX, srcY, srcWidth, srcHeight,
						blend);
			} else {
				int rop2 = 0;
				rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
				int dwRop = rop2 == R2_XORPEN ? SRCINVERT : SRCCOPY;
				if (srcWidth != destWidth || srcHeight != destHeight) {
					int mode = 0;
					mode = SetStretchBltMode(_W_GRAPHICS(gc)->handle,
					COLORONCOLOR);
					StretchBlt(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
							destHeight, srcHdc, srcX, srcY, srcWidth, srcHeight,
							dwRop);
					SetStretchBltMode(_W_GRAPHICS(gc)->handle, mode);
				} else {
					BitBlt(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
							destHeight, srcHdc, srcX, srcY, dwRop);
				}
			}
			SelectObject(srcHdc, oldSrcBitmap);
			DeleteDC(srcHdc);
		}
		_w_image_dispose_hbitmap(image, &img);
	}
	return result;
}
wresult w_graphics_draw_surface(w_graphics *gc, w_surface *surface, w_rect *src,
		w_rect *dest) {
	return w_graphics_draw_image(gc, (w_image*) surface, src, dest, 0);
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawLine(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, pt1->x, pt1->y, pt2->x, pt2->y);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			MoveToEx(_W_GRAPHICS(gc)->handle, pt1->x, pt1->y, 0);
			LineTo(_W_GRAPHICS(gc)->handle, pt2->x, pt2->y);
			if (_W_GRAPHICS(gc)->line.width <= 1) {
				SetPixel(_W_GRAPHICS(gc)->handle, pt1->x, pt1->y,
				_W_GRAPHICS(gc)->foreground);
			}
		}
	}
	return result;
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawEllipse(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, rect->x, rect->y, rect->width,
					rect->height);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			int x = rect->x;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
					x--;
			}
			Ellipse(_W_GRAPHICS(gc)->handle, x, rect->y, x + rect->width + 1,
					rect->y + rect->height + 1);
		}
	}
	return result;
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_w_graphics_init_gdip(gc);
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		GpGraphics *gdipGraphics = _gc->gdipGraphics;
		GdipTranslateWorldTransform(gdipGraphics, _gc->gdipXOffset,
				_gc->gdipYOffset, MatrixOrderPrepend);
		GdipDrawPath(gdipGraphics, _gc->gdipPen, _W_PATH(path)->handle);
		GdipTranslateWorldTransform(gdipGraphics, -_gc->gdipXOffset,
				-_gc->gdipYOffset, MatrixOrderPrepend);
	}
	return result;
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			w_pattern *foregroundPattern = _W_GRAPHICS(gc)->foregroundPattern;
			GpBrush *FgBrush =
					foregroundPattern != 0 ?
					_W_PATTERN(foregroundPattern)->handle :
												_W_GRAPHICS(gc)->gdipFgBrush;
			GdipFillRectangle(_W_GRAPHICS(gc)->gdipGraphics, FgBrush, pt->x,
					pt->y, 1, 1);
		} else {
			SetPixel(_W_GRAPHICS(gc)->handle, pt->x, pt->y,
			_W_GRAPHICS(gc)->foreground & 0x00FFFFFF);
		}
	}
	return result;
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawPolygonI(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, (GpPoint*) pointArray, count);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			POINT *points = (POINT*) pointArray;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width % 2) == 0) {
					points = (POINT*) _w_toolkit_malloc(count * sizeof(POINT));
					if (points != 0) {
						for (int i = 0; i < count; i++) {
							points[i].x = pointArray[i].x - 1;
							points[i].y = pointArray[i].y - 1;
						}
					} else
						points = (POINT*) pointArray;
				}
			}
			Polygon(_W_GRAPHICS(gc)->handle, points, count);
			if (points != (POINT*) pointArray) {
				_w_toolkit_free(points, count * sizeof(POINT));
			}
		}
	}
	return result;
}
wresult w_graphics_draw_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		POINT *points = (POINT*) _w_toolkit_malloc(count * sizeof(POINT));
		if (points == 0)
			return W_ERROR_NO_MEMORY;
		for (int i = 0; i < count; i++) {
			points[i].x = va_arg(args, int);
			points[i].y = va_arg(args, int);
		}
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawPolygonI(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, (GpPoint*) points, count);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width % 2) == 0) {
					for (int i = 0; i < count; i++) {
						points[i].x--;
						points[i].y--;
					}
				}
			}
			Polygon(_W_GRAPHICS(gc)->handle, points, count);
		}
		_w_toolkit_free(points, count * sizeof(POINT));
	}
	return result;
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawLinesI(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, (GpPoint*) pointArray, count);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			POINT *points = (POINT*) pointArray;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width % 2) == 0) {
					points = (POINT*) _w_toolkit_malloc(count * sizeof(POINT));
					if (points != 0) {
						for (int i = 0; i < count; i++) {
							points[i].x = pointArray[i].x - 1;
							points[i].y = pointArray[i].y - 1;
						}
					} else
						points = (POINT*) pointArray;
				}
			}
			Polyline(_W_GRAPHICS(gc)->handle, points, count);
			if (count >= 1) {
				if (_W_GRAPHICS(gc)->line.width <= 1) {
					SetPixel(_W_GRAPHICS(gc)->handle, points[count - 1].x,
							points[count - 1].y,
							_W_GRAPHICS(gc)->foreground & 0x00FFFFFF);
				}
			}
			if (points != (POINT*) pointArray) {
				_w_toolkit_free(points, count * sizeof(POINT));
			}
		}
	}
	return result;
}
wresult w_graphics_draw_polylinev(w_graphics *gc, int count, va_list args) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		POINT *points = (POINT*) _w_toolkit_malloc(count * sizeof(POINT));
		if (points == 0)
			return W_ERROR_NO_MEMORY;
		for (int i = 0; i < count; i++) {
			points[i].x = va_arg(args, int);
			points[i].y = va_arg(args, int);
		}
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawLinesI(gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, (GpPoint*) points, count);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width % 2) == 0) {
					for (int i = 0; i < count; i++) {
						points[i].x--;
						points[i].y--;
					}
				}
			}
			Polyline(_W_GRAPHICS(gc)->handle, points, count);
			if (count >= 1) {
				if (_W_GRAPHICS(gc)->line.width <= 1) {
					SetPixel(_W_GRAPHICS(gc)->handle, points[count - 1].x,
							points[count - 1].y,
							_W_GRAPHICS(gc)->foreground & 0x00FFFFFF);
				}
			}
		}
		_w_toolkit_free(points, count * sizeof(POINT));
	}
	return result;
}
wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			if (width < 0) {
				x = x + width;
				width = -width;
			}
			if (height < 0) {
				y = y + height;
				height = -height;
			}
			GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
			GdipTranslateWorldTransform(gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			GdipDrawRectangle(gdipGraphics, _W_GRAPHICS(gc)->gdipPen, x, y,
					width, height);
			GdipTranslateWorldTransform(gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
			return W_TRUE;
		}
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			/*
			 * Note that Rectangle() subtracts one pixel in MIRRORED mode when
			 * the pen was created with CreatePen() and its width is 0 or 1.
			 */
			if (_W_GRAPHICS(gc)->line.width > 1) {
				if ((((int) _W_GRAPHICS(gc)->line.width) % 2) == 1)
					x++;
			} else {
				if (_W_GRAPHICS(gc)->hPen != 0
						&& GetObjectW(_W_GRAPHICS(gc)->hPen, 0, 0)
								!= sizeof(LOGPEN)) {
					x++;
				}
			}
		}
		Rectangle(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1);
	}
	return result;
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result > 0) {
		int x = rect->x, y = rect->y, width = rect->width,
				height = rect->height;
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			int nx = x;
			int ny = y;
			int nw = width;
			int nh = height;
			int naw = arcWidth;
			int nah = arcHeight;

			if (nw < 0) {
				nw = 0 - nw;
				nx = nx - nw;
			}
			if (nh < 0) {
				nh = 0 - nh;
				ny = ny - nh;
			}
			if (naw < 0)
				naw = 0 - naw;
			if (nah < 0)
				nah = 0 - nah;

			GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
			_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
					MatrixOrderPrepend);
			if (naw == 0 || nah == 0) {
				GdipDrawRectangleI(_W_GRAPHICS(gc)->gdipGraphics,
				_W_GRAPHICS(gc)->gdipPen, x, y, width, height);
			} else {
				GpPath *path = 0;
				GdipCreatePath(FillModeAlternate, &path);
				if (path != 0) {
					if (nw > naw) {
						if (nh > nah) {
							GdipAddPathArc(path, nx + nw - naw, ny, naw, nah, 0,
									-90);
							GdipAddPathArc(path, nx, ny, naw, nah, -90, -90);
							GdipAddPathArc(path, nx, ny + nh - nah, naw, nah,
									-180, -90);
							GdipAddPathArc(path, nx + nw - naw, ny + nh - nah,
									naw, nah, -270, -90);
						} else {
							GdipAddPathArc(path, nx + nw - naw, ny, naw, nh,
									-270, -180);
							GdipAddPathArc(path, nx, ny, naw, nh, -90, -180);
						}
					} else {
						if (nh > nah) {
							GdipAddPathArc(path, nx, ny, nw, nah, 0, -180);
							GdipAddPathArc(path, nx, ny + nh - nah, nw, nah,
									-180, -180);
						} else {
							GdipAddPathArc(path, nx, ny, nw, nh, 0, 360);
						}
					}
					GdipClosePathFigure(path);
					GdipDrawPath(_W_GRAPHICS(gc)->gdipGraphics,
					_W_GRAPHICS(gc)->gdipPen, path);
					GdipDeletePath(path);
				} else
					result = W_ERROR_NO_HANDLES;
			}
			GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
					-_W_GRAPHICS(gc)->gdipXOffset,
					-_W_GRAPHICS(gc)->gdipYOffset, MatrixOrderPrepend);
		} else {
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				if (_W_GRAPHICS(gc)->line.width != 0
						&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
					x--;
			}
			RoundRect(_W_GRAPHICS(gc)->handle, x, y, x + width + 1,
					y + height + 1, arcWidth, arcHeight);
		}
	}
	return result;
}
wresult _w_graphics_use_gdip(HDC hdc, WCHAR *text, size_t length) {
	WORD *glyphs = _w_toolkit_malloc(length * sizeof(WORD));
	if (glyphs != 0) {
		GetGlyphIndicesW(hdc, text, length, glyphs,
		GGI_MARK_NONEXISTING_GLYPHS);
		for (int i = 0; i < length; i++) {
			if (glyphs[i] == -1) {
				switch (text[i]) {
				case '\t':
				case '\n':
				case '\r':
					break;
				default:
					_w_toolkit_free(glyphs, length * sizeof(WORD));
					return W_TRUE;
					break;
				}
			}
		}
		_w_toolkit_free(glyphs, length * sizeof(WORD));
	}
	return W_FALSE;
}
float _w_graphics_measure_space(GpGraphics *gdipGraphics, GpFont *font,
		GpStringFormat *format) {
	GpRectF bounds, pt;
	WCHAR ch[2];
	ch[0] = ' ';
	ch[1] = 0;
	INT i1 = 0, i2 = 0;
	GdipMeasureString(gdipGraphics, ch, 1, font, &pt, format, &bounds, &i1,
			&i2);
	return bounds.Width;
}

wresult _w_graphics_draw_text_gdip(w_graphics *gc, GpGraphics *gdipGraphics,
		WCHAR *text, size_t length, w_rect *rect, int flags, wresult draw,
		w_size *size) {
	wresult needsBounds = !draw || (flags & W_DRAW_TRANSPARENT) == 0;
	wresult reset_buffer = W_FALSE;
	WCHAR text_tmp[3];
	if (length == 0) {
		if (draw)
			return W_TRUE;
		text_tmp[0] = ' ';
		text_tmp[1] = 0;
		text = text_tmp;
		length = 1;
		reset_buffer = W_TRUE;
	}
	GpRectF pt, bounds;
	GpStringFormat *genericTypographic = 0, *format = 0;
	INT formatFlags;
	REAL tabs[1];
	GdipStringFormatGetGenericTypographic(&genericTypographic);
	GdipCloneStringFormat(genericTypographic, &format);
	GdipGetStringFormatFlags(format, &formatFlags);
	formatFlags |= StringFormatFlagsMeasureTrailingSpaces;
	if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0)
		formatFlags |= StringFormatFlagsDirectionRightToLeft;
	GdipSetStringFormatFlags(format, formatFlags);
	if ((flags & W_DRAW_TAB) != 0) {
		tabs[0] = _w_graphics_measure_space(gdipGraphics,
		_W_GRAPHICS(gc)->gdipFont, format) * 8;
	} else {
		tabs[0] = 0;
	}
	GdipSetStringFormatTabStops(format, 0, 1, tabs);
	int hotkeyPrefix =
			(flags & W_DRAW_MNEMONIC) != 0 ?
					HotkeyPrefixShow : HotkeyPrefixNone;
//if ((flags & W_DRAW_MNEMONIC) != 0 && (_W_GRAPHICS(gc)->uiState & UISF_HIDEACCEL) != 0) hotkeyPrefix = HotkeyPrefixHide;
	GdipSetStringFormatHotkeyPrefix(format, hotkeyPrefix);
	if (needsBounds) {
		GdipMeasureString(gdipGraphics, text, length,
		_W_GRAPHICS(gc)->gdipFont, &pt, format, &bounds, NULL, NULL);
	}
	if (draw) {
		if ((flags & W_DRAW_TRANSPARENT) == 0) {
			GdipFillRectangle(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, rect->x,
					rect->y, (int) ceil(bounds.Width),
					(int) ceil(bounds.Height));
		}
		GraphicsState gstate = 0;
		GpBrushType brushType;
		GpBrush *brush = _w_graphics_get_fg_brush(gc);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			GdipGetBrushType(brush, &brushType);
			switch (brushType) {
			case BrushTypeLinearGradient:
				GdipScaleLineTransform(brush, -1, 1, MatrixOrderPrepend);
				GdipTranslateLineTransform(brush, -2 * rect->x, 0,
						MatrixOrderPrepend);
				break;
			case BrushTypeTextureFill:
				GdipScaleTextureTransform(brush, -1, 1, MatrixOrderPrepend);
				GdipTranslateTextureTransform(brush, -2 * rect->x, 0,
						MatrixOrderPrepend);
				break;
			}
			GdipSaveGraphics(gdipGraphics, &gstate);
			GdipScaleWorldTransform(gdipGraphics, -1, 1, MatrixOrderPrepend);
			GdipTranslateWorldTransform(gdipGraphics, -2 * rect->x, 0,
					MatrixOrderPrepend);
		}
		pt.X = rect->x;
		pt.Y = rect->y;
		GdipDrawString(gdipGraphics, text, length,
		_W_GRAPHICS(gc)->gdipFont, &pt, format, brush);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			GdipGetBrushType(brush, &brushType);
			switch (brushType) {
			case BrushTypeLinearGradient:
				GdipResetLineTransform(brush);
				break;
			case BrushTypeTextureFill:
				GdipResetTextureTransform(brush);
				break;
			}
			GdipRestoreGraphics(gdipGraphics, gstate);
		}
	}
	GdipDeleteStringFormat(format);
	if (reset_buffer) {
		bounds.Width = 0;
		text[0] = 0;
		length = 0;
	}
	if (size != 0) {
		size->width = (int) ceil(bounds.Width);
		size->height = (int) ceil(bounds.Height);
	}
	return W_TRUE;
}
GpRectF* w_graphics_draw_text_1(w_graphics *gc, GpGraphics *gdipGraphics,
		WCHAR *text, size_t _length, int start, int length, w_rect *rect,
		int flags, int mnemonicIndex, TEXTMETRICW *lptm, boolean draw,
		GpRectF *r) {
	wresult drawMnemonic = draw && mnemonicIndex != -1
			&& (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_UISF_HIDEACCEL) == 0;
	wresult needsBounds = !draw || drawMnemonic
			|| (flags & W_DRAW_TRANSPARENT) == 0
			|| (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0
			|| (flags & W_DRAW_DELIMITER) != 0;
	GpRectF *bounds;
	GraphicsState gstate;
	if (length <= 0) {
		bounds = 0;
		if (needsBounds) {
			bounds = r;
			memset(r, 0, sizeof(GpRectF));
			bounds->Height = lptm->tmHeight;
		}
		return bounds;
	}
	int nGlyphs = (length * 3 / 2) + 16;
	GCP_RESULTSW result;
	memset(&result, 0, sizeof(result));
	result.lStructSize = sizeof(GCP_RESULTS);
	result.nGlyphs = nGlyphs;
	size_t mem_size = nGlyphs * 18;
	char *mem = (char*) _w_toolkit_malloc(mem_size), *mem_i;
	if (mem != 0) {
		memset(mem, 0, mem_size);
		mem_i = mem;
		result.lpDx = (int*) mem_i;
		mem_i += nGlyphs * sizeof(int);
		result.lpGlyphs = (LPWSTR) mem_i;
		mem_i += nGlyphs * sizeof(WCHAR);
		int dwFlags = GCP_GLYPHSHAPE | GCP_REORDER | GCP_LIGATE;
		if (drawMnemonic) {
			result.lpOrder = (UINT*) mem_i;
			mem_i += nGlyphs * sizeof(int);
		}
		HDC hdc;
		GdipGetDC(gdipGraphics, &hdc);
		HFONT hFont = _W_GRAPHICS(gc)->hGDIFont;
		if (hFont == 0 && _W_GRAPHICS(gc)->font != 0)
			hFont = _W_FONT(_W_GRAPHICS(gc)->font)->handle;
		HFONT oldFont = 0;
		if (hFont != 0)
			oldFont = SelectObject(hdc, hFont);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			SetLayout(hdc, GetLayout(hdc) | LAYOUT_RTL);
		}
		GetCharacterPlacementW(hdc, &text[start], length - start, 0, &result,
				dwFlags);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			SetLayout(hdc, GetLayout(hdc) & ~LAYOUT_RTL);
		}
		if (hFont != 0)
			SelectObject(hdc, oldFont);
		GdipReleaseDC(gdipGraphics, hdc);
		nGlyphs = result.nGlyphs;
		w_rect drawR;
		drawR.x = rect->x;
		drawR.y = rect->y + lptm->tmAscent;
		drawR.width = rect->width;
		drawR.height = rect->height;
		int *dx = result.lpDx;
		GpPointF *points = (GpPointF*) mem_i;
		mem_i += nGlyphs * sizeof(GpPointF);
		for (int i = 0, j = 0; i < nGlyphs; i++) {
			points[j].X = drawR.x;
			points[j].Y = drawR.y;
			j++;
			drawR.x += dx[i];
		}
		bounds = 0;
		if (needsBounds) {
			bounds = r;
			GdipMeasureDriverString(gdipGraphics, (UINT16*) result.lpGlyphs,
					nGlyphs,
					_W_GRAPHICS(gc)->gdipFont, points, 0, 0, bounds);
		}
		if (draw) {
			if ((flags & W_DRAW_TRANSPARENT) == 0) {
				GdipFillRectangle(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush,
						rect->x, rect->y, (int) ceil(bounds->Width),
						(int) ceil(bounds->Height));
			}
			GraphicsState gstate = 0;
			GpBrush *brush = _w_graphics_get_fg_brush(gc);
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				GpBrushType type;
				GdipGetBrushType(brush, &type);
				switch (type) {
				case BrushTypeLinearGradient:
					GdipScaleLineTransform(brush, -1, 1, MatrixOrderPrepend);
					GdipTranslateLineTransform(brush,
							-2 * rect->x - bounds->Width, 0,
							MatrixOrderPrepend);
					break;
				case BrushTypeTextureFill:
					GdipScaleTextureTransform(brush, -1, 1, MatrixOrderPrepend);
					GdipTranslateLineTransform(brush,
							-2 * rect->x - bounds->Width, 0,
							MatrixOrderPrepend);
					break;
				}
				GdipSaveGraphics(gdipGraphics, &gstate);
				GdipScaleWorldTransform(gdipGraphics, -1, 1,
						MatrixOrderPrepend);
				GdipTranslateWorldTransform(gdipGraphics,
						-2 * rect->x - bounds->Width, 0, MatrixOrderPrepend);
			}
			GdipDrawDriverString(gdipGraphics, (UINT16*) result.lpGlyphs,
					result.nGlyphs,
					_W_GRAPHICS(gc)->gdipFont, brush, points, 0, 0);
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
				GpBrushType type;
				GdipGetBrushType(brush, &type);
				switch (type) {
				case BrushTypeLinearGradient:
					GdipResetLineTransform(brush);
					break;
				case BrushTypeTextureFill:
					GdipResetTextureTransform(brush);
					break;
				}
				GdipRestoreGraphics(gdipGraphics, gstate);
			}
			if (drawMnemonic) {
				GpPen *pen = 0;
				GdipCreatePen2(brush, 1, UnitWorld, &pen);
				if (pen != 0) {
					int order = result.lpOrder[mnemonicIndex];
					int mnemonicLeft, mnemonicRight;
					if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED)
							!= 0) {
						mnemonicLeft = (int) ceil(bounds->Width)
								- (int) points[order].X + 2 * rect->x;
						mnemonicRight = mnemonicLeft - dx[order];
					} else {
						mnemonicLeft = (int) points[order].X;
						mnemonicRight = mnemonicLeft + dx[order];
					}
					int mnemonicY = rect->y + lptm->tmAscent + 2;
					SmoothingMode smoothingMode;
					GdipGetSmoothingMode(gdipGraphics, &smoothingMode);
					GdipSetSmoothingMode(gdipGraphics, SmoothingModeNone);
					GdipDrawLine(gdipGraphics, pen, mnemonicLeft, mnemonicY,
							mnemonicRight, mnemonicY);
					GdipSetSmoothingMode(gdipGraphics, smoothingMode);
					GdipDeletePen(pen);
				}
			}
		}
		_w_toolkit_free(mem, mem_size);
	}
	return bounds;
}
wresult _w_graphics_draw_text_0(w_graphics *gc, GpGraphics *gdipGraphics,
		WCHAR *text, size_t length, w_rect *rect, int flags, w_size *size) {
	GpRectF r;
	HDC hdc = 0;
	GdipGetDC(gdipGraphics, &hdc);
	HFONT hFont = _W_GRAPHICS(gc)->hGDIFont;
	if (hFont == 0 && _W_GRAPHICS(gc)->font != 0)
		hFont = _W_FONT(_W_GRAPHICS(gc)->font)->handle;
	HFONT oldFont = 0;
	if (hFont != 0)
		oldFont = SelectObject(hdc, hFont);
	TEXTMETRICW lptm;
	GetTextMetricsW(hdc, &lptm);
	wresult gdip = _w_graphics_use_gdip(hdc, text, length);
	if (hFont != 0)
		SelectObject(hdc, oldFont);
	GdipReleaseDC(gdipGraphics, hdc);
	if (gdip) {
		return _w_graphics_draw_text_gdip(gc, gdipGraphics, text, length, rect,
				flags, size == 0, size);
	}
	w_rect drawR;
	drawR.x = rect->x;
	drawR.y = rect->y;
	drawR.width = rect->width;
	drawR.height = rect->height;
	int i = 0, start = 0, end = 0, width = 0, mnemonicIndex = -1;
	if ((flags & (W_DRAW_DELIMITER | W_DRAW_TAB | W_DRAW_MNEMONIC)) != 0) {
		int tabWidth = lptm.tmAveCharWidth * 8;
		while (i < length) {
			char c = text[end++] = text[i++];
			switch (c) {
			case '\t':
				if ((flags & W_DRAW_TAB) != 0) {
					int l = end - start - 1;
					GpRectF *bounds = w_graphics_draw_text_1(gc, gdipGraphics,
							text, length, start, l, &drawR, flags,
							mnemonicIndex, &lptm, size == 0, &r);
					drawR.x += ceil(bounds->Width);
					drawR.x = rect->x
							+ (((drawR.x - rect->x) / tabWidth) + 1) * tabWidth;
					mnemonicIndex = -1;
					start = end;
				}
				break;
			case '&':
				if ((flags & W_DRAW_MNEMONIC) != 0) {
					if (i == length) {
						end--;
						continue;
					}
					if (text[i] == '&') {
						i++;
						continue;
					}
					end--;
					mnemonicIndex = end - start;
				}
				break;
			case '\r':
			case '\n':
				if ((flags & W_DRAW_DELIMITER) != 0) {
					int l = end - start - 1;
					if (c == '\r' && end != length && text[end] == '\n') {
						end++;
						i++;
					}
					GpRectF *bounds = w_graphics_draw_text_1(gc, gdipGraphics,
							text, length, start, l, &drawR, flags,
							mnemonicIndex, &lptm, size == 0, &r);
					drawR.y += ceil(bounds->Height);
					width = WMAX(width, drawR.x + (int ) ceil(bounds->Width));
					drawR.x = rect->x;
					mnemonicIndex = -1;
					start = end;
				}
				break;
			}
		}
		length = end;
	}
	GpRectF *bounds = w_graphics_draw_text_1(gc, gdipGraphics, text, length,
			start, length - start, &drawR, flags, mnemonicIndex, &lptm,
			size == 0, &r);
	if (size != 0) {
		drawR.y += ceil(bounds->Height);
		width = WMAX(width, drawR.x + (int ) ceil(bounds->Width));
		size->width = width;
		size->height = drawR.y;
	}
	return W_TRUE;
}
wresult w_graphics_draw_text(w_graphics *gc, const char *text, size_t length,
		w_rect *rect, int flags, int enc) {
	if (text == 0)
		return W_FALSE;
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		size_t newlength;
		WCHAR *s = _win_text_fix(text, length, &newlength, enc);
		if (s != 0) {
			if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
				result = w_graphics_check(gc,
						GRAPHICS_STATE_FONT | GRAPHICS_STATE_FOREGROUND
								| ((flags & W_DRAW_TRANSPARENT) != 0 ?
										0 : GRAPHICS_STATE_BACKGROUND));
				_w_graphics_draw_text_0(gc, _W_GRAPHICS(gc)->gdipGraphics, s,
						newlength, rect, flags, 0);
			} else {
				RECT r;
				/*
				 * Feature in Windows.  For some reason DrawText(), the maximum
				 * value for the bottom and right coordinates for the RECT that
				 * is used to position the text is different on between Windows
				 * versions.  If this value is larger than the maximum, nothing
				 * is drawn.  On Windows 98, the limit is 0x7FFF.  On Windows CE,
				 * NT, and 2000 it is 0x6FFFFFF. And on XP, it is 0x7FFFFFFF.
				 * The fix is to use the the smaller limit for Windows 98 and the
				 * larger limit on the other Windows platforms.
				 */
				int limit, right, bottom;
#if IsWin95
				limit =0x7FFF;
#else
				limit = 0x6FFFFFF;
#endif
				if (rect->width >= 0)
					right = rect->x + rect->width;
				else
					right = limit;
				if (rect->height >= 0)
					bottom = rect->y + rect->height;
				else
					bottom = limit;

				SetRect(&r, rect->x, rect->y, right, bottom);
				int uFormat = DT_LEFT;
				if ((flags & W_DRAW_DELIMITER) == 0)
					uFormat |= DT_SINGLELINE;
				if ((flags & W_DRAW_TAB) != 0)
					uFormat |= DT_EXPANDTABS;
				if ((flags & W_DRAW_MNEMONIC) == 0)
					uFormat |= DT_NOPREFIX;
				if ((flags & W_DRAW_MNEMONIC) != 0
						&& (_W_GRAPHICS(gc)->state
								& GRAPHICS_STATE_UISF_HIDEACCEL) != 0) {
					uFormat |= DT_HIDEPREFIX;
				}
				int rop2 = 0;
				rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
				int oldBkMode = SetBkMode(_W_GRAPHICS(gc)->handle,
						(flags & W_DRAW_TRANSPARENT) != 0 ?
						TRANSPARENT :
															OPAQUE);
				result = w_graphics_check(gc,
						GRAPHICS_STATE_FONT | GRAPHICS_STATE_FOREGROUND_TEXT
								| GRAPHICS_STATE_BACKGROUND_TEXT);
				if (rop2 != R2_XORPEN) {
					DrawTextW(_W_GRAPHICS(gc)->handle, s, newlength, &r,
							uFormat);
				} else {
					int foreground = GetTextColor(_W_GRAPHICS(gc)->handle);
					if ((flags & W_DRAW_TRANSPARENT) != 0) {
						DrawTextW(_W_GRAPHICS(gc)->handle, s, newlength, &r,
								uFormat | DT_CALCRECT);
						int width = r.right - r.left;
						int height = r.bottom - r.top;
						HBITMAP hBitmap = CreateCompatibleBitmap(
						_W_GRAPHICS(gc)->handle, width, height);
						if (hBitmap != 0) {
							HDC memDC = CreateCompatibleDC(
							_W_GRAPHICS(gc)->handle);
							HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
							PatBlt(memDC, 0, 0, width, height, BLACKNESS);
							SetBkMode(memDC, TRANSPARENT);
							SetTextColor(memDC, foreground);
							SelectObject(memDC,
									GetCurrentObject(_W_GRAPHICS(gc)->handle,
									OBJ_FONT));
							SetRect(&r, 0, 0, 0x7FFF, 0x7FFF);
							DrawTextW(memDC, s, newlength, &r, uFormat);
							BitBlt(_W_GRAPHICS(gc)->handle, rect->x, rect->y,
									width, height, memDC, 0, 0, SRCINVERT);
							SelectObject(memDC, hOldBitmap);
							DeleteDC(memDC);
							DeleteObject(hBitmap);
						} else
							result = W_ERROR_NO_MEMORY;
					} else {
						int background = GetBkColor(_W_GRAPHICS(gc)->handle);
						SetTextColor(_W_GRAPHICS(gc)->handle,
								foreground ^ background);
						DrawTextW(_W_GRAPHICS(gc)->handle, s, newlength, &r,
								uFormat);
						SetTextColor(_W_GRAPHICS(gc)->handle, foreground);
					}
				}
				SetBkMode(_W_GRAPHICS(gc)->handle, oldBkMode);
			}
			_win_text_free(text, s, newlength);
		}
	}
	return result;
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		int x = rect->x;
		int y = rect->y;
		int width = rect->width;
		int height = rect->height;
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		if (width == 0 || height == 0 || arcAngle == 0)
			return result;
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			if (width == height) {
				GdipFillPie(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, x, y,
						width, height, -startAngle, -arcAngle);
			} else {
				GraphicsState state;
				GdipSaveGraphics(gdipGraphics, &state);
				GdipTranslateWorldTransform(gdipGraphics, x, y,
						MatrixOrderPrepend);
				GdipScaleWorldTransform(gdipGraphics, width, height,
						MatrixOrderPrepend);
				GdipFillPie(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, 0, 0, 1,
						1, -startAngle, -arcAngle);
				GdipRestoreGraphics(gdipGraphics, state);
			}
		} else {
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0)
				x--;
			int x1, y1, x2, y2, tmp;
			int isNegative;
			if (arcAngle >= 360 || arcAngle <= -360) {
				x1 = x2 = x + width;
				y1 = y2 = y + height / 2;
			} else {
				isNegative = arcAngle < 0;

				arcAngle = arcAngle + startAngle;
				if (isNegative) {
					// swap angles
					tmp = startAngle;
					startAngle = arcAngle;
					arcAngle = tmp;
				}
				x1 = _cos(startAngle, width) + x + width / 2;
				y1 = -1 * _sin(startAngle, height) + y + height / 2;

				x2 = _cos(arcAngle, width) + x + width / 2;
				y2 = -1 * _sin(arcAngle, height) + y + height / 2;
			}
			Pie(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1,
					x1, y1, x2, y2);
		}
	}
	return result;
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		int vertical) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		if (rect->width == 0 || rect->height == 0)
			return result;
		int backgroundRGB, foregroundRGB;
		backgroundRGB = w_graphics_get_background(gc);
		foregroundRGB = w_graphics_get_foreground(gc);
		int fromRGB, toRGB;
		fromRGB = foregroundRGB;
		toRGB = backgroundRGB;
		wresult swapColors = W_FALSE;
		int x = rect->x;
		int y = rect->y;
		int width = rect->width;
		int height = rect->height;
		if (width < 0) {
			x += width;
			width = -width;
			if (!vertical)
				swapColors = W_TRUE;
		}
		if (height < 0) {
			y += height;
			height = -height;
			if (vertical)
				swapColors = W_TRUE;
		}
		if (swapColors) {
			fromRGB = backgroundRGB;
			toRGB = foregroundRGB;
		}
		if (fromRGB == toRGB) {
			return w_graphics_fill_rectangle(gc, rect);
		}
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			_w_graphics_init_gdip(gc);
			GpPointF p1, p2;
			p1.X = x;
			p1.Y = y;
			if (vertical) {
				p2.X = p1.X;
				p2.Y = p1.Y + height;
			} else {
				p2.X = p1.X + width;
				p2.Y = p1.Y;
			}
			ARGB fromGpColor = _W_GRAPHICS(gc)->alpha << 24
					| (fromRGB & 0x00FFFFFF);
			ARGB toGpColor = _W_GRAPHICS(gc)->alpha << 24
					| (toRGB & 0x00FFFFFF);
			GpBrush *brush;
			GdipCreateLineBrush(&p1, &p2, fromGpColor, toGpColor, WrapModeTile,
					&brush);
			GdipFillRectangle(gdipGraphics, brush, x, y, width, height);
			GdipDeleteBrush(brush);
		} else {
			/* Use GradientFill if supported, only on Windows 98, 2000 and newer. */
			/*
			 * Bug in Windows: On Windows 2000 when the device is a printer,
			 * GradientFill swaps red and blue color components, causing the
			 * gradient to be printed in the wrong color. On Windows 98 when
			 * the device is a printer, GradientFill does not fill completely
			 * to the right edge of the rectangle. The fix is not to use
			 * GradientFill for printer devices.
			 */
			BOOL success = FALSE;
			int rop2 = 0;
			rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
			if (/*IsWinNT &&*/rop2 != R2_XORPEN
					&& GetDeviceCaps(_W_GRAPHICS(gc)->handle, TECHNOLOGY)
							!= DT_RASPRINTER) {

				GRADIENT_RECT gradientRect;
				gradientRect.UpperLeft = 0;
				gradientRect.LowerRight = 1;

				TRIVERTEX trivertex[2];
				trivertex[0].x = x;
				trivertex[0].y = y;
				trivertex[0].Red = (short) ((W_RED(fromRGB) << 8)
						| W_RED(fromRGB));
				trivertex[0].Green = (short) ((W_GREEN(fromRGB) << 8)
						| W_GREEN(fromRGB));
				trivertex[0].Blue = (short) ((W_BLUE(fromRGB) << 8)
						| W_BLUE(fromRGB));
				trivertex[0].Alpha = -1;

				trivertex[1].x = x + width;
				trivertex[1].y = y + height;
				trivertex[1].Red = (short) ((W_RED(toRGB) << 8) | W_RED(toRGB));
				trivertex[1].Green = (short) ((W_GREEN(toRGB) << 8)
						| W_GREEN(toRGB));
				trivertex[1].Blue = (short) ((W_BLUE(toRGB) << 8)
						| W_BLUE(toRGB));
				trivertex[1].Alpha = -1;

				success = GradientFill(_W_GRAPHICS(gc)->handle, trivertex, 2,
						&gradientRect, 1,
						vertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
			}
			if (!success) {

			}
		}
	}
	return result;
}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GdipFillEllipse(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, rect->x,
					rect->y, rect->width, rect->height);
		} else {
			int x = rect->x;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0)
				x--;
			Ellipse(_W_GRAPHICS(gc)->handle, x, rect->y,
					rect->x + rect->width + 1, rect->y + rect->height + 1);
		}
	}
	return result;
}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_w_graphics_init_gdip(gc);
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		_w_graphics *_gc = _W_GRAPHICS(gc);
		GpGraphics *gdipGraphics = _gc->gdipGraphics;
		GpFillMode mode =
				GetPolyFillMode(_gc->handle) == WINDING ?
						FillModeWinding : FillModeAlternate;
		GdipSetPathFillMode( _W_PATH(path)->handle, mode);
		GdipFillPath(gdipGraphics, _gc->gdipBrush, _W_PATH(path)->handle);
	}
	return result;
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *points, int count) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GpFillMode mode =
					GetPolyFillMode(_W_GRAPHICS(gc)->handle) == WINDING ?
							FillModeWinding : FillModeAlternate;
			GdipFillPolygonI(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush,
					(GpPoint*) points, count, mode);
		} else {
			Polygon(_W_GRAPHICS(gc)->handle, (POINT*) points, count);
		}
	}
	return result;
}
wresult w_graphics_fill_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		POINT *points = (POINT*) _w_toolkit_malloc(count * sizeof(POINT));
		if (points == 0)
			return W_ERROR_NO_MEMORY;
		for (int i = 0; i < count; i++) {
			points[i].x = va_arg(args, int);
			points[i].y = va_arg(args, int);
		}
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GpFillMode mode =
					GetPolyFillMode(_W_GRAPHICS(gc)->handle) == WINDING ?
							FillModeWinding : FillModeAlternate;
			GdipFillPolygonI(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush,
					(GpPoint*) points, count, mode);
		} else {
			Polygon(_W_GRAPHICS(gc)->handle, (POINT*) points, count);
		}
		_w_toolkit_free(points, count * sizeof(POINT));
	}
	return result;
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			int x = rect->x;
			int y = rect->y;
			int width = rect->width;
			int height = rect->height;
			if (width < 0) {
				x = x + width;
				width = -width;
			}
			if (height < 0) {
				y = y + height;
				height = -height;
			}
			GdipFillRectangle(_W_GRAPHICS(gc)->gdipGraphics,
			_W_GRAPHICS(gc)->gdipBrush, x, y, width, height);
		} else {
			int rop2 = 0;
			rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
			int dwRop = rop2 == R2_XORPEN ? PATINVERT : PATCOPY;
			PatBlt(_W_GRAPHICS(gc)->handle, rect->x, rect->y, rect->width,
					rect->height, dwRop);
		}
	}
	return result;
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			int nx = rect->x;
			int ny = rect->y;
			int nw = rect->width;
			int nh = rect->height;
			int naw = arcWidth;
			int nah = arcHeight;

			if (nw < 0) {
				nw = 0 - nw;
				nx = nx - nw;
			}
			if (nh < 0) {
				nh = 0 - nh;
				ny = ny - nh;
			}
			if (naw < 0)
				naw = 0 - naw;
			if (nah < 0)
				nah = 0 - nah;

			if (naw == 0 || nah == 0) {
				GdipFillRectangle(gdipGraphics,
				_W_GRAPHICS(gc)->gdipBrush, rect->x, rect->y, rect->width,
						rect->height);
			} else {
				GpPath *path;
				GdipCreatePath(FillModeAlternate, &path);
				if (path == 0)
					return W_ERROR_NO_HANDLES;
				if (nw > naw) {
					if (nh > nah) {
						GdipAddPathArc(path, nx + nw - naw, ny, naw, nah, 0,
								-90);
						GdipAddPathArc(path, nx, ny, naw, nah, -90, -90);
						GdipAddPathArc(path, nx, ny + nh - nah, naw, nah, -180,
								-90);
						GdipAddPathArc(path, nx + nw - naw, ny + nh - nah, naw,
								nah, -270, -90);
					} else {
						GdipAddPathArc(path, nx + nw - naw, ny, naw, nh, -270,
								-180);
						GdipAddPathArc(path, nx, ny, naw, nh, -90, -180);
					}
				} else {
					if (nh > nah) {
						GdipAddPathArc(path, nx, ny, nw, nah, 0, -180);
						GdipAddPathArc(path, nx, ny + nh - nah, nw, nah, -180,
								-180);
					} else {
						GdipAddPathArc(path, nx, ny, nw, nh, 0, 360);
					}
				}
				GdipClosePathFigure(path);
				GdipFillPath(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, path);
				GdipDeletePath(path);
			}
		} else {
			int x = rect->x;
			if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0)
				x--;
			RoundRect(_W_GRAPHICS(gc)->handle, x, rect->y, x + rect->width + 1,
					rect->y + rect->height + 1, arcWidth, arcHeight);
		}
	}
	return result;
}
void _w_graphics_flush(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GdipFlush(_W_GRAPHICS(gc)->gdipGraphics, FlushIntentionFlush);
		/*
		 * Note Flush() does not flush the output to the
		 * underline HDC. This is done by calling GetHDC()
		 * followed by ReleaseHDC().
		 */
		HDC hdc;
		GdipGetDC(_W_GRAPHICS(gc)->gdipGraphics, &hdc);
		GdipReleaseDC(_W_GRAPHICS(gc)->gdipGraphics, hdc);
	}
}
int w_graphics_get_advance_width(w_graphics *gc, int ch) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		INT width;
		GetCharWidthW(_W_GRAPHICS(gc)->handle, ch, ch, &width);
		return width;
	} else
		return 0;
}
wresult w_graphics_get_advanced(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->gdipGraphics != 0;
	}
	return result;
}
int w_graphics_get_alpha(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->alpha;
	}
	return 0xFF;
}
int w_graphics_get_antialias(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics == 0)
			return W_DEFAULT;
		SmoothingMode mode;
		GdipGetSmoothingMode(_W_GRAPHICS(gc)->gdipGraphics, &mode);
		switch (mode) {
		case SmoothingModeDefault:
			return W_DEFAULT;
		case SmoothingModeHighSpeed:
		case SmoothingModeNone:
			return W_OFF;
		case SmoothingModeAntiAlias:
		case SmoothingModeAntiAlias8x8:
		case SmoothingModeHighQuality:
			return W_ON;
		}
		return W_DEFAULT;
	}
	return W_DEFAULT;
}
w_color w_graphics_get_background(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0xFF000000;
	return _W_GRAPHICS(gc)->background;
}
w_pattern* w_graphics_get_background_pattern(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->backgroundPattern;
}
int w_graphics_get_char_width(w_graphics *gc, int ch) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		/* GetCharABCWidths only succeeds on truetype fonts */
		ABC width;
		if (GetCharABCWidthsW(_W_GRAPHICS(gc)->handle, ch, ch, &width)) {
			return width.abcA;
		}
		/* It wasn't a truetype font */
		TEXTMETRICW lptm;
		GetTextMetricsW(_W_GRAPHICS(gc)->handle, &lptm);
		SIZE size;
		WCHAR tmp[3];
		tmp[0] = ch;
		tmp[1] = 0;
		GetTextExtentPoint32W(_W_GRAPHICS(gc)->handle, tmp, 1, &size);
		return size.cx - lptm.tmOverhang;
	}
	return 0;
}
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *r) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GpRectF rect;
			GdipSetPixelOffsetMode(gdipGraphics, PixelOffsetModeNone);
			GdipGetVisibleClipBounds(gdipGraphics, &rect);
			GdipSetPixelOffsetMode(gdipGraphics, PixelOffsetModeHalf);
			r->x = rect.X;
			r->y = rect.Y;
			r->width = rect.Width;
			r->height = rect.Height;
		} else {
			RECT rect;
			GetClipBox(_W_GRAPHICS(gc)->handle, &rect);
			r->x = rect.left;
			r->y = rect.top;
			r->width = rect.right - rect.left;
			r->height = rect.bottom - rect.top;
		}
	}
	return result;
}
wresult w_graphics_get_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (region == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (_W_REGION(region)->handle == 0) {
			_W_REGION(region)->handle = CreateRectRgn(0, 0, 0, 0);
		}
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GpRegion *rgn = 0;
			GdipCreateRegion(&rgn);
			GdipGetClip(gdipGraphics, rgn);
			BOOL ret = FALSE;
			GdipIsInfiniteRegion(rgn, gdipGraphics, &ret);
			if (ret) {
				GpRectF rect;
				GdipSetPixelOffsetMode(gdipGraphics, PixelOffsetModeNone);
				GdipGetVisibleClipBounds(gdipGraphics, &rect);
				GdipSetPixelOffsetMode(gdipGraphics, PixelOffsetModeHalf);
				SetRectRgn(_W_REGION(region)->handle, rect.X, rect.Y,
						rect.X + rect.Width, rect.Y + rect.Height);
			} else {
				GpMatrix *matrix = 0;
				GpMatrix *identity = 0;
				GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
				GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &identity);
				GdipGetWorldTransform(gdipGraphics, matrix);
				GdipSetWorldTransform(gdipGraphics, identity);
				HRGN hRgn = 0;
				GdipGetRegionHRgn(rgn, gdipGraphics, &hRgn);
				GdipSetWorldTransform(gdipGraphics, matrix);
				GdipDeleteMatrix(identity);
				GdipDeleteMatrix(matrix);
				POINT pt;
				GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
				OffsetRgn(hRgn, pt.x, pt.y);
				CombineRgn(_W_REGION(region)->handle, hRgn, 0, RGN_COPY);
				DeleteObject(hRgn);
			}
			GdipDeleteRegion(rgn);
		} else {
			POINT pt;
			GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
			int result = GetClipRgn(_W_GRAPHICS(gc)->handle,
			_W_REGION(region)->handle);
			if (result != 1) {
				RECT rect;
				GetClipBox(_W_GRAPHICS(gc)->handle, &rect);
				SetRectRgn(_W_REGION(region)->handle, rect.left, rect.top,
						rect.right, rect.bottom);
			} else {
				OffsetRgn(_W_REGION(region)->handle, pt.x, pt.y);
			}
			HRGN metaRgn = CreateRectRgn(0, 0, 0, 0);
			if (GetMetaRgn(_W_GRAPHICS(gc)->handle, metaRgn) != 0) {
				OffsetRgn(metaRgn, pt.x, pt.y);
				CombineRgn(_W_REGION(region)->handle, metaRgn,
				_W_REGION(region)->handle, RGN_AND);
				DeleteObject(metaRgn);
				HWND hwnd = _W_GRAPHICS(gc)->hwnd;
				if (hwnd != 0 && _W_GRAPHICS(gc)->ps != 0) {
					HRGN sysRgn = CreateRectRgn(0, 0, 0, 0);
					if (GetRandomRgn(_W_GRAPHICS(gc)->handle, sysRgn, SYSRGN)
							== 1) {
						if (WIN32_VERSION >= VERSION(4, 10)) {
							if ((GetLayout(_W_GRAPHICS(gc)->handle) & LAYOUT_RTL)
									!= 0) {
								int nBytes = GetRegionData(sysRgn, 0, 0);
								RGNDATA *lpRgnData = _w_toolkit_malloc(
										nBytes / 4);
								GetRegionData(sysRgn, nBytes, lpRgnData);
								XFORM xf;
								xf.eM11 = -1;
								xf.eM12 = 0;
								xf.eM21 = 0;
								xf.eM22 = 1;
								xf.eDx = 0;
								xf.eDy = 0;
								HRGN newSysRgn = ExtCreateRegion(&xf, nBytes,
										lpRgnData);
								_w_toolkit_free(lpRgnData, nBytes / 4);
								DeleteObject(sysRgn);
								sysRgn = newSysRgn;
							}
						}
						MapWindowPoints(0, hwnd, &pt, 1);
						OffsetRgn(sysRgn, pt.x, pt.y);
						CombineRgn(_W_REGION(region)->handle, sysRgn,
						_W_REGION(region)->handle, RGN_AND);
					}
					DeleteObject(sysRgn);
				}
			}
		}
	}
	return result;
}
int w_graphics_get_fill_rule(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		return GetPolyFillMode(_W_GRAPHICS(gc)->handle) == WINDING ?
				W_FILL_WINDING : W_FILL_EVEN_ODD;
	}
	return W_FILL_EVEN_ODD;
}
GpBrush* _w_graphics_get_fg_brush(w_graphics *gc) {
	w_pattern *fg = _W_GRAPHICS(gc)->foregroundPattern;
	return fg != 0 ? (GpBrush*) _W_PATTERN(fg)->handle :
	_W_GRAPHICS(gc)->gdipFgBrush;
}
w_font* w_graphics_get_font(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		return _W_GRAPHICS(gc)->font;
	}
	return 0;
}
wresult w_graphics_get_font_metrics(w_graphics *gc,
		w_fontmetrics *fontMetrics) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		GetTextMetricsW(_W_GRAPHICS(gc)->handle, (TEXTMETRICW*) fontMetrics);
		return result;
	} else {
		memset(fontMetrics, 0, sizeof(TEXTMETRICW));
		return result;
	}
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0xFF000000;
	return _W_GRAPHICS(gc)->foreground;
}
w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->foregroundPattern;
}
int w_graphics_get_interpolation(w_graphics *gc) {
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			InterpolationMode mode;
			GdipGetInterpolationMode(_W_GRAPHICS(gc)->gdipGraphics, &mode);
			switch (mode) {
			case InterpolationModeDefault:
				return W_DEFAULT;
			case InterpolationModeNearestNeighbor:
				return W_NONE;
			case InterpolationModeBilinear:
			case InterpolationModeLowQuality:
				return W_LOW;
			case InterpolationModeBicubic:
			case InterpolationModeHighQualityBilinear:
			case InterpolationModeHighQualityBicubic:
			case InterpolationModeHighQuality:
				return W_HIGH;
			}
		}
	}
	return W_DEFAULT;
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return &_W_GRAPHICS(gc)->line;
}
int w_graphics_get_line_cap(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.cap;
}
wresult w_graphics_get_line_dash(w_graphics *gc, int **dashes, int *length) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	*dashes = _W_GRAPHICS(gc)->line.dashes;
	*length = _W_GRAPHICS(gc)->line.length;
	return W_TRUE;
}
int w_graphics_get_line_join(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.join;
}
int w_graphics_get_line_style(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.style;
}
float w_graphics_get_line_width(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return 0;
	return _W_GRAPHICS(gc)->line.width;
}
int w_graphics_get_style(w_graphics *gc) {
	int style = 0;
	if (_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) {
		style |= W_MIRRORED;
	}
	return style;
}
int w_graphics_get_text_antialias(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->handle != 0 && _W_GRAPHICS(gc)->gdipGraphics != 0) {
		TextRenderingHint mode;
		GdipGetTextRenderingHint(_W_GRAPHICS(gc)->gdipGraphics, &mode);
		switch (mode) {
		case TextRenderingHintSystemDefault:
			return W_DEFAULT;
		case TextRenderingHintSingleBitPerPixel:
		case TextRenderingHintSingleBitPerPixelGridFit:
			return W_OFF;
		case TextRenderingHintAntiAlias:
		case TextRenderingHintAntiAliasGridFit:
		case TextRenderingHintClearTypeGridFit:
			return W_ON;
		}
	}
	return W_DEFAULT;
}
wresult w_graphics_get_transform(w_graphics *gc, w_transform *transform) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (transform == 0)
			return W_ERROR_NULL_ARGUMENT;
		if (_W_TRANSFORM(transform)->handle == 0) {
			w_transform_create(transform, 0);
		}
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GdipGetWorldTransform(gdipGraphics,
			_W_TRANSFORM(transform)->handle);
			GpMatrix *identity = _w_graphics_identity(gc);
			GdipInvertMatrix(identity);
			GdipMultiplyMatrix(_W_TRANSFORM(transform)->handle, identity,
					MatrixOrderAppend);
			GdipDeleteMatrix(identity);
		} else {
			w_transformmatrix matrix;
			matrix.m11 = 1;
			matrix.m12 = 0;
			matrix.m21 = 0;
			matrix.m22 = 1;
			matrix.dx = 0;
			matrix.dy = 0;
			w_transform_set_elements(transform, &matrix);
		}
	}
	return result;
}
wresult w_graphics_get_xor_mode(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		int rop2 = 0;
		rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
		return rop2 == R2_XORPEN;
	}
	return result;
}
GpMatrix* _w_graphics_identity(w_graphics *gc) {
	GpMatrix *matrix = 0;
	if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
		int width = 0;
		int technology = GetDeviceCaps(_W_GRAPHICS(gc)->handle, TECHNOLOGY);
		if (technology == DT_RASPRINTER) {
			width = GetDeviceCaps(_W_GRAPHICS(gc)->handle, PHYSICALWIDTH);
		} else {
			if (_W_GRAPHICS(gc)->image != 0) {
				BITMAP bm;
				GetObjectW(_W_IMAGE(_W_GRAPHICS(gc)->image)->handle, sizeof(bm),
						&bm);
				width = bm.bmWidth;
			} else {
				HWND hwnd = WindowFromDC(_W_GRAPHICS(gc)->handle);
				if (hwnd != 0) {
					RECT rect;
					GetClientRect(hwnd, &rect);
					width = rect.right - rect.left;
				} else {
					HBITMAP hBitmap = GetCurrentObject(_W_GRAPHICS(gc)->handle,
					OBJ_BITMAP);
					BITMAP bm;
					GetObjectW(hBitmap, sizeof(bm), &bm);
					width = bm.bmWidth;
				}
			}
		}
		POINT pt;
		GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
		GdipCreateMatrix2(-1, 0, 0, 1, width + 2 * pt.x, 0, &matrix);
		return matrix;
	}
	GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
	return matrix;
}
wresult _w_graphics_init_gdip(w_graphics *gc) {
	if (_W_GRAPHICS(gc)->gdipGraphics != 0)
		return W_TRUE;
	/*
	 * Feature in GDI+. The GDI+ clipping set with Graphics->SetClip()
	 * is always intersected with the GDI clipping at the time the
	 * GDI+ graphics is created.  This means that the clipping
	 * cannot be reset.  The fix is to clear the clipping before
	 * the GDI+ graphics is created and reset it afterwards.
	 */
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	int result = GetClipRgn(_W_GRAPHICS(gc)->handle, hRgn);
	POINT pt;
	GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
	OffsetRgn(hRgn, pt.x, pt.y);
	SelectClipRgn(_W_GRAPHICS(gc)->handle, 0);

	/*
	 * Bug in GDI+.  GDI+ does not work when the HDC layout is RTL.  There
	 * are many issues like pixel corruption, but the most visible problem
	 * is that it does not have an effect when drawing to an bitmap.  The
	 * fix is to clear the bit before creating the GDI+ graphics and install
	 * a mirroring matrix ourselves.
	 */
	if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
		SetLayout(_W_GRAPHICS(gc)->handle,
				GetLayout(_W_GRAPHICS(gc)->handle) & ~LAYOUT_RTL);
	}
	GdipCreateFromHDC(_W_GRAPHICS(gc)->handle, &_W_GRAPHICS(gc)->gdipGraphics);
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GdipSetPageUnit(_W_GRAPHICS(gc)->gdipGraphics, UnitPixel);
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeHalf);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {

			GpMatrix *matrix = _w_graphics_identity(gc);
			GdipSetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, matrix);
			GdipDeleteMatrix(matrix);
		}
		if (result == 1)
			_w_graphics_set_clipping_hrgn(gc, hRgn);
		DeleteObject(hRgn);
		_W_GRAPHICS(gc)->state = 0;
		if (_W_GRAPHICS(gc)->hPen != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, GetStockObject(NULL_PEN));
			DeleteObject(_W_GRAPHICS(gc)->hPen);
			_W_GRAPHICS(gc)->hPen = 0;
		}
		if (_W_GRAPHICS(gc)->hBrush != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, GetStockObject(NULL_BRUSH));
			DeleteObject(_W_GRAPHICS(gc)->hBrush);
			_W_GRAPHICS(gc)->hBrush = 0;
		}
		GdipSetSmoothingMode(_W_GRAPHICS(gc)->gdipGraphics,
				SmoothingModeAntiAlias);
		return W_TRUE;
	} else {
		DeleteObject(hRgn);
		return W_ERROR_NO_HANDLES;
	}
}
wresult w_graphics_is_clipped(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
		if (gdipGraphics != 0) {
			GpRegion *rgn = 0;
			GdipCreateRegion(&rgn);
			GdipGetClip(gdipGraphics, rgn);
			BOOL isInfinite;
			GdipIsInfiniteRegion(rgn, gdipGraphics, &isInfinite);
			GdipDeleteRegion(rgn);
			return !isInfinite;
		} else {
			HRGN region = CreateRectRgn(0, 0, 0, 0);
			int result = GetClipRgn(_W_GRAPHICS(gc)->handle, region);
			DeleteObject(region);
			return result > 0;
		}
	}
	return result;
}
wresult w_graphics_set_advanced(w_graphics *gc, int advanced) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (advanced && _W_GRAPHICS(gc)->gdipGraphics != 0)
		return W_TRUE;
	if (advanced) {
		wresult result = _w_graphics_init_gdip(gc);
		if (result < 0)
			return result;
	} else {
		_w_graphics_dispose_gdip(gc);
		_W_GRAPHICS(gc)->alpha = 0xFF;
		_W_GRAPHICS(gc)->backgroundPattern =
		_W_GRAPHICS(gc)->foregroundPattern = 0;
		w_graphics_set_clipping_region(gc, 0);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			SetLayout(_W_GRAPHICS(gc)->handle,
					GetLayout(_W_GRAPHICS(gc)->handle) | LAYOUT_RTL);
			_W_GRAPHICS(gc)->state = GRAPHICS_STATE_MIRRORED;
		} else
			_W_GRAPHICS(gc)->state = 0;
	}
	return W_TRUE;
}
wresult w_graphics_set_antialias(w_graphics *gc, int antialias) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && antialias == W_DEFAULT)
			return result;
		SmoothingMode mode = 0;
		switch (antialias) {
		case W_DEFAULT:
			mode = SmoothingModeDefault;
			break;
		case W_OFF:
			mode = SmoothingModeNone;
			break;
		case W_ON:
			mode = SmoothingModeAntiAlias;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_w_graphics_init_gdip(gc);
		GdipSetSmoothingMode(_W_GRAPHICS(gc)->gdipGraphics, mode);
	}
	return result;
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && (alpha & 0xFF) == 0xFF)
			return result;
		_w_graphics_init_gdip(gc);
		_W_GRAPHICS(gc)->alpha = alpha & 0xFF;
		_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_BACKGROUND
				| GRAPHICS_STATE_FOREGROUND);
	}
	return result;
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->background = color;
	_W_GRAPHICS(gc)->backgroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_BACKGROUND
			| GRAPHICS_STATE_BACKGROUND_TEXT);
	return W_TRUE;
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (pattern != 0 && w_pattern_is_ok(pattern) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && pattern == 0)
			return result;
		_w_graphics_init_gdip(gc);
		if (_W_GRAPHICS(gc)->backgroundPattern == pattern)
			return result;
		_W_GRAPHICS(gc)->backgroundPattern = pattern;
		_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_BACKGROUND;
	}
	return result;
}
void _w_graphics_set_clipping_hrgn(w_graphics *gc, HRGN clipRgn) {
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		if (clipRgn != 0) {
			GpRegion *region = 0;
			GdipCreateRegionHrgn(clipRgn, &region);
			if (region != 0) {
				GdipSetClipRegion(_W_GRAPHICS(gc)->gdipGraphics, region,
						CombineModeReplace);
				GdipDeleteRegion(region);
			}
		} else {
			GdipResetClip(_W_GRAPHICS(gc)->gdipGraphics);
		}
	} else {
		POINT pt;
		if (clipRgn != 0) {
			GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
			OffsetRgn(clipRgn, -pt.x, -pt.y);
		}
		SelectClipRgn(_W_GRAPHICS(gc)->handle, clipRgn);
		if (clipRgn != 0) {
			OffsetRgn(clipRgn, pt.x, pt.y);
		}
	}
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (path != 0 && _W_PATH(path)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		_w_graphics_set_clipping_hrgn(gc, 0);
		if (path != 0) {
			_w_graphics_init_gdip(gc);
			GpFillMode mode =
					GetPolyFillMode(_W_GRAPHICS(gc)->handle) == WINDING ?
							FillModeWinding : FillModeAlternate;
			GdipSetPathFillMode(_W_PATH(path)->handle, mode);
			GdipSetClipPath(_W_GRAPHICS(gc)->gdipGraphics,
			_W_PATH(path)->handle, CombineModeReplace);
		}
	}
	return result;
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		HRGN hRgn = CreateRectRgn(rect->x, rect->y, rect->x + rect->width,
				rect->y + rect->height);
		_w_graphics_set_clipping_hrgn(gc, hRgn);
		DeleteObject(hRgn);
	}
	return result;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (region != 0 && _W_REGION(region)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		HRGN hRgn = region != 0 ? _W_REGION(region)->handle : 0;
		_w_graphics_set_clipping_hrgn(gc, hRgn);
	}
	return result;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		int mode = ALTERNATE;
		switch (rule) {
		case W_FILL_WINDING:
			mode = WINDING;
			break;
		case W_FILL_EVEN_ODD:
			mode = ALTERNATE;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		SetPolyFillMode(_W_GRAPHICS(gc)->handle, mode);
	}
	return result;
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (font != 0 && w_font_is_ok(font) <= 0)
			return W_ERROR_INVALID_ARGUMENT;
		_W_GRAPHICS(gc)->font =
				font != 0 ?
						font :
						_w_toolkit_get_system_font(W_TOOLKIT(win_toolkit));
		_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FONT;
	}
	return result;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->foreground = color;
	_W_GRAPHICS(gc)->foregroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_FOREGROUND
			| GRAPHICS_STATE_FOREGROUND_TEXT);
	return W_TRUE;
}
wresult w_graphics_set_foreground_pattern(w_graphics *gc, w_pattern *pattern) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (pattern != 0 && w_pattern_is_ok(pattern) < 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && pattern == 0)
			return result;
		_w_graphics_init_gdip(gc);
		if (_W_GRAPHICS(gc)->foregroundPattern == pattern)
			return result;
		_W_GRAPHICS(gc)->foregroundPattern = pattern;
		_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FOREGROUND;
	}
	return result;
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && interpolation == W_DEFAULT)
			return result;
		InterpolationMode mode = 0;
		switch (interpolation) {
		case W_DEFAULT:
			mode = InterpolationModeDefault;
			break;
		case W_NONE:
			mode = InterpolationModeNearestNeighbor;
			break;
		case W_LOW:
			mode = InterpolationModeLowQuality;
			break;
		case W_HIGH:
			mode = InterpolationModeHighQuality;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_w_graphics_init_gdip(gc);
		GdipSetInterpolationMode(_W_GRAPHICS(gc)->gdipGraphics, mode);
	}
	return result;
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	int mask = 0;
	w_line_attributes *line = &_W_GRAPHICS(gc)->line;
	float lineWidth = attributes->width;
	if (lineWidth != line->width) {
		mask |= GRAPHICS_STATE_LINE_WIDTH | GRAPHICS_STATE_DRAW_OFFSET;
	}
	int lineStyle = attributes->style;
	if (lineStyle != line->style) {
		mask |= GRAPHICS_STATE_LINE_STYLE;
		switch (lineStyle) {
		case W_LINE_SOLID:
		case W_LINE_DASH:
		case W_LINE_DOT:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			break;
		case W_LINE_CUSTOM:
			if (attributes->dashes == 0)
				lineStyle = W_LINE_SOLID;
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int join = attributes->join;
	if (join != line->join) {
		mask |= GRAPHICS_STATE_LINE_JOIN;
		switch (join) {
		case W_JOIN_MITER:
		case W_JOIN_ROUND:
		case W_JOIN_BEVEL:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int cap = attributes->cap;
	if (cap != line->cap) {
		mask |= GRAPHICS_STATE_LINE_CAP;
		switch (cap) {
		case W_CAP_FLAT:
		case W_CAP_ROUND:
		case W_CAP_SQUARE:
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
	}
	int *dashes = attributes->dashes;
	int *lineDashes = line->dashes;
	if (dashes != 0 && attributes->length > 0) {
		int changed = lineDashes == 0 || line->length != attributes->length;
		int length = attributes->length;
		for (int i = 0; i < length; i++) {
			float dash = dashes[i];
			if (dash <= 0)
				return W_ERROR_INVALID_ARGUMENT;
			if (!changed && lineDashes[i] != dash)
				changed = W_TRUE;
		}
		if (changed) {
			int *newDashes = (int*) realloc(line->dashes,
					length * sizeof(float));
			if (newDashes == 0)
				return W_ERROR_NO_MEMORY;
			memcpy(newDashes, dashes, length * sizeof(float));
			line->length = length;
			dashes = newDashes;
			mask |= GRAPHICS_STATE_LINE_STYLE;
		} else {
			dashes = lineDashes;
		}
	} else {
		if (lineDashes != 0 && line->length > 0) {
			mask |= GRAPHICS_STATE_LINE_STYLE;
		} else {
			dashes = lineDashes;
		}
	}
	float dashOffset = attributes->dashOffset;
	if (dashOffset != line->dashOffset) {
		mask |= GRAPHICS_STATE_LINE_STYLE;
	}
	float miterLimit = attributes->miterLimit;
	if (miterLimit != line->miterLimit) {
		mask |= GRAPHICS_STATE_LINE_MITERLIMIT;
	}
	if (mask != 0) {
		line->width = lineWidth;
		line->style = lineStyle;
		line->cap = cap;
		line->join = join;
		line->dashes = dashes;
		line->dashOffset = dashOffset;
		line->miterLimit = miterLimit;
		_W_GRAPHICS(gc)->state &= ~mask;
	}
	return W_TRUE;
}
wresult w_graphics_set_line_cap(w_graphics *gc, int cap) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.cap == cap)
		return W_TRUE;
	switch (cap) {
	case W_CAP_ROUND:
	case W_CAP_FLAT:
	case W_CAP_SQUARE:
		break;
	default:
		return W_TRUE;
	}
	_W_GRAPHICS(gc)->line.cap = cap;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_CAP;
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	w_line_attributes *line = &_W_GRAPHICS(gc)->line;
	int *lineDashes = line->dashes;
	if (dashes != 0 && length > 0) {
		wresult changed = _W_GRAPHICS(gc)->line.style != W_LINE_CUSTOM
				|| lineDashes == 0 || line->length != length;
		if (lineDashes != 0) {
			for (int i = 0; i < length; i++) {
				int dash = dashes[i];
				if (dash <= 0)
					dash = 0;	//W_ERROR_INVALID_ARGUMENT;
				if (!changed && lineDashes[i] != dash)
					changed = W_TRUE;
			}
		}
		if (!changed)
			return W_TRUE;
		lineDashes = (int*) realloc(line->dashes, length * sizeof(int));
		line->length = length;
		for (int i = 0; i < length; i++) {
			lineDashes[i] = dashes[i];
		}
		line->dashes = lineDashes;
		line->style = W_LINE_CUSTOM;
	} else {
		if (line->style == W_LINE_SOLID
				&& (lineDashes == 0 || line->length == 0))
			return W_TRUE;
		if (line->dashes != 0) {
			free(line->dashes);
		}
		line->dashes = 0;
		line->style = W_LINE_SOLID;
	}
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.join == join)
		return W_TRUE;
	switch (join) {
	case W_JOIN_MITER:
	case W_JOIN_ROUND:
	case W_JOIN_BEVEL:
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_W_GRAPHICS(gc)->line.join = join;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_JOIN;
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.style == lineStyle)
		return W_TRUE;
	switch (lineStyle) {
	case W_LINE_SOLID:
	case W_LINE_DASH:
	case W_LINE_DOT:
	case W_LINE_DASHDOT:
	case W_LINE_DASHDOTDOT:
		break;
	case W_LINE_CUSTOM:
		if (_W_GRAPHICS(gc)->line.dashes == 0)
			lineStyle = W_LINE_SOLID;
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_W_GRAPHICS(gc)->line.style = lineStyle;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.width == lineWidth)
		return W_TRUE;
	_W_GRAPHICS(gc)->line.width = lineWidth;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_LINE_WIDTH
			| GRAPHICS_STATE_DRAW_OFFSET);
	return W_TRUE;
}
wresult w_graphics_set_xor_mode(w_graphics *gc, int _xor) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		SetROP2(_W_GRAPHICS(gc)->handle, _xor ? R2_XORPEN : R2_COPYPEN);
	}
	return result;
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && antialias == W_DEFAULT)
			return result;
		TextRenderingHint textMode = 0;
		switch (antialias) {
		case W_DEFAULT:
			textMode = TextRenderingHintSystemDefault;
			break;
		case W_OFF:
			textMode = TextRenderingHintSingleBitPerPixelGridFit;
			break;
		case W_ON: {
			int type = 0;
			SystemParametersInfoW(SPI_GETFONTSMOOTHINGTYPE, 0, &type, 0);
			if (type == FE_FONTSMOOTHINGCLEARTYPE) {
				textMode = TextRenderingHintClearTypeGridFit;
			} else {
				textMode = TextRenderingHintAntiAliasGridFit;
			}
		}
			break;
		default:
			return W_ERROR_INVALID_ARGUMENT;
		}
		_w_graphics_init_gdip(gc);
		GdipSetTextRenderingHint(_W_GRAPHICS(gc)->gdipGraphics, textMode);
	}
	return result;
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *transform) {
	wresult result = w_graphics_check(gc, 0);
	if (result > 0) {
		if (transform != 0 && _W_TRANSFORM(transform)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_GRAPHICS(gc)->gdipGraphics == 0 && transform == 0)
			return result;
		_w_graphics_init_gdip(gc);
		GpMatrix *identity = _w_graphics_identity(gc);
		if (transform != 0) {
			GdipMultiplyMatrix(identity, _W_TRANSFORM(transform)->handle,
					MatrixOrderPrepend);
		}
		GdipSetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, identity);
		GdipDeleteMatrix(identity);
		_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_DRAW_OFFSET;
	}
	return result;
}
wresult w_graphics_text_extent(w_graphics *gc, const char *text, int length,
		w_size *size, int flags, int enc) {
	memset(size, 0, sizeof(w_size));
	if (text == 0)
		return W_FALSE;
	wresult result = w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result > 0) {
		size_t newlength;
		WCHAR *s = _win_text_fix(text, length, &newlength, enc);
		if (s != 0) {
			if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
				_w_graphics_draw_text_0(gc, _W_GRAPHICS(gc)->gdipGraphics, s,
						newlength, 0, flags, size);
			} else {
				if (length == 0) {
					SIZE _size;
					WCHAR c = ' ';
					GetTextExtentPoint32W(_W_GRAPHICS(gc)->handle, &c, 1,
							&_size);
					size->width = _size.cx;
					size->height = _size.cy;
				} else {
					RECT rect;
					int uFormat = DT_LEFT | DT_CALCRECT;
					if ((flags & W_DRAW_DELIMITER) == 0)
						uFormat |= DT_SINGLELINE;
					if ((flags & W_DRAW_TAB) != 0)
						uFormat |= DT_EXPANDTABS;
					if ((flags & W_DRAW_MNEMONIC) == 0)
						uFormat |= DT_NOPREFIX;
					DrawTextW(_W_GRAPHICS(gc)->handle, s, length, &rect,
							uFormat);
					size->width = rect.right;
					size->height = rect.bottom;
				}
			}
			_win_text_free(text, s, newlength);
		}
	}
	return result;
}
