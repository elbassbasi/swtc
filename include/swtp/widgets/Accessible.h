/*
 * Name:        Accessible.h
 * Author:      Azeddine EL Bassbasi
 * Created:     2020-12-29
 * Copyright:   (c) 2020 Azeddine EL Bassbasi
 * Licence:
 */
#ifndef SWTP_WIDGETS_ACCESSIBLE_H_
#define SWTP_WIDGETS_ACCESSIBLE_H_
#include "Widget.h"
/**
 * Instances of this class provide a bridge between application
 * code and assistive technology clients. Many platforms provide
 * default accessible behavior for most widgets, and this class
 * allows that default behavior to be overridden. Applications
 * can get the default Accessible object for a control by sending
 * it <code>getAccessible</code>, and then add an accessible listener
 * to override simple items like the name and help string, or they
 * can add an accessible control listener to override complex items.
 * As a rule of thumb, an application would only want to use the
 * accessible control listener to implement accessibility for a
 * custom control.
 *
 * @see Control#getAccessible
 * @see AccessibleListener
 * @see AccessibleEvent
 * @see AccessibleControlListener
 * @see AccessibleControlEvent
 */
class SWTP_PUBLIC WAccessible {
public:
	/**
	 * Constructs a new instance of this class given its parent.
	 *
	 * @param parent the Accessible parent, which must not be null
	 *
	 * @see #dispose
	 * @see Control#getAccessible
	 */
	WAccessible(WAccessible *parent) {

	}
	/**
	 * Adds a relation with the specified type and target
	 * to the receiver's set of relations.
	 *
	 * @param type an <code>ACC</code> constant beginning with RELATION_* indicating the type of relation
	 * @param target the accessible that is the target for this relation
	 *
	 * @since 3.6
	 */
	void AddRelation(int type, WAccessible *target) {
		w_accessible_add_relation(W_ACCESSIBLE(this), type,
				W_ACCESSIBLE(target));
	}
	/**
	 * Returns the control for this Accessible object.
	 *
	 * @return the receiver's control
	 */
	WControl* GetControl() {
		return (WControl*) w_accessible_get_control(W_ACCESSIBLE(this));
	}
	/**
	 * Removes the relation with the specified type and target
	 * from the receiver's set of relations.
	 *
	 * @param type an <code>ACC</code> constant beginning with RELATION_* indicating the type of relation
	 * @param target the accessible that is the target for this relation
	 */
	void RemoveRelation(int type, WAccessible *target) {
		w_accessible_remove_relation(W_ACCESSIBLE(this), type,
				W_ACCESSIBLE(target));
	}
	/**
	 * Sends a message with event-specific data to accessible clients
	 * indicating that something has changed within a custom control.
	 *
	 * @param event an <code>ACC</code> constant beginning with EVENT_* indicating the message to send
	 * @param eventData an object containing event-specific data, or null if there is no event-specific data
	 * (eventData is specified in the documentation for individual ACC.EVENT_* constants)
	 *
	 * @see ACC#EVENT_ACTION_CHANGED
	 * @see ACC#EVENT_ATTRIBUTE_CHANGED
	 * @see ACC#EVENT_DESCRIPTION_CHANGED
	 * @see ACC#EVENT_DOCUMENT_LOAD_COMPLETE
	 * @see ACC#EVENT_DOCUMENT_LOAD_STOPPED
	 * @see ACC#EVENT_DOCUMENT_RELOAD
	 * @see ACC#EVENT_HYPERLINK_ACTIVATED
	 * @see ACC#EVENT_HYPERLINK_ANCHOR_COUNT_CHANGED
	 * @see ACC#EVENT_HYPERLINK_END_INDEX_CHANGED
	 * @see ACC#EVENT_HYPERLINK_SELECTED_LINK_CHANGED
	 * @see ACC#EVENT_HYPERLINK_START_INDEX_CHANGED
	 * @see ACC#EVENT_HYPERTEXT_LINK_COUNT_CHANGED
	 * @see ACC#EVENT_HYPERTEXT_LINK_SELECTED
	 * @see ACC#EVENT_LOCATION_CHANGED
	 * @see ACC#EVENT_NAME_CHANGED
	 * @see ACC#EVENT_PAGE_CHANGED
	 * @see ACC#EVENT_SECTION_CHANGED
	 * @see ACC#EVENT_SELECTION_CHANGED
	 * @see ACC#EVENT_STATE_CHANGED
	 * @see ACC#EVENT_TABLE_CAPTION_CHANGED
	 * @see ACC#EVENT_TABLE_CHANGED
	 * @see ACC#EVENT_TABLE_COLUMN_DESCRIPTION_CHANGED
	 * @see ACC#EVENT_TABLE_COLUMN_HEADER_CHANGED
	 * @see ACC#EVENT_TABLE_ROW_DESCRIPTION_CHANGED
	 * @see ACC#EVENT_TABLE_ROW_HEADER_CHANGED
	 * @see ACC#EVENT_TABLE_SUMMARY_CHANGED
	 * @see ACC#EVENT_TEXT_ATTRIBUTE_CHANGED
	 * @see ACC#EVENT_TEXT_CARET_MOVED
	 * @see ACC#EVENT_TEXT_CHANGED
	 * @see ACC#EVENT_TEXT_COLUMN_CHANGED
	 * @see ACC#EVENT_TEXT_SELECTION_CHANGED
	 * @see ACC#EVENT_VALUE_CHANGED
	 */
	void SendEvent(int event, void *eventData) {
		w_accessible_send_event(W_ACCESSIBLE(this), event, eventData);
	}
	/**
	 * Sends a message to accessible clients that the child selection
	 * within a custom container control has changed.
	 */
	void SelectionChanged() {
		w_accessible_selection_changed(W_ACCESSIBLE(this));
	}
	/**
	 * Sends a message to accessible clients indicating that the focus
	 * has changed within a custom control.
	 *
	 * @param childID an identifier specifying a child of the control
	 */
	void SetFocus(int childID) {
		w_accessible_set_focus(W_ACCESSIBLE(this), childID);
	}
	/**
	 * Sends a message to accessible clients that the text
	 * caret has moved within a custom control.
	 *
	 * @param index the new caret index within the control
	 */
	void TextCaretMoved(int index) {
		w_accessible_text_caret_moved(W_ACCESSIBLE(this), index);
	}
	/**
	 * Sends a message to accessible clients that the text
	 * within a custom control has changed.
	 *
	 * @param type the type of change, one of <code>ACC.TEXT_INSERT</code>
	 * or <code>ACC.TEXT_DELETE</code>
	 * @param startIndex the text index within the control where the insertion or deletion begins
	 * @param length the non-negative length in characters of the insertion or deletion
	 *
	 * @see ACC#TEXT_INSERT
	 * @see ACC#TEXT_DELETE
	 */
	void TextChanged(int type, int startIndex, int length) {
		w_accessible_text_changed(W_ACCESSIBLE(this), type, startIndex, length);
	}
	/**
	 * Sends a message to accessible clients that the text
	 * selection has changed within a custom control.
	 */
	void TextSelectionChanged() {
		w_accessible_text_selection_changed(W_ACCESSIBLE(this));
	}
private:
	void* handles[sizeof(w_accessible)/sizeof(void*)];
};
namespace swt {
typedef ::WAccessible Accessible;
}  // namespace swt
#endif /* SWTP_WIDGETS_ACCESSIBLE_H_ */
