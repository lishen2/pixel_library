#ifndef _PX_COMMON_H_
#define _PX_COMMON_H_

#define RET_OK     0
#define RET_FAILED 1
#define RET_ERROR  RET_FAILED

#ifdef _WIN32
#define SYMBOL_EXPORT __declspec(dllexport)
#else
#define SYMBOL_EXPORT
#endif

#define ASSERT(exp)\
    if (!(exp)){\
        px_printf("ASSERT: %s:%d\r\n", __FUNCTION__, __LINE__);\
    }

#endif

