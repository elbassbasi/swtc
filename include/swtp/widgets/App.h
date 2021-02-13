/*
 * App.h
 *
 *  Created on: 27 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_WIDGETS_APP_H_
#define SWTP_WIDGETS_APP_H_
#include "Toolkit.h"
class SWTP_PUBLIC WApp: public w_app {
public:
	WApp() {
		w_app_init(this);
	}
	WApp(int argc, char **argv) {
		w_app_init_with_args(this, argc, argv);
	}
	~WApp() {
		w_app_dispose(this);
	}
	static WApp* Get() {
		return (WApp*) w_app_get();
	}
	int GetArgc() {
		return this->argc;
	}
	char** GetArgv() {
		return this->argv;
	}
	const char* GetExecutableFile() {
		return w_app_get_executable_file(this);
	}
	const char* GetExecutablePath() {
		return w_app_get_executable_path(this);
	}
	const char* GetCurrentDirectory() {
		return w_app_get_current_directory(this);
	}
	/**
	 * Returns the platform toolkit. One is created (making the
	 * thread that invokes this method its user-interface thread)
	 * if it did not already exist.
	 *
	 * @return the platform toolkit
	 */
	WToolkit* GetPlatformToolkit() {
		return (WToolkit*) w_app_get_platform_toolkit(this);
	}
	/**
	 * Returns the default toolkit. One is created (making the
	 * thread that invokes this method its user-interface thread)
	 * if it did not already exist.
	 *
	 * @return the default toolkit
	 */
	WToolkit* GetDefaultToolkit() {
		return (WToolkit*) w_app_get_default_toolkit(this);
	}
	WToolkit* SetDefaultToolkit(WToolkit *toolkit) {
		return (WToolkit*)w_app_set_default_toolkit(this, W_TOOLKIT(toolkit));
	}
	int Run() {
		return GetDefaultToolkit()->Run();
	}
	void Exit(int exit_code) {
		GetDefaultToolkit()->PostQuit(exit_code);
	}
	void Exit() {
		GetDefaultToolkit()->PostQuit();
	}
	WThread* GetToolkitThread() {
		return GetDefaultToolkit()->GetThread();
	}
	WThread* GetMainThread() {
		return GetDefaultToolkit()->GetThread();
	}
	bool IsMainThread() {
		return w_threadid_equal(GetMainThread()->GetID(),
				w_thread_get_current_id());
	}
};

namespace swt {
typedef ::WApp App;
}  // namespace swt

#endif /* SWTP_WIDGETS_APP_H_ */
