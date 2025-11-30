#ifndef _BIGSIZE_
#define _BIGSIZE_

#include<stdint.h>
#include<stddef.h>

uint8_t read_bigsize(uint8_t const* const buf, const size_t maxlength, uint64_t* const value);
uint8_t write_bigsize(const uint64_t value, uint8_t* const buf); 

#endif
