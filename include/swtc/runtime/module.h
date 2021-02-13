/*
 * Name:        module.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWT_RUNTIME_MODULE_H_
#define SWT_RUNTIME_MODULE_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "thread.h"
typedef struct w_module {
	int unused;
} w_module;

SWT_PUBLIC w_module* w_module_load(const char *module);
SWT_PUBLIC w_module* w_module_get(const char *module);
SWT_PUBLIC wresult w_module_unload(w_module *module);
SWT_PUBLIC const char* w_module_default_extension();
SWT_PUBLIC void* w_module_get_symbol(w_module *module, const char *symbol);

#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_MODULE_H_ */
