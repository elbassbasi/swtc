/*
 * Name:        custom.c
 * Author:      Azeddine EL Bassbasi
 * Created:     2021-01-03
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#include "custom.h"
#include "toolkit.h"
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
/**
 * fixed widget implementation
 */
typedef cairo_rectangle_int_t GtkAllocation;
GType _w_fixed_type = 0;
_w_fixed* _w_fixed_get(GtkWidget *widget) {
	if (_W_IS_FIXED(widget))
		return (_w_fixed*) widget;
	else
		return 0;
}
void _w_fixed_move(GtkWidget *child, gint x, gint y) {
	_w_fixed *_child = _w_fixed_get(child);
	if (_child != 0) {
		_child->alloc.x = x;
		_child->alloc.y = y;
	} else {
		return;
	}
}
void _w_fixed_restack(GtkWidget *fixed, GtkWidget *child, GtkWidget *sibling,
		gboolean above) {

}
void _w_fixed_resize(GtkWidget *child, gint width, gint height) {
	_w_fixed *_child = _w_fixed_get(child);
	if (_child != 0) {
		_child->alloc.width = width;
		_child->alloc.height = height;
	} else {
		return;
	}
	if (width < 0)
		return;
	/*
	 * Feature in GTK: sometimes the sizing of child SwtFixed widgets
	 * does not happen quickly enough, causing miscalculations in SWT.
	 * Allocate the size of the child directly when swt_fixed_resize()
	 * is called. See bug 487160.
	 */
	GtkAllocation to_allocate;
	GtkRequisition req;
	//gtk_widget_get_allocation(child, &allocation);

	// Keep x and y values the same to prevent misplaced containers
	to_allocate.x = _child->alloc.x; // allocation.x;
	to_allocate.y = _child->alloc.y; //allocation.y;
	to_allocate.width = width;
	to_allocate.height = height;

	// Call gtk_widget_get_preferred_size() and finish the allocation.
	gtk_widget_get_preferred_size(child, &req, NULL);
	gtk_widget_size_allocate(child, &to_allocate);
}
void _w_fixed_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	_w_fixed *fixed = _w_fixed_get(widget);
	if (fixed == 0)
		return;
	GtkAllocation child_allocation;
	gtk_widget_set_allocation(widget, allocation);
	gboolean has_window = gtk_widget_get_has_window(widget);

	if (has_window) {
		if (gtk_widget_get_realized(widget)) {
			gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x,
					allocation->y, allocation->width, allocation->height);
		}
	}
	if (fixed->child != 0 && has_window) {
		child_allocation.x = 0;
		child_allocation.y = 0;
		child_allocation.width = allocation->width;
		child_allocation.height = allocation->height;
		gtk_widget_size_allocate(fixed->child, &child_allocation);
	}

	_w_fixed *child = fixed->first;

	while (child) {
		if (has_window && child->ignore_fixed == 0) {
			gtk_widget_size_allocate(GTK_WIDGET(child), &child->alloc);
		}
		child = child->next;
	}
}
void _w_fixed_add(GtkContainer *container, GtkWidget *widget) {
	_w_fixed *fixed = _w_fixed_get((GtkWidget*) container);
	if (fixed == 0)
		return;
	_w_fixed *_child = _w_fixed_get(widget);
	if (_child != 0 && _child->use_as_child == 0) {
		if (fixed->first != 0) {
			_child->next = 0;
			_child->prev = fixed->first->prev;
			fixed->first->prev->next = _child;
			fixed->first->prev = _child;
		} else {
			_child->next = 0;
			fixed->first = _child;
			fixed->first->prev = _child;
		}
		fixed->count++;
	} else {
		if (fixed->child != 0) {
			printf("add multiple child");
			gtk_widget_unparent(fixed->child);
		}
		fixed->child = widget;
	}
	gtk_widget_set_parent(widget, GTK_WIDGET(container));
}
void _w_fixed_remove(GtkContainer *container, GtkWidget *widget) {
	_w_fixed *fixed = _w_fixed_get((GtkWidget*) container);
	if (fixed == 0)
		return;
	if (fixed->child == widget) {
		gtk_widget_unparent(widget);
		fixed->child = 0;
		return;
	}
	_w_fixed *_child = _w_fixed_get(widget);
	if (_child != 0) {
		if (_child == fixed->first) {
			fixed->first = _child->next;
			if (fixed->first != 0) {
				fixed->first->prev = _child->prev;
			}
		} else {
			if (_child->next != 0) {
				_child->next->prev = _child->prev;
			} else {
				fixed->first->prev = _child->prev;
			}
		}
		gtk_widget_unparent(widget);
		fixed->count--;
	}
}

