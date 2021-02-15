/*
 * TTransformImage.cpp
 *
 *  Created on: 8 févr. 2021
 *      Author: azeddine
 */

#include "TTransformImage.h"
void TTransformImage::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "image", new TTransformImage());
}
TTransformImage::TTransformImage() {
}
TTransformImage::~TTransformImage() {
}
void TTransformImage::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
	font.Create("Arial", 30, W_BOLD);
}

void TTransformImage::DrawText(WGraphics &gc, const char *text, int x, int y) {
	//gc.SetFont(&font);
	gc.SetBackground(W_COLOR_RED);
	gc.FillRectangle(x, y, 110, 60);
	gc.SetForeground(W_COLOR_WHITE);
	gc.DrawTextTransparent(text, x + 10, y + 10);
}

bool TTransformImage::OnPaint(WPaintEvent &e) {
	WSize size(110, 60);
	// Original image
	int x = 30, y = 30;
	DrawText(*e.gc, "Original", x, y);
	x += size.width + 30;

	WTransform transform;
	transform.Create();

	// Reflect around the y axis.
	transform.SetElements(-1, 0, 0, 1, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "xReflect", -1 * x - size.width, y);

	x = 30;
	y += size.height + 30;

	// Reflect around the x axis.
	transform.SetElements(1, 0, 0, -1, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "yReflect", x, -1 * y - size.height);

	x += size.width + 30;

	// Reflect around the x and y axes
	transform.SetElements(-1, 0, 0, -1, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "xyReflect", -1 * x - size.width, -1 * y - size.height);

	x = 30;
	y += size.height + 30;

	// Shear in the x-direction
	transform.SetElements(1, 0, -1, 1, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "xShear", 300, y);

	// Shear in y-direction
	transform.SetElements(1, -1, 0, 1, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "yShear", 150, 475);

	// Rotate by 45 degrees
	float cos45 = cosf(M_PI / 4);
	float sin45 = sinf(M_PI / 4);
	transform.SetElements(cos45, sin45, -sin45, cos45, 0, 0);
	e->SetTransform(transform);
	DrawText(*e.gc, "rotate", 400, 60);
	return true;
}
