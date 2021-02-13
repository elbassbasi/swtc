/*
 * canvas.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swtc.h>
/*
 * caret
 */
wresult w_caret_get_bounds(w_caret *caret,w_point* location,w_size* size) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->get_bounds(caret, location,size);
    } else{
        if(location != 0){
            memset(location, 0, sizeof(w_point));
        }
        if(size != 0){
            memset(size, 0, sizeof(w_size));
        }
		return result;
    }
}
wresult w_caret_get_font(w_caret *caret,w_font** font) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->get_font(caret,font);
	} else{
		*font = 0;
		return result;
	}
}
wresult w_caret_get_image(w_caret *caret, w_image *image) {
    w_image_dispose(image);
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->get_image(caret, image);
	} else
		return result;
}
wresult w_caret_get_parent(w_caret *caret,w_canvas** parent) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->get_parent(caret,parent);
	} else{
		*parent = 0;
		return result;
	}
}
wresult w_caret_get_visible(w_caret *caret) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->get_visible(caret);
	} else
		return result;
}
wresult w_caret_is_visible(w_caret *caret) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->is_visible(caret);
	} else
		return result;
}
wresult w_caret_set_bounds(w_caret *caret,w_point* location,w_size* size) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->set_bounds(caret, location, size);
	} else
		return result;
}
wresult w_caret_set_font(w_caret *caret, w_font *font) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->set_font(caret, font);
	} else
		return result;
}
wresult w_caret_set_image(w_caret *caret, w_image *image) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->set_image(caret, image);
	} else
		return result;
}
wresult w_caret_set_visible(w_caret *caret, int visible) {
	wresult result = W_WIDGETDATA_CHECK0(caret);
	if (result > 0) {
		return W_CARET_GET_CLASS(caret)->set_visible(caret, visible);
	} else
		return result;
}
/*
 * IME
 */
wresult w_ime_get_caret_offset(w_ime *ime) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_caret_offset(ime);
	} else
		return result;
}
wresult w_ime_get_commit_count(w_ime *ime) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_commit_count(ime);
	} else
		return result;
}
wresult w_ime_get_composition_offset(w_ime *ime) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_composition_offset(ime);
	} else
		return result;
}
wresult w_ime_get_ranges(w_ime *ime, w_iterator *ranges) {
    w_iterator_close(ranges);
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_ranges(ime, ranges);
	} else
		return result;
}
wresult w_ime_get_text(w_ime *ime, w_alloc alloc,void* user_data) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_text(ime, alloc,user_data);
	} else
		return result;
}
wresult w_ime_get_wide_caret(w_ime *ime) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->get_wide_caret(ime);
	} else
		return result;
}
wresult w_ime_set_composition_offset(w_ime *ime, int offset) {
	wresult result = W_WIDGETDATA_CHECK0(ime);
	if (result > 0) {
		return W_IME_GET_CLASS(ime)->set_composition_offset(ime, offset);
	} else
		return result;
}

/*
 * canvas
 */
wresult w_canvas_create(w_canvas *canvas, w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(canvas), toolkit, W_WIDGET(parent), style,
			_W_CLASS_CANVAS, post_event);
}
w_canvas* w_canvas_new(w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event) {
	return W_CANVAS(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_CANVAS,post_event));
}
wresult w_canvas_create_caret(w_canvas *canvas, w_caret *caret) {
	w_widgetdata_close(W_WIDGETDATA(caret));
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->create_caret(canvas, caret);
	} else
		return result;
}
wresult w_canvas_scroll(w_canvas *canvas, w_point *dest, w_rect *rect,
		int all) {
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->scroll(canvas, dest, rect, all);
	} else
		return result;
}
wresult w_canvas_get_caret(w_canvas *canvas,w_caret** caret) {
    wresult result = W_WIDGETDATA_CHECK0(canvas);
    if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->get_caret(canvas,caret);
    } else{
        *caret = 0;
		return result;
    }
}
wresult w_canvas_create_ime(w_canvas *canvas, w_ime *ime) {
	w_widgetdata_close(W_WIDGETDATA(ime));
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->create_ime(canvas, ime);
	} else
		return result;
}
wresult w_canvas_set_caret(w_canvas *canvas, w_caret *caret) {
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->set_caret(canvas, caret);
	} else
		return result;
}
wresult w_canvas_set_ime(w_canvas *canvas, w_ime *ime) {
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->set_ime(canvas, ime);
	} else
		return result;
}
wresult w_canvas_get_ime(w_canvas *canvas,w_ime** ime) {
	wresult result = W_WIDGET_CHECK0(canvas);
	if (result > 0) {
		return W_CANVAS_GET_CLASS(canvas)->get_ime(canvas,ime);
    } else{
        *ime = 0;
		return result;
    }
}
