#ifndef _DIGIT_H_
#define _DIGIT_H_

enum px_digit_size{
    PX_DIGIT_SIZE_5x3,
};

SYMBOL_EXPORT uint8_t** px_digit_get(enum px_digit_size size, char number);

#endif

