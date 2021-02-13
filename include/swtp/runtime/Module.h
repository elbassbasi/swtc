/*
 * Module.h
 *
 *  Created on: 6 sept. 2020
 *      Author: azeddine
 */

#ifndef SWT_SWTP_RUNTIME_MODULE_H_
#define SWT_SWTP_RUNTIME_MODULE_H_
#include "Thread.h"

class WModule {
private:
	WModule() {

	}
	~WModule() {

	}
public:
	static WModule* Load(const char *module) {
		return w_module_load(module);
	}
	static WModule* Get(const char *module) {
		return w_module_get(module);
	}
	static WResult Unload(WModule *module) {
		return w_module_unload((w_module*) module);
	}
	static const char* DefaultExtension() {
		return w_module_default_extension();
	}
	static void* GetSymbole(WModule *module, const char *symbol) {
		return w_module_get_symbol((w_module*) module, symbol);
	}
	void* GetSymbole(const char *symbol) {
		return GetSymbole(this, symbol);
	}
	WResult Unload() {
		return Unload(this);
	}
};

#endif /* SWT_SWTP_RUNTIME_MODULE_H_ */
