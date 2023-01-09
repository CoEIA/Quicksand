#ifndef __PADDING_H
#define __PADDING_H

#include <stdint.h>
/**
 * @brief This is Padding fucntion for Quicksand.
 * 
 * @param input Input before padding
 * @param input_len Length of the input.
 * @param padded_input  Output after padding.
 * @param block_size The block size.
 * @return int 
 */
int qs_padding(uint8_t *input, uint64_t input_len, uint8_t *padded_input, uint64_t block_size);

#endif