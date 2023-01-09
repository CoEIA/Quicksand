// REF_QuickSand API v1.0

#ifndef __REF_QUICKSAND__H
#define __REF_QUICKSAND__H

#include <stdint.h>

// Error Messages
#define REF_ERR_OUTPUT_LEN      -1
#define REF_ERR_INPUT_LEN       -2
#define REF_ERR_NOT_INITIALIZED -3

// Success Messages
#define REF_SUCCESS 1


// Init function
int ref_qs_init(void);

// Update (Absorb) function
int ref_qs_update(uint8_t *input, uint64_t input_len);

// return digest with size 128 Bytes
int ref_qs_digest(uint8_t *output);

//return digest with specific output size less than or equal to 128 Bytes
int ref_qs_digest_n(uint8_t *output, uint64_t output_len);

// performs update and digest in one call
int ref_qs_update_digest(uint8_t *input, uint64_t input_len, uint8_t *output);

// Cleanup (Close or Deconstruct) function
void ref_qs_cleanup();



#endif