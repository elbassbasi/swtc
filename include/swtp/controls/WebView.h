/*
 * Browser.h
 *
 *  Created on: 12 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_WEBVIEW_H_
#define SWTP_CONTROLS_WEBVIEW_H_
#include "../widgets/Composite.h"
/**
 * Instances of this class implement the browser user interface
 * metaphor.  It allows the user to visualize and navigate through
 * HTML documents.
 * <p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not make sense to set a layout on it.
 * </p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>MOZILLA, WEBKIT</dd>
 * <dt><b>Events:</b></dt>
 * <dd>CloseWindowListener, LocationListener, OpenWindowListener, ProgressListener, StatusTextListener, TitleListener, VisibilityWindowListener</dd>
 * </dl>
 * <p>
 * Note: At most one of the styles MOZILLA and WEBKIT may be specified.
 * </p>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WWebView: public WComposite {
public:
	WWebView() {
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
	 * @param parent a widget which will be the parent of the new instance (cannot be null)
	 * @param style the style of widget to construct
	 *
	 * @see Widget#getStyle
	 */
	WWebView(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Navigate to the previous session history item.
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see #forward
	 */
	bool Back() {
		return _WReturnBool(_back());
	}
	/**
	 * Executes the specified script.
	 * <p>
	 * Executes a script containing javascript commands in the context of the current document.
	 * If document-defined functions or properties are accessed by the script then this method
	 * should not be invoked until the document has finished loading (<code>ProgressListener.completed()</code>
	 * gives notification of this).
	 *
	 * @param script the script with javascript commands
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see ProgressListener#completed(ProgressEvent)
	 */
	bool Execute(const char *script) {
		return _WReturnBool(_execute(script, -1, W_ENCODING_UTF8));
	}
	/**
	 * Attempts to dispose the receiver, but allows the dispose to be vetoed
	 * by the user in response to an <code>onbeforeunload</code> listener
	 * in the Browser's current page.
	 *
	 * @return <code>true</code> if the receiver was disposed, and <code>false</code> otherwise
	 *
	 * @see #dispose()
	 */
	bool Close() {
		return _WReturnBool(_close());
	}
	/**
	 * Returns the result, if any, of executing the specified script.
	 * <p>
	 * Evaluates a script containing javascript commands in the context of
	 * the current document.  If document-defined functions or properties
	 * are accessed by the script then this method should not be invoked
	 * until the document has finished loading (<code>ProgressListener.completed()</code>
	 * gives notification of this).
	 * </p><p>
	 * If the script returns a value with a supported type then a java
	 * representation of the value is returned.  The supported
	 * javascript -> java mappings are:
	 * <ul>
	 * <li>javascript null or undefined -> <code>null</code></li>
	 * <li>javascript number -> <code>java.lang.Double</code></li>
	 * <li>javascript string -> <code>java.lang.String</code></li>
	 * <li>javascript bool -> <code>java.lang.bool</code></li>
	 * <li>javascript array whose elements are all of supported types -> <code>java.lang.Object[]</code></li>
	 * </ul>
	 *
	 * An <code>SWTException</code> is thrown if the return value has an
	 * unsupported type, or if evaluating the script causes a javascript
	 * error to be thrown.
	 *
	 * @param script the script with javascript commands
	 *
	 * @return the return value, if any, of executing the script
	 *
	 * @see ProgressListener#completed(ProgressEvent)
	 */
	bool Evaluate(const char *script, WValue &result) {
		return _WReturnBool(_evaluate(script, -1, W_ENCODING_UTF8, &result));
	}
	/**
	 * Navigate to the next session history item.
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise
	 *
	 * @see #back
	 */
	bool Forward() {
		return _WReturnBool(_forward());
	}
	/**
	 * Returns the type of native browser being used by this instance.
	 * Examples: "ie", "mozilla", "voyager", "webkit"
	 *
	 * @return the type of the native browser
	 */
	const char* GetBrowserType() {
		char *type;
		_get_browser_type(&type);
		return type;
	}
	/**
	 * Returns <code>true</code> if javascript will be allowed to run in pages
	 * subsequently viewed in the receiver, and <code>false</code> otherwise.
	 *
	 * @return the receiver's javascript enabled state
	 *
	 * @see #setJavascriptEnabled
	 */
	bool GetJavascriptEnabled() {
		return _WReturnBool(_get_javascript_enabled());
	}
	/**
	 * Returns a string with HTML that represents the content of the current page.
	 *
	 * @return HTML representing the current page or an empty <code>String</code>
	 * if this is empty
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		_get_text(w_alloc_string_ref, &ref, W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns the current URL.
	 *
	 * @return the current URL or an empty <code>String</code> if there is no current URL
	 *
	 * @see #setUrl
	 */
	WString GetUrl() {
		w_string_ref *ref = 0;
		_get_url(w_alloc_string_ref, &ref, W_ENCODING_UTF8);
		return ref;
	}
	/**
	 * Returns <code>true</code> if the receiver can navigate to the
	 * previous session history item, and <code>false</code> otherwise.
	 *
	 * @return the receiver's back command enabled state
	 *
	 * @see #back
	 */
	bool IsBackEnabled() {
		return _WReturnBool(_is_back_enabled());

	}
	/**
	 * Returns <code>true</code> if the receiver can navigate to the
	 * next session history item, and <code>false</code> otherwise.
	 *
	 * @return the receiver's forward command enabled state
	 *
	 * @see #forward
	 */
	bool IsForwardEnabled() {
		return _WReturnBool(_is_forward_enabled());
	}
	/**
	 * Refresh the current page.
	 */
	bool Refresh() {
		return _WReturnBool(_refresh());

	}
	/**
	 * Sets whether javascript will be allowed to run in pages subsequently
	 * viewed in the receiver.  Note that setting this value does not affect
	 * the running of javascript in the current page.
	 *
	 * @param enabled the receiver's new javascript enabled state
	 */
	bool SetJavascriptEnabled(bool enabled) {
		return _WReturnBool(_set_javascript_enabled(enabled));
	}
	/**
	 * Renders a string containing HTML.  The rendering of the content occurs asynchronously.
	 * The rendered page will be given trusted permissions; to render the page with untrusted
	 * permissions use <code>setText(String html, bool trusted)</code> instead.
	 * <p>
	 * The html parameter is Unicode-encoded since it is a java <code>String</code>.
	 * As a result, the HTML meta tag charset should not be set. The charset is implied
	 * by the <code>String</code> itself.
	 *
	 * @param html the HTML content to be rendered
	 *
	 * @return true if the operation was successful and false otherwise.
	 *
	 * @see #setText(String,bool)
	 * @see #setUrl
	 */
	bool SetText(const char *html) {
		return SetText(html, true);
	}
	/**
	 * Renders a string containing HTML.  The rendering of the content occurs asynchronously.
	 * The rendered page can be given either trusted or untrusted permissions.
	 * <p>
	 * The <code>html</code> parameter is Unicode-encoded since it is a java <code>String</code>.
	 * As a result, the HTML meta tag charset should not be set. The charset is implied
	 * by the <code>String</code> itself.
	 * <p>
	 * The <code>trusted</code> parameter affects the permissions that will be granted to the rendered
	 * page.  Specifying <code>true</code> for trusted gives the page permissions equivalent
	 * to a page on the local file system, while specifying <code>false</code> for trusted
	 * gives the page permissions equivalent to a page from the internet.  Page content should
	 * be specified as trusted if the invoker created it or trusts its source, since this would
	 * allow (for instance) style sheets on the local file system to be referenced.  Page
	 * content should be specified as untrusted if its source is not trusted or is not known.
	 *
	 * @param html the HTML content to be rendered
	 * @param trusted <code>false</code> if the rendered page should be granted restricted
	 * permissions and <code>true</code> otherwise
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise.
	 *
	 * @see #setText(String)
	 * @see #setUrl
	 */
	bool SetText(const char *html, bool trusted) {
		return _WReturnBool(_set_text(html, -1, W_ENCODING_UTF8, trusted));

	}
	/**
	 * Begins loading a URL.  The loading of its content occurs asynchronously.
	 *
	 * @param url the URL to be loaded
	 *
	 * @return true if the operation was successful and false otherwise.
	 *
	 * @see #getUrl
	 * @see #setUrl(String,String,String[])
	 */
	bool SetUrl(const char *url) {
		return SetUrl(url, 0, 0);
	}
	/**
	 * Begins loading a URL.  The loading of its content occurs asynchronously.
	 * <p>
	 * If the URL causes an HTTP request to be initiated then the provided
	 * <code>postData</code> and <code>header</code> arguments, if any, are
	 * sent with the request.  A value in the <code>headers</code> argument
	 * must be a name-value pair with a colon separator in order to be sent
	 * (for example: "<code>user-agent: custom</code>").
	 *
	 * @param url the URL to be loaded
	 * @param postData post data to be sent with the request, or <code>null</code>
	 * @param headers header lines to be sent with the request, or <code>null</code>
	 *
	 * @return <code>true</code> if the operation was successful and <code>false</code> otherwise.
	 */
	bool SetUrl(const char *url, const char *postData, const char *headers) {
		return _WReturnBool(
				_set_url(url, -1, W_ENCODING_UTF8, postData, headers));
	}
	/**
	 * Stop any loading and rendering activity.
	 */
	bool Stop() {
		return _WReturnBool(_stop());
	}
protected:
	w_class_id _GetClassID();
public:
	WResult _back() {
		return w_webview_back(W_WEBVIEW(this));
	}
	WResult _execute(const char *script, int length, int enc) {
		return w_webview_execute(W_WEBVIEW(this), script, length, enc);
	}
	WResult _close() {
		return w_webview_close(W_WEBVIEW(this));
	}
	WResult _evaluate(const char *script, int length, int enc, WValue *result) {
		return w_webview_evaluate(W_WEBVIEW(this), script, length, enc, result);
	}
	WResult _forward() {
		return w_webview_forward(W_WEBVIEW(this));
	}
	WResult _get_browser_type(char **type) {
		return w_webview_get_webview_type(W_WEBVIEW(this), type);
	}
	WResult _get_javascript_enabled() {
		return w_webview_get_javascript_enabled(W_WEBVIEW(this));
	}
	WResult _get_text(w_alloc alloc, void *user_data, int enc) {
		return w_webview_get_text(W_WEBVIEW(this), alloc, user_data, enc);
	}
	WResult _get_url(w_alloc alloc, void *user_data, int enc) {
		return w_webview_get_url(W_WEBVIEW(this), alloc, user_data, enc);
	}
	WResult _is_back_enabled() {
		return w_webview_is_back_enabled(W_WEBVIEW(this));
	}
	WResult _is_forward_enabled() {
		return w_webview_is_forward_enabled(W_WEBVIEW(this));
	}
	WResult _refresh() {
		return w_webview_refresh(W_WEBVIEW(this));
	}
	WResult _set_javascript_enabled(int enabled) {
		return w_webview_set_javascript_enabled(W_WEBVIEW(this), enabled);
	}
	WResult _set_text(const char *html, int length, int enc, int trusted) {
		return w_webview_set_text(W_WEBVIEW(this), html, length, enc, trusted);
	}
	WResult _set_url(const char *url, int length, int enc, const char *postData,
			const char *headers) {
		return w_webview_set_url(W_WEBVIEW(this), url, length, enc, postData,
				headers);
	}
	WResult _stop() {
		return w_webview_stop(W_WEBVIEW(this));
	}
private:
	void *handles[(sizeof(w_webview) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_WEBVIEW_H_ */
