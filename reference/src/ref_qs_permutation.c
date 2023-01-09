#include <ref_qs_permutation.h>
#include <ref_aes_ni.h>
#include <string.h>

void ref_xor_f(uint8_t *dst, uint8_t *src, uint64_t len)
{
    while(len--)
    {
        dst[len] ^= src[len];
    }
}

void ref_shift_rows(ref_qs_state *state)
{
    // 0-th row is fixed

    // 1-st row
    ref_u64x2 tmp = state->u128_square_state[1][0];
    state->u128_square_state[1][0] = state->u128_square_state[1][1];
    state->u128_square_state[1][1] = state->u128_square_state[1][2];
    state->u128_square_state[1][2] = state->u128_square_state[1][3];
    state->u128_square_state[1][3] = state->u128_square_state[1][4];
    state->u128_square_state[1][4] = state->u128_square_state[1][5];
    state->u128_square_state[1][5] = tmp;

    // 2-nd row
    tmp = state->u128_square_state[2][0];
    state->u128_square_state[2][0] = state->u128_square_state[2][2];
    state->u128_square_state[2][2] = state->u128_square_state[2][4];
    state->u128_square_state[2][4] = tmp;

    tmp = state->u128_square_state[2][1];
    state->u128_square_state[2][1] = state->u128_square_state[2][3];
    state->u128_square_state[2][3] = state->u128_square_state[2][5];
    state->u128_square_state[2][5] = tmp;

    // 3-rd row
    tmp = state->u128_square_state[3][0];
    state->u128_square_state[3][0] = state->u128_square_state[3][3];
    state->u128_square_state[3][3] = tmp;

    tmp = state->u128_square_state[3][1];
    state->u128_square_state[3][1] = state->u128_square_state[3][4];
    state->u128_square_state[3][4] = tmp;

    tmp = state->u128_square_state[3][2];
    state->u128_square_state[3][2] = state->u128_square_state[3][5];
    state->u128_square_state[3][5] = tmp;

    // 4-th row
    tmp = state->u128_square_state[4][0];
    state->u128_square_state[4][0] = state->u128_square_state[4][4];
    state->u128_square_state[4][4] = state->u128_square_state[4][2];
    state->u128_square_state[4][2] = tmp;

    tmp = state->u128_square_state[4][1];
    state->u128_square_state[4][1] = state->u128_square_state[4][5];
    state->u128_square_state[4][5] = state->u128_square_state[4][3];
    state->u128_square_state[4][3] = tmp;

    // 5-th row
    tmp = state->u128_square_state[5][0];
    state->u128_square_state[5][0] = state->u128_square_state[5][5];
    state->u128_square_state[5][5] = state->u128_square_state[5][4];
    state->u128_square_state[5][4] = state->u128_square_state[5][3];
    state->u128_square_state[5][3] = state->u128_square_state[5][2];
    state->u128_square_state[5][2] = state->u128_square_state[5][1];
    state->u128_square_state[5][1] = tmp;
}

void ref_add_round_constants_and_sub_byte(ref_qs_state *state, int rn)
{
    int rn_idx = rn * REF_QS_SQUARE_STATE_DIM * REF_QS_SQUARE_STATE_DIM;

    for(int i = 0; i < REF_QS_SQUARE_STATE_DIM; i++)
    {
        int i_idx = i * REF_QS_SQUARE_STATE_DIM;
        
        for(int j = 0; j < REF_QS_SQUARE_STATE_DIM; j++)
        {
            state->u128_square_state[i][j] ^= rcons[rn_idx + i_idx + j].rcon128;
            ref_aes128_enc(&(state->m128_square_state[i][j]));
        }
    }
}

// DEFIN GF(2^128)
#define REF_IRR 0x87 // Irreducible polynomial

#define REF_GF128_MUL1_XOR(in, out) {\
   out.HIGH ^= in.HIGH;\
   out.LOW  ^= in.LOW;\
}

#define REF_GF128_MUL2_XOR(in, out) {\
  out.HIGH ^= (in.HIGH<<1) ^ (in.LOW>>63) ;\
  out.LOW ^= (in.LOW<<1) ^ (in.HIGH>>63) * REF_IRR;\
}

#define REF_GF128_MUL3_XOR(in, out) {\
  out.HIGH ^= (in.HIGH<<1) ^ (in.LOW>>63) ^ in.HIGH;\
  out.LOW ^= (in.LOW<<1) ^ (in.HIGH>>63) * REF_IRR ^ in.LOW;\
}


#define REF_GF128_MUL8_XOR(in, out) {\
  out.HIGH ^= (in.HIGH<<3) ^ (in.LOW>>61);\
  out.LOW ^= (in.LOW<<3)\
    ^ (REF_IRR * ((in.HIGH>>61)&0x04))\
    ^ (REF_IRR * ((in.HIGH>>61)&0x02))\
    ^ (REF_IRR * ((in.HIGH>>61)&0x01));\
}

void ref_mix_column(ref_qs_state *state)
{
    ref_qs_state tmp_state = {0};
    
    
    for(int j = 0; j < REF_QS_SQUARE_STATE_DIM; j++)
    {
        REF_GF128_MUL1_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[0][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[0][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[0][j]);
        REF_GF128_MUL3_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[0][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[0][j]);
        REF_GF128_MUL8_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[0][j]);

        REF_GF128_MUL8_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[1][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[1][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[1][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[1][j]);
        REF_GF128_MUL3_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[1][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[1][j]);
        
        REF_GF128_MUL2_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[2][j]);
        REF_GF128_MUL8_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[2][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[2][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[2][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[2][j]);
        REF_GF128_MUL3_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[2][j]);
        
        REF_GF128_MUL3_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[3][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[3][j]);
        REF_GF128_MUL8_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[3][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[3][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[3][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[3][j]);

        REF_GF128_MUL2_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[4][j]);
        REF_GF128_MUL3_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[4][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[4][j]);
        REF_GF128_MUL8_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[4][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[4][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[4][j]);

        REF_GF128_MUL1_XOR(state->u128_LH_state[0][j], tmp_state.u128_LH_state[5][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[1][j], tmp_state.u128_LH_state[5][j]);
        REF_GF128_MUL3_XOR(state->u128_LH_state[2][j], tmp_state.u128_LH_state[5][j]);
        REF_GF128_MUL2_XOR(state->u128_LH_state[3][j], tmp_state.u128_LH_state[5][j]);
        REF_GF128_MUL8_XOR(state->u128_LH_state[4][j], tmp_state.u128_LH_state[5][j]);
        REF_GF128_MUL1_XOR(state->u128_LH_state[5][j], tmp_state.u128_LH_state[5][j]);
    }
    

    memcpy(state->u8_state, tmp_state.u8_state, REF_QS_STATE_SIZE);
}

void ref_permute(ref_qs_state *state)
{
    for(int rn = 0; rn < REF_QS_PERM_ROUNDS; rn++)
    {
        // Add Round Constants And Sub Byte
        ref_add_round_constants_and_sub_byte(state, rn);

        // Shift Rows
        ref_shift_rows(state);

        // Mix Columns
        ref_mix_column(state);

    }
}

void ref_absorb(uint8_t *input_block)
{

    // Xor input_block with bitrate section of internal state
    ref_xor_f(ref_internal_state.state.u8_state, input_block, REF_QS_BITRATE_SIZE);

    // perform permutation function
    ref_permute(&ref_internal_state.state);

}

void ref_squeeze(uint8_t *output, uint64_t output_len)
{
    // perform one permutation
    memcpy(output, ref_internal_state.state.u8_state, output_len);
}