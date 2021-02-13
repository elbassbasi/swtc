/*
 * DateTime.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_DATETIME_H_
#define SWTP_CONTROLS_DATETIME_H_
#include "../widgets/Composite.h"
/**
 * Instances of this class are selectable user interface
 * objects that allow the user to enter and modify date
 * or time values.
 * <p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to add children to it, or set a layout on it.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>DATE, TIME, CALENDAR, SHORT, MEDIUM, LONG, DROP_DOWN</dd>
 * <dt><b>Events:</b></dt>
 * <dd>DefaultSelection, Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles DATE, TIME, or CALENDAR may be specified,
 * and only one of the styles SHORT, MEDIUM, or LONG may be specified.
 * The DROP_DOWN style is a <em>HINT</em>, and it is only valid with the DATE style.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WDateTime: public WComposite {
public:
	WDateTime() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#DATE
	 * @see SWT#TIME
	 * @see SWT#CALENDAR
	 * @see SWT#SHORT
	 * @see SWT#MEDIUM
	 * @see SWT#LONG
	 * @see SWT#DROP_DOWN
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WDateTime(WComposite *parent, int style) {
		Create(parent, style);
	}
	/**
	 * Returns the receiver's date, or day of the month.
	 * <p>
	 * The first day of the month is 1, and the last day depends on the month and year.
	 * </p>
	 *
	 * @return a positive integer beginning with 1
	 */
	int GetDay() {
		return w_datetime_get_day(W_DATETIME(this));
	}
	/**
	 * Returns the receiver's hours.
	 * <p>
	 * Hours is an integer between 0 and 23.
	 * </p>
	 *
	 * @return an integer between 0 and 23
	 */
	int GetHours() {
		return w_datetime_get_hours(W_DATETIME(this));
	}
	/**
	 * Returns the receiver's minutes.
	 * <p>
	 * Minutes is an integer between 0 and 59.
	 * </p>
	 *
	 * @return an integer between 0 and 59
	 */
	int GetMinutes() {
		return w_datetime_get_minutes(W_DATETIME(this));
	}
	/**
	 * Returns the receiver's month.
	 * <p>
	 * The first month of the year is 0, and the last month is 11.
	 * </p>
	 *
	 * @return an integer between 0 and 11
	 */
	int GetMonth() {
		return w_datetime_get_month(W_DATETIME(this));
	}
	/**
	 * Returns the receiver's seconds.
	 * <p>
	 * Seconds is an integer between 0 and 59.
	 * </p>
	 *
	 * @return an integer between 0 and 59
	 */
	int GetSeconds() {
		return w_datetime_get_seconds(W_DATETIME(this));
	}

	/**
	 * Returns the receiver's year.
	 * <p>
	 * The first year is 1752 and the last year is 9999.
	 * </p>
	 *
	 * @return an integer between 1752 and 9999
	 */
	int GetYear() {
		return w_datetime_get_year(W_DATETIME(this));
	}
	/**
	 * Sets the receiver's year, month, and day in a single operation.
	 * <p>
	 * This is the recommended way to set the date, because setting the year,
	 * month, and day separately may result in invalid intermediate dates.
	 * </p>
	 *
	 * @param year an integer between 1752 and 9999
	 * @param month an integer between 0 and 11
	 * @param day a positive integer beginning with 1
	 */
	bool SetDate(int year, int month, int day) {
		return w_datetime_set_date(W_DATETIME(this), year, month, day) > 0;
	}
	/**
	 * Sets the receiver's date, or day of the month, to the specified day.
	 * <p>
	 * The first day of the month is 1, and the last day depends on the month and year.
	 * If the specified day is not valid for the receiver's month and year, then it is ignored.
	 * </p>
	 *
	 * @param day a positive integer beginning with 1
	 *
	 * @see #setDate
	 */
	bool SetDay(int day) {
		return w_datetime_set_day(W_DATETIME(this), day) > 0;
	}
	/**
	 * Sets the receiver's hours.
	 * <p>
	 * Hours is an integer between 0 and 23.
	 * </p>
	 *
	 * @param hours an integer between 0 and 23
	 */
	bool SetHours(int hours) {
		return w_datetime_set_hours(W_DATETIME(this), hours) > 0;
	}
	/**
	 * Sets the receiver's minutes.
	 * <p>
	 * Minutes is an integer between 0 and 59.
	 * </p>
	 *
	 * @param minutes an integer between 0 and 59
	 */
	bool SetMinutes(int minutes) {
		return w_datetime_set_minutes(W_DATETIME(this), minutes) > 0;
	}
	/**
	 * Sets the receiver's month.
	 * <p>
	 * The first month of the year is 0, and the last month is 11.
	 * If the specified month is not valid for the receiver's day and year, then it is ignored.
	 * </p>
	 *
	 * @param month an integer between 0 and 11
	 *
	 * @see #setDate
	 */
	bool SetMonth(int month) {
		return w_datetime_set_month(W_DATETIME(this), month) > 0;
	}
	/**
	 * Sets the receiver's seconds.
	 * <p>
	 * Seconds is an integer between 0 and 59.
	 * </p>
	 *
	 * @param seconds an integer between 0 and 59
	 */
	bool SetSeconds(int seconds) {
		return w_datetime_set_seconds(W_DATETIME(this), seconds) > 0;
	}
	/**
	 * Sets the receiver's hours, minutes, and seconds in a single operation.
	 *
	 * @param hours an integer between 0 and 23
	 * @param minutes an integer between 0 and 59
	 * @param seconds an integer between 0 and 59
	 */
	bool SetTime(int hours, int minutes, int seconds) {
		return w_datetime_set_time(W_DATETIME(this), hours, minutes, seconds)
				> 0;
	}
	/**
	 * Sets the receiver's year.
	 * <p>
	 * The first year is 1752 and the last year is 9999.
	 * If the specified year is not valid for the receiver's day and month, then it is ignored.
	 * </p>
	 *
	 * @param year an integer between 1752 and 9999
	 *
	 * @see #setDate
	 */
	bool SetYear(int year) {
		return w_datetime_set_year(W_DATETIME(this), year) > 0;
	}
protected:
	w_class_id _GetClassID();
private:
	void *handles[(sizeof(w_datetime) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_DATETIME_H_ */
