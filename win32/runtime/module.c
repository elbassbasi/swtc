/*
 * Name:        module.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "core.h"
w_module* w_module_load(const char *module) {
	size_t newlength;
	WCHAR *s = _win_text_fix(module, -1, &newlength, W_ENCODING_UTF8);
	w_module *hmodule = (w_module*) LoadLibraryW(s);
	_win_text_free(module, s, newlength);
	return hmodule;
}
w_module* w_module_get(const char *module) {
	size_t newlength;
	WCHAR *s = _win_text_fix(module, -1, &newlength, W_ENCODING_UTF8);
	w_module *hmodule = (w_module*) GetModuleHandleW(s);
	_win_text_free(module, s, newlength);
	return hmodule;
}
wresult w_module_unload(w_module *module) {
	return FreeLibrary((HMODULE) module);
}
const char* w_module_default_extension() {
	return ".dll";
}
void* w_module_get_symbol(w_module *module, const char *symbol) {
	return GetProcAddress((HMODULE) module, symbol);
}
