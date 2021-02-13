/*
 * module.c
 *
 *  Created on: 22 janv. 2020
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
#include <unistd.h>
#include <pthread.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>

wresult w_module_unload(w_module *module) {
	dlclose(module);
	return W_TRUE;
}
w_module* w_module_load(const char *module) {
	w_module *m = dlopen(module, RTLD_LAZY | RTLD_GLOBAL);
	return m;
}
w_module* w_module_get(const char *module) {
	w_module *m = dlopen(module, RTLD_NOLOAD | RTLD_GLOBAL);
	return m;
}
void* w_module_get_symbol(w_module *module, const char *symbol) {
	return dlsym(module, symbol);
}
const char* w_module_default_extension() {
	return ".so";
}
