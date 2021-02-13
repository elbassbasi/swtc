/*
 * Resource.h
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CORE_RESOURCE_H_
#define SWTP_CORE_RESOURCE_H_
#include "../../swtc.h"
#include <exception>
#include <initializer_list>
#if __cplusplus >= 201103L
#include <functional>
#endif
#ifdef SWTPBUILD
#define SWTP_PUBLIC DLL_EXPORT
#else
#define SWTP_PUBLIC //DLL_IMPORT
#endif
typedef wresult WResult;
class SWTP_PUBLIC WResource {
private:
	WResource(const WResource &resource) {

	}
	WResource& operator =(const WResource &resource) {
		return *this;
	}
public:
	WResource() {
	}
};
inline int _WReturnInt(WResult result) {
	return result >= 0 ? result : 0;
}
inline int _WReturnIntIndex(WResult result) {
	return result >= 0 ? result : -1;
}
inline bool _WReturnBool(WResult result) {
	return result > 0;
}
class WException: public std::exception {
public:
	WException(WResult result) {
		this->result = result;
	}
	WResult result;
};
SWTP_PUBLIC int _WReturnIntExpt(WResult result);
SWTP_PUBLIC int _WReturnIntIndexExpt(WResult result);
SWTP_PUBLIC bool _WReturnBoolExpt(WResult result);

#endif /* SWTP_CORE_RESOURCE_H_ */