void _w_fixed_forall(GtkContainer *container, gboolean include_internals,
		GtkCallback callback, gpointer callback_data) {
	_w_fixed *fixed = _W_FIXED(container);
	if (fixed == 0)
		return;
	if (fixed->child != 0) {
		callback(fixed->child, callback_data);
	}
	_w_fixed *_child = fixed->first;
	while (_child != 0) {
		if (_child->ignore_fixed == 0)
			callback(GTK_WIDGET(_child), callback_data);
		_child = _child->next;
	}
}
void _w_fixed_finalize(GObject *object) {
	_w_fixed *fixed = _W_FIXED(object);
	g_object_unref(fixed->hadjustment);
	g_object_unref(fixed->vadjustment);
	g_clear_object(&fixed->accessible);
	((GObjectClass*) g_type_class_peek_static(
	GTK_TYPE_CONTAINER))->finalize(object);
}
void _w_fixed_realize(GtkWidget *widget) {
	GtkAllocation allocation;
	GdkWindow *window;
	GdkWindowAttr attributes;
	gint attributes_mask;
	if (!gtk_widget_get_has_window(widget)) {
		GTK_WIDGET_CLASS (g_type_class_peek_static(
						GTK_TYPE_CONTAINER))->realize(widget);
		return;
	}

	gtk_widget_set_realized(widget, TRUE);

	gtk_widget_get_allocation(widget, &allocation);

	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual(widget);
	attributes.event_mask = GDK_EXPOSURE_MASK | GDK_SCROLL_MASK | 1 << 23 /*GDK_SMOOTH_SCROLL_MASK*/
	| gtk_widget_get_events(widget);
	attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;
	window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes,
			attributes_mask);
	gtk_widget_set_window(widget, window);
	gdk_window_set_user_data(window, widget);
	if (NULL != gtk_check_version(3, 18, 0)) {
		gtk_style_context_set_background(gtk_widget_get_style_context(widget),
				window);
	}
}
void _w_fixed_map(GtkWidget *widget) {
	_w_fixed *fixed = _W_FIXED(widget);
	if (fixed == 0)
		return;

	gtk_widget_set_mapped(widget, TRUE);
	if (fixed->child != 0) {
		if (gtk_widget_get_visible(fixed->child)) {
			if (!gtk_widget_get_mapped(fixed->child))
				gtk_widget_map(fixed->child);
		}
	}
	_w_fixed *_child = fixed->first;
	while (_child != 0) {
		if (_child->ignore_fixed == 0) {
			if (gtk_widget_get_visible(GTK_WIDGET(_child))) {
				if (!gtk_widget_get_mapped(GTK_WIDGET(_child)))
					gtk_widget_map(GTK_WIDGET(_child));
			}
		}
		_child = _child->next;
	}
	if (gtk_widget_get_has_window(widget)) {
		//NOTE: contrary to most of GTK, swt_fixed_* container does not raise windows upon showing them.
		//This has the effect that widgets are drawn *beneath* the previous one.
		//E.g if this line is changed to gdk_window_show (..) then widgets are drawn on top of the previous one.
		//This affects mostly only the absolute layout with overlapping widgets, e.g minimizied panels that
		//pop-out in Eclipse (aka fast-view).
		//As such, be attentive to swt_fixed_forall(..); traversing children may need to be done in reverse in some
		//cases.
		gdk_window_show_unraised(gtk_widget_get_window(widget));
	}
}
void _w_fixed_get_preferred_width(GtkWidget *widget, gint *minimum,
		gint *natural) {
	if (minimum) {
		*minimum = 0;
	}
	if (natural) {
		*natural = 0;
	}
}

