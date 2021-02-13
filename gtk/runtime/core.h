/*
 * Name:        core.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef GTK_RUNTIME_CORE_H_
#define GTK_RUNTIME_CORE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <swtc/swtc.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <cairo/cairo.h>
#include <gio/gio.h>
#define GTK4 (GTK_MAJOR_VERSION == 4)
#define GTK3 (GTK_MAJOR_VERSION == 3)
#define GTK2 (GTK_MAJOR_VERSION == 2)
#define GTK1 (GTK_MAJOR_VERSION == 1)
#define USE_CAIRO 1
#define VERSION(major,minor,micro) ((major << 16) + (minor << 8) + micro)
#define H_GTK_VERSION VERSION(GTK_MAJOR_VERSION,GTK_MINOR_VERSION,GTK_MICRO_VERSION)
#define H_GDK_VERSION VERSION(GDK_MAJOR_VERSION,GDK_MINOR_VERSION,GDK_MICRO_VERSION)
#define IMAGE_REF (1 << 1)
#define _GTK_TEXT_FIX_REQUIRED_NULL (1 << 16)
#define _GTK_TEXT_FIX_CALCUL_LENGTH (1 << 17)
#define _GTK_TEXT_FIX_COPY (1 << 18)
#define WMIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define WMAX(X, Y) (((X) > (Y)) ? (X) : (Y))
typedef cairo_rectangle_int_t GdkRectangle;
int _mnemonics_from_gtk_0(const char *text, size_t text_length, char *out,
		size_t out_length, int *process);
char* _gtk_text_fix(const char *text, int text_length, int enc, int *newlength,
		int *mnemonic);
void _gtk_text_free(const char *text, char *alloc, int length);
wresult _gtk_alloc_set_text(w_alloc string, void *user_data, const char *text,
		size_t text_length, int enc);
#ifdef __cplusplus
}
#endif
#endif /* GTK_RUNTIME_CORE_H_ */
