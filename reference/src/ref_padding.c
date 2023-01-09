#include <ref_padding.h>
#include <string.h>


int ref_qs_padding(uint8_t *input, uint64_t input_len, uint8_t *padded_input, uint64_t block_size)
{
    memset(padded_input, 0, block_size);
    memcpy(padded_input, input, input_len);

    padded_input[input_len]         = 0x80;
    if(block_size - input_len == 1)
    {
        padded_input[(2 * block_size) - 1] = 0x01;
        return 2;
    }
    else
    {
        padded_input[block_size - 1]    = 0x01;
        return 1;
    }
}