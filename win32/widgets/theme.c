/*
 * Name:        theme.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "toolkit.h"

void _w_theme_init(){
	win_toolkit->theme =(w_theme*) &win_toolkit->win32theme;

}

