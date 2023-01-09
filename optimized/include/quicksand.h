// QuickSand API v1.0

#ifndef __QUICKSAND__H
#define __QUICKSAND__H

#include <stdint.h>

/**
 * @brief Error Messages
 * 
 */
#define ERR_OUTPUT_LEN      -1  // There is an error in the Output length
#define ERR_INPUT_LEN       -2  // There is an error in the Input length
#define ERR_NOT_INITIALIZED -3  // The quicksand initialization failed

/**
 * @brief Success Messages
 * 
 */
#define SUCCESS 1
/**
 * @brief void pointer
 * 
 */
typedef void* QS_CTX;

/**
 * @brief This is the initialization function. Must be invoked at first.
 it sets the internal state with bitrate and capacity to zero.
 * 
 * @return QS_CTX QuickSand context
 */
QS_CTX qs_init();

// Update (Absorb) function
int qs_update(QS_CTX ctx, uint8_t *input, uint64_t input_len);

/**
 * @brief Return the digest with size 128 Bytes
 * 
 * @param ctx QuickSand context
 * @param output 
 * @return int 
 */
int qs_digest(QS_CTX ctx, uint8_t *output);

/**
 * @brief Return the digest with specific output size less than or equal to 128 Bytes
 * 
 * @param ctx QuickSand context
 * @param output 
 * @param output_len 
 * @return int 
 */
int qs_digest_n(QS_CTX ctx, uint8_t *output, uint64_t output_len);

/**
 * @brief Performs update and digest in one call
 * 
 * @param ctx QuickSand context
 * @param input 
 * @param input_len 
 * @param output 
 * @return int 
 */
int qs_update_digest(QS_CTX ctx, uint8_t *input, uint64_t input_len, uint8_t *output);

/**
 * @brief Memory Free (Close or Deconstruct)  of the QuickSand context
 * 
 * @param ctx QuickSand context
 */
void qs_cleanup(QS_CTX ctx);
#endif