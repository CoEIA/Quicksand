#include <ref_quicksand.h>
#include <ref_qs_permutation.h>
#include <ref_padding.h>
#include <ref_aes_ni.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




int ref_qs_init(void)
{
    // initialize the state of quicksand
    memset(&ref_internal_state, 0, sizeof(ref_qs_ctx));
    ref_internal_state.is_initialized = true;

    // set AES Key 
    ref_aes128_load_key(ref_key);

    return REF_SUCCESS;
}

int ref_get_remaining_bytes_len(int internal_idx, int input_len)
{
    int result = REF_QS_BITRATE_SIZE - internal_idx;
    
    return (input_len < result) ? input_len : result;
}

int ref_qs_update(uint8_t *input, uint64_t input_len)
{
    // check if internal state is initialized
    if(!ref_internal_state.is_initialized)
        return REF_ERR_NOT_INITIALIZED;
    
    // check input_len
    if(input_len == 0)
        return REF_ERR_INPUT_LEN;
    // Append internal buffer, (if become complete block) process it with absorb
    int remaining_bytes_len = ref_get_remaining_bytes_len(ref_internal_state.internal_buffer_idx, input_len);
    memcpy( ref_internal_state.internal_buffer + ref_internal_state.internal_buffer_idx,
            input,
            remaining_bytes_len
    );

    ref_internal_state.internal_buffer_idx += remaining_bytes_len;

    if(ref_internal_state.internal_buffer_idx == REF_QS_BITRATE_SIZE)
    {
        ref_absorb(ref_internal_state.internal_buffer);
        ref_internal_state.internal_buffer_idx = 0;
    }

    input_len -= remaining_bytes_len;
    input += remaining_bytes_len;

    // Process Complete Blocks of input data (ignore additional bytes that doesn't fit into one block)
    uint64_t input_blocks = (input_len / REF_QS_BITRATE_SIZE);
    
    for(int i = 0; i < input_blocks; i++)
    {
        ref_absorb(input);
        input += REF_QS_BITRATE_SIZE;
    }

    // if input_len is not multiple of QS_BITRATE_SIZE, 
    // last block has not been absorbed and it needs to be stored in internal buffer
    if(input_len % REF_QS_BITRATE_SIZE)
    {
        ref_internal_state.internal_buffer_idx = input_len % REF_QS_BITRATE_SIZE;
        memcpy(ref_internal_state.internal_buffer, input, ref_internal_state.internal_buffer_idx);
    }

    return REF_SUCCESS;
}

int ref_qs_update_digest(uint8_t *input, uint64_t input_len, uint8_t *output)
{
    int ret;
    ret = ref_qs_update(input, input_len);
    if(ret != REF_SUCCESS)
        return ret;

    ret = ref_qs_digest(output);
    if(ret != REF_SUCCESS)
        return ret;

    return REF_SUCCESS;
}

int ref_qs_digest(uint8_t *output)
{
    return ref_qs_digest_n(output, REF_QS_DIGEST_SIZE);
}

int ref_qs_digest_n(uint8_t *output, uint64_t output_len)
{
    if(output_len > REF_QS_DIGEST_SIZE)
        return REF_ERR_OUTPUT_LEN;

    uint8_t padded_input[2 * REF_QS_BITRATE_SIZE] = {0};
        
    int nb = ref_qs_padding(
        ref_internal_state.internal_buffer, 
        ref_internal_state.internal_buffer_idx, 
        padded_input, 
        REF_QS_BITRATE_SIZE);
    
    for(int i = 0; i < nb; i++)
        ref_absorb(padded_input + (i * REF_QS_BITRATE_SIZE));
    ref_internal_state.internal_buffer_idx = 0;

    ref_squeeze(output, output_len);
    
    return REF_SUCCESS;
}

void ref_qs_cleanup()
{
    ref_internal_state.is_initialized = false;
    memset(&ref_internal_state, 0, sizeof(ref_qs_ctx));
}