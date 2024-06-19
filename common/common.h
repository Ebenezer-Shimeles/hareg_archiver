#pragma once

#define __DEBUG true

#ifdef __DEBUG
#define DEBUG_CALL(X) X
#else
#define DEBUG_CALL(X)
#endif

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

