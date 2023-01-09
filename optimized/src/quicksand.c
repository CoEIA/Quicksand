#include <quicksand.h>
#include <qs_permutation.h>
#include <padding.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#ifdef AESNI
    #include <aes_ni.h>
#elif AESARM
    #include <aes_arm.h>
#else
    #include <aes.h>
#endif


/**
 * @brief This is the initialization function. Must be invoked at first.
 it sets the internal state with bitrate and capacity to zero.
 * 
 * @return QS_CTX QuickSand context
 */
QS_CTX qs_init()
{   
    qs_ctx_st *internal_state = (qs_ctx_st*)calloc(1,sizeof(qs_ctx_st));

    // If the calloc fail the return will be NULL
    if(internal_state == NULL)
        return NULL;
    // initialize the state of quicksand
    internal_state->is_initialized = true;

    // set AES Key 
#if !defined(AESNI) && !defined(AESARM)
    AES_init_ctx();
#endif
    
    return (QS_CTX) internal_state;
}

/**
 * @brief Get the remaining bytes len object
 * an internal function that supports the calculation of Padding size
 *
 * @param internal_idx 
 * @param input_len 
 * @return uint32_t 
 */
uint32_t get_remaining_bytes_len(int internal_idx, uint64_t input_len)
{
    uint32_t result = QS_BITRATE_SIZE - internal_idx;
    
    return (input_len < result) ? input_len : result;
}


/**
 * @brief QuickSand Update function.
 * 
 * @param ctx QuickSand context
 * @param input new input string that needes to be updated to the digest
 * @param input_len the size of the new input string
 * @return int Flag for Success (1) or other flags (check Error messages section in quicksand.h)
 */
int qs_update(QS_CTX ctx, uint8_t *input, uint64_t input_len)
{
    qs_ctx_st *internal_state = (qs_ctx_st *) ctx;
    // check if internal state is initialized
    if(!internal_state->is_initialized)
        return ERR_NOT_INITIALIZED;
    
    // check input_len
    if(input_len == 0)
        return ERR_INPUT_LEN;
    
    // Append internal buffer, (if become complete block) process it with absorb
    int remaining_bytes_len = get_remaining_bytes_len(internal_state->internal_buffer_idx, input_len);
    memcpy( internal_state->internal_buffer + internal_state->internal_buffer_idx,
            input,
            remaining_bytes_len
    );

    internal_state->internal_buffer_idx += remaining_bytes_len;

    if(internal_state->internal_buffer_idx == QS_BITRATE_SIZE)
    {
        absorb(internal_state, internal_state->internal_buffer);
        internal_state->internal_buffer_idx = 0;
    }

    input_len -= remaining_bytes_len;
    input += remaining_bytes_len;

    // Process Complete Blocks of input data (ignore additional bytes that doesn't fit into one block)
    uint64_t input_blocks = (input_len / QS_BITRATE_SIZE);
    
    for(uint64_t i = 0; i < input_blocks; i++)
    {
        absorb(internal_state, input);
        input += QS_BITRATE_SIZE;
    }

    // if input_len is not multiple of QS_BITRATE_SIZE, 
    // last block has not been absorbed and it needs to be stored in internal buffer
    if(input_len % QS_BITRATE_SIZE)
    {
        internal_state->internal_buffer_idx = input_len % QS_BITRATE_SIZE;
        memcpy(internal_state->internal_buffer, input, internal_state->internal_buffer_idx);
    }

    return SUCCESS;
}

/**
 * @brief An alternative function for a single call to update and digest.
 * 
 * @param ctx QuickSand context
 * @param input string 
 * @param input_len size of input string
 * @param output the digest
 * @return int Flag for Success (1) or other flags (check Error messages section in quicksand.h)
 */
int qs_update_digest(QS_CTX ctx, uint8_t *input, uint64_t input_len, uint8_t *output)
{
    int ret;
    ret = qs_update(ctx, input, input_len);
    if(ret != SUCCESS)
        return ret;

    ret = qs_digest(ctx, output);
    if(ret != SUCCESS)
        return ret;

    return SUCCESS;
}

/**
 * @brief QuickSand digest function. a shortcut to call digest_n with a fixed output size
 * 
 * @param ctx QuickSand context
 * @param output the digest of fixed size
 * @return int 
 */
int qs_digest(QS_CTX ctx, uint8_t *output)
{
    return qs_digest_n(ctx, output, QS_DIGEST_SIZE);
}

/**
 * @brief QuickSand digest function.
 * 
 * @param ctx QuickSand context
 * @param output the digest of fixed size
 * @param output_len 
 * @return int 
 */
int qs_digest_n(QS_CTX ctx, uint8_t *output, uint64_t output_len)
{
    qs_ctx_st *internal_state = (qs_ctx_st *) ctx;
    if(output_len > QS_DIGEST_SIZE)
        return ERR_OUTPUT_LEN;

    uint8_t padded_input[2 * QS_BITRATE_SIZE] = {0};
        
    int nb = qs_padding(
        internal_state->internal_buffer, 
        internal_state->internal_buffer_idx, 
        padded_input, 
        QS_BITRATE_SIZE);
    
    for(int i = 0; i < nb; i++)
        absorb(internal_state, padded_input + (i * QS_BITRATE_SIZE));
    internal_state->internal_buffer_idx = 0;

    squeeze(internal_state, output, output_len);
    return SUCCESS;
}
/**
 * @brief Memory Free (Close or Deconstruct)  of the QuickSand context
 * 
 * @param ctx Quicksand context
 */
void qs_cleanup(QS_CTX ctx)
{
    memset(ctx, 0, sizeof(qs_ctx_st));
    free(ctx);
}