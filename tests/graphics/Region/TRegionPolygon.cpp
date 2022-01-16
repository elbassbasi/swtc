/*
 * TRegionPolygon.cpp
 *
 *  Created on: 16 janv. 2022
 *      Author: azeddine
 */

#include "TRegionPolygon.h"
void TRegionPolygon::Registre(WTreeItem &parent) {
	ITreeItem::Regitre(parent, "Polygon", new TRegionPolygon());
}
TRegionPolygon::TRegionPolygon() {
}

TRegionPolygon::~TRegionPolygon() {
}

void TRegionPolygon::CreateControl(WComposite *parent) {
	CanvasTreeItem::CreateControl(parent);
}
bool TRegionPolygon::OnPaint(WPaintEvent &e) {
	WRect r,bounds;
	GetClientArea(r);
	WRegion region;
	region.Create();
	WPoint *points;
	/*int count = Circle(&points, 67, 67, 67);
	e->SetBackground(W_COLOR_RED);
	e->FillPolygon(points, count);
	region.Add(points, count);
	delete[] points;*/
	region.AddElliptic(0, 0, 100, 100);
	region.SubtractElliptic(20, 20, 60, 60);
	/*count = Circle(&points, 20, 67, 50);
	region.Subtract(points, count);
	delete[] points;
	int _p[] = { 67, 50, 55, 105, 79, 105 };
	region.Subtract((WPoint*) _p, sizeof(_p) / (2 * sizeof(_p[0])));*/
	region.GetBounds(bounds);
	e->SetClipping(region);
	e->SetBackground(W_COLOR_BLUE);
	e->FillRectangle(r);
	return true;
}
