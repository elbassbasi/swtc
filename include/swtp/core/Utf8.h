/*
 * Utf8.h
 *
 *  Created on: 2 oct. 2020
 *      Author: azeddine
 */

#ifndef SWTP_CORE_UTF8_H_
#define SWTP_CORE_UTF8_H_
#include "Resource.h"
#include <string>
SWTP_PUBLIC size_t w_alloc_std_string(void *user_data, size_t size, void **string);
SWTP_PUBLIC size_t w_alloc_new(void *user_data, size_t size, void **ptr);
#endif /* SWTP_CORE_UTF8_H_ */
