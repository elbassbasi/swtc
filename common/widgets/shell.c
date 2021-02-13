/*
 * shell.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
wresult w_shell_create(w_shell *shell, w_toolkit *toolkit, w_shell *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(shell), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SHELL, post_event);
}
w_shell* w_shell_new(w_toolkit *toolkit, w_shell *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return (w_shell*) _w_widget_new(toolkit, W_WIDGET(parent), style,
			_W_CLASS_SHELL, post_event);
}
wresult w_shell_close(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->close(shell);
	} else
		return ret;
}
wresult w_shell_get_toolbar(w_shell *shell,w_toolbar** toolbar) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_toolbar(shell,toolbar);
    } else{
        *toolbar = 0;
		return ret;
    }
}
wresult w_shell_get_alpha(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_alpha(shell);
	} else
		return ret;
}
wresult w_shell_get_full_screen(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_full_screen(shell);
	} else
		return ret;
}
wresult w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_minimum_size(shell, size);
	} else {
		memset(size, 0, sizeof(w_size));
		return ret;
	}
}
wresult w_shell_get_modified(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_modified(shell);
	} else
		return ret;
}
wresult w_shell_get_ime_input_mode(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_ime_input_mode(shell);
	} else
		return ret;
}
wresult w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	w_iterator_close(iterator);
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_shells(shell, iterator);
	} else
		return ret;
}
wresult w_shell_open(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->open(shell);
	} else
		return ret;
}
wresult w_shell_set_active(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_active(shell);
	} else
		return ret;
}
wresult w_shell_set_alpha(w_shell *shell, int alpha) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_alpha(shell, alpha);
	} else
		return ret;
}
wresult w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_full_screen(shell, fullScreen);
	} else
		return ret;
}
wresult w_shell_set_ime_input_mode(w_shell *shell, int mode) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_ime_input_mode(shell, mode);
	} else
		return ret;
}
wresult w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_minimum_size(shell, size);
	} else
		return ret;
}
wresult w_shell_set_modified(w_shell *shell, int modified) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_modified(shell, modified);
	} else
		return ret;
}
wresult w_shell_force_active(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->force_active(shell);
	} else
		return ret;
}
wresult w_shell_get_default_button(w_shell *shell,w_button** button) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_default_button(shell,button);
    } else{
        *button = 0;
		return ret;
    }
}
wresult w_shell_get_images(w_shell *shell, w_image *images, size_t length) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_images(shell, images, length);
	} else
		return ret;
}
wresult w_shell_get_maximized(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_maximized(shell);
	} else
		return ret;
}
wresult w_shell_get_menu_bar(w_shell *shell,w_menu** menubar) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_menu_bar(shell,menubar);
    } else{
        *menubar = 0;
		return ret;
    }
}
wresult w_shell_get_minimized(w_shell *shell) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_minimized(shell);
	} else
		return ret;
}
wresult w_shell_get_text(w_shell *shell, w_alloc alloc,void* user_data,int enc) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->get_text(shell, alloc,user_data,enc);
	} else
		return ret;
}
wresult w_shell_set_default_button(w_shell *shell, w_button *button) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_default_button(shell, button);
	} else
		return ret;
}
wresult w_shell_set_images(w_shell *shell, w_image *images, size_t length) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_images(shell, images, length);
	} else
		return ret;
}
wresult w_shell_set_maximized(w_shell *shell, int maximized) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_maximized(shell, maximized);
	} else
		return ret;
}
wresult w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_menu_bar(shell, menu);
	} else
		return ret;
}
wresult w_shell_set_minimized(w_shell *shell, int minimized) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_minimized(shell, minimized);
	} else
		return ret;
}
wresult w_shell_set_text(w_shell *shell, const char *string, size_t length,int enc) {
	wresult ret = W_WIDGET_CHECK0(shell);
	if (ret > 0) {
		return W_SHELL_GET_CLASS(shell)->set_text(shell, string, length,enc);
	} else
		return ret;
}
