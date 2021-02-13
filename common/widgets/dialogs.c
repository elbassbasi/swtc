/*
 * dialogs.c
 *
 *  Created on: 2 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>

wresult w_messagebox_open(struct w_toolkit *toolkit, w_messagebox *messagebox){
	if(toolkit == 0) toolkit = w_app_get_default_toolkit(w_app_get());
	return W_TOOLKIT_GET_CLASS(toolkit)->messagebox_open(toolkit,messagebox);
}

