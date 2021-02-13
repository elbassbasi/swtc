/*
 * Name:        line.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_GRAPHICS_LINE_H_
#define SWT_GRAPHICS_LINE_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_line_attributes {
	unsigned style :3;
	unsigned cap :2;
	unsigned join :2;
	unsigned dashOffset :13;
	unsigned miterLimit :12;
	float width;
	int length;
	int *dashes;
} w_line_attributes;
typedef struct w_line {
	unsigned style :3;
	unsigned cap :2;
	unsigned join :2;
	unsigned dashOffset :13;
	unsigned miterLimit :12;
	float width;
	int length;
	int dashes[0];
} w_line;

#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_LINE_H_ */
