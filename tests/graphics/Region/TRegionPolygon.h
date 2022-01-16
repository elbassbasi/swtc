/*
 * TRegionPolygon.h
 *
 *  Created on: 16 janv. 2022
 *      Author: azeddine
 */

#ifndef TESTS_GRAPHICS_REGION_TREGIONPOLYGON_H_
#define TESTS_GRAPHICS_REGION_TREGIONPOLYGON_H_
#include "TRegions.h"
class TRegionPolygon : public CanvasTreeItem {
public:
	static void Registre(WTreeItem &parent);
	TRegionPolygon();
	~TRegionPolygon();
	void CreateControl(WComposite *parent);
	bool OnPaint(WPaintEvent &e);
};

#endif /* TESTS_GRAPHICS_REGION_TREGIONPOLYGON_H_ */
