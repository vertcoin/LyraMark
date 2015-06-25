#ifndef LYRA2RE_H
#define LYRA2RE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void lyra2re_hash(const char* input, char* output, uint64_t timeCost, uint64_t nRows, uint64_t nCols);

#ifdef __cplusplus
}
#endif

#endif
