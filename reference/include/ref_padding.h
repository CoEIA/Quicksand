#ifndef __REF_PADDING_H
#define __REF_PADDING_H

#include <stdint.h>

int ref_qs_padding(uint8_t *input, uint64_t input_len, uint8_t *padded_input, uint64_t block_size);

#endif