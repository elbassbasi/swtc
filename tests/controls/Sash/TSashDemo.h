/*
 * TSashDemo.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef TESTS_CONTROLS_SASH_TSASHDEMO_H_
#define TESTS_CONTROLS_SASH_TSASHDEMO_H_
#include "TSashes.h"
#define NUMBER_OF_CONTROL 5
class TSashDemo : public CompositeTreeItem {
public:
	WSashLayout<NUMBER_OF_CONTROL> layout;
	WButton btn[NUMBER_OF_CONTROL];
	static void Registre(WTreeItem &parent);
	void CreateControl(WComposite *parent);
};

#endif /* TESTS_CONTROLS_SASH_TSASHDEMO_H_ */