void _w_fixed_get_preferred_height(GtkWidget *widget, gint *minimum,
		gint *natural) {
	if (minimum) {
		*minimum = 0;
	}
	if (natural) {
		*natural = 0;
	}
}
enum {
	PROP_0,
	PROP_HADJUSTMENT,
	PROP_VADJUSTMENT,
	PROP_HSCROLL_POLICY,
	PROP_VSCROLL_POLICY,
};
void _w_fixed_get_property(GObject *object, guint prop_id, GValue *value,
		GParamSpec *pspec) {
	_w_fixed *fixed = (_w_fixed*) object;

	switch (prop_id) {
	case PROP_HADJUSTMENT:
		g_value_set_object(value, fixed->hadjustment);
		break;
	case PROP_VADJUSTMENT:
		g_value_set_object(value, fixed->vadjustment);
		break;
	case PROP_HSCROLL_POLICY:
		g_value_set_enum(value, fixed->hscroll_policy);
		break;
	case PROP_VSCROLL_POLICY:
		g_value_set_enum(value, fixed->vscroll_policy);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

void _w_fixed_set_property(GObject *object, guint prop_id, const GValue *value,
		GParamSpec *pspec) {
	_w_fixed *fixed = (_w_fixed*) object;
	GtkAdjustment *adjustment;

	switch (prop_id) {
	case PROP_HADJUSTMENT:
		adjustment = (GtkAdjustment*) g_value_get_object(value);
		if (adjustment && fixed->hadjustment == adjustment)
			return;
		if (fixed->hadjustment != NULL)
			g_object_unref(fixed->hadjustment);
		if (adjustment == NULL)
			adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		fixed->hadjustment = (GtkAdjustment*) g_object_ref_sink(adjustment);
		g_object_notify(G_OBJECT(fixed), "hadjustment");
		break;
	case PROP_VADJUSTMENT:
		adjustment = (GtkAdjustment*) g_value_get_object(value);
		if (adjustment && fixed->vadjustment == adjustment)
			return;
		if (fixed->vadjustment != NULL)
			g_object_unref(fixed->vadjustment);
		if (adjustment == NULL)
			adjustment = gtk_adjustment_new(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
		fixed->vadjustment = (GtkAdjustment*) g_object_ref_sink(adjustment);
		g_object_notify(G_OBJECT(fixed), "vadjustment");
		break;
	case PROP_HSCROLL_POLICY:
		fixed->hscroll_policy = g_value_get_enum(value);
		break;
	case PROP_VSCROLL_POLICY:
		fixed->vscroll_policy = g_value_get_enum(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}
void _w_fixed_init(_w_fixed *fixed) {
	memset(&fixed->hadjustment, 0, sizeof(_w_fixed) - sizeof(GtkContainer));
}
void _w_fixed_class_init(GtkContainerClass *clazz) {
	GObjectClass *gobject_class = (GObjectClass*) clazz;
	GtkWidgetClass *widget_class = (GtkWidgetClass*) clazz;
	GtkContainerClass *container_class = (GtkContainerClass*) clazz;

	/* GOject implementation */
	gobject_class->set_property = _w_fixed_set_property;
	gobject_class->get_property = _w_fixed_get_property;
	gobject_class->finalize = _w_fixed_finalize;

	/* Scrollable implemetation */
	g_object_class_override_property(gobject_class, PROP_HADJUSTMENT,
			"hadjustment");
	g_object_class_override_property(gobject_class, PROP_VADJUSTMENT,
			"vadjustment");
	g_object_class_override_property(gobject_class, PROP_HSCROLL_POLICY,
			"hscroll-policy");
	g_object_class_override_property(gobject_class, PROP_VSCROLL_POLICY,
			"vscroll-policy");

	/* Widget implementation */
	widget_class->realize = _w_fixed_realize;
	widget_class->map = _w_fixed_map;
	widget_class->get_preferred_width = _w_fixed_get_preferred_width;
	widget_class->get_preferred_height = _w_fixed_get_preferred_height;
	widget_class->size_allocate = _w_fixed_size_allocate;

	/* Container implementation */
	container_class->add = _w_fixed_add;
	container_class->remove = _w_fixed_remove;
	container_class->forall = _w_fixed_forall;
}
void _w_fixed_registre(const char *name, GType *type) {
	GTypeInfo fixed_info = { sizeof(GtkContainerClass),
	NULL, /* base_init */
	NULL, /* base_finalize */
	(GClassInitFunc) _w_fixed_class_init,
	NULL, /* class_finalize */
	NULL, /* class_data */
	sizeof(_w_fixed), 0, /* n_preallocs */
	(GInstanceInitFunc) _w_fixed_init, NULL };
	//GType parent_class = GTK_TYPE_CONTAINER;

	*type = g_type_register_static(GTK_TYPE_CONTAINER, name, &fixed_info,
			(GTypeFlags) 0);
	GInterfaceInfo interface_info = { NULL, NULL, NULL };
	g_type_add_interface_static(*type, GTK_TYPE_SCROLLABLE, &interface_info);
}
GtkWidget* _w_fixed_new() {
	if (_w_fixed_type == 0) {
		_w_fixed_registre("w_fixed", &_w_fixed_type);
	}
	return g_object_new(_w_fixed_type, NULL);
}
/**
 * image widget implementation
 */
GType _w_image_widget_type = 0;
GtkWidgetClass *parent_class = 0;
gboolean _w_image_widget_draw(GtkWidget *widget, cairo_t *cr) {
	GdkPixbuf *pixbuf = 0;
	_w_image_widget *image = _W_IMAGE_WIDGET(widget);
	if (image->index >= 0) {
		int i = image->index;
		w_imagelist *imagelist = 0;
		image->get_image_list(image->parent, &imagelist);
		if (imagelist != 0) {
			if (i < _W_IMAGELIST(imagelist)->images->count && i >= 0) {
				pixbuf = _W_IMAGELIST(imagelist)->images->images[i];
			}
		}
	}
	g_object_set(widget, "pixbuf", pixbuf, NULL);
	return parent_class->draw(widget, cr);
}
void _w_image_widget_get_preferred_width(GtkWidget *widget, gint *minimum,
		gint *natural) {
	_w_image_widget *image = _W_IMAGE_WIDGET(widget);
	w_imagelist *imagelist = 0;
	image->get_image_list(image->parent, &imagelist);
	if (imagelist != 0) {
		if (minimum) {
			*minimum = _W_IMAGELIST(imagelist)->images->width;
		}
		if (natural)
			*natural = _W_IMAGELIST(imagelist)->images->width;
	} else {
		if (minimum) {
			*minimum = 0;
		}
		if (natural)
			*natural = 0;
	}
}

void _w_image_widget_get_preferred_height(GtkWidget *widget, gint *minimum,
		gint *natural) {
	_w_image_widget *image = _W_IMAGE_WIDGET(widget);
	w_imagelist *imagelist = 0;
	image->get_image_list(image->parent, &imagelist);
	if (imagelist != 0) {
		if (minimum) {
			*minimum = _W_IMAGELIST(imagelist)->images->height;
		}
		if (natural)
			*natural = _W_IMAGELIST(imagelist)->images->height;
	} else {
		if (minimum) {
			*minimum = 0;
		}
		if (natural)
			*natural = 0;
	}
}
void _w_image_widget_init(_w_image_widget *image) {
	image->get_image_list = 0;
	image->parent = 0;
	image->index = -1;
}
void _w_image_widget_class_init(GtkWidgetClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	clazz->draw = _w_image_widget_draw;
	clazz->get_preferred_width = _w_image_widget_get_preferred_width;
	clazz->get_preferred_height = _w_image_widget_get_preferred_height;
}
void _w_image_widget_registre(const char *name, GType *type) {
	GTypeInfo image_widget_info = { sizeof(GtkImageClass),
	NULL, /* base_init */
	NULL, /* base_finalize */
	(GClassInitFunc) _w_image_widget_class_init,
	NULL, /* class_finalize */
	NULL, /* class_data */
	sizeof(_w_image_widget), 0, /* n_preallocs */
	(GInstanceInitFunc) _w_image_widget_init, NULL };

	*type = g_type_register_static(GTK_TYPE_IMAGE, name, &image_widget_info,
			(GTypeFlags) 0);

}
_w_image_widget* _w_image_widget_new() {
	if (_w_image_widget_type == 0) {
		_w_image_widget_registre("w_image", &_w_image_widget_type);
	}
	return (_w_image_widget*) g_object_new(_w_image_widget_type, NULL);
}

