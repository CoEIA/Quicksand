#include <qs_permutation.h>
#include <string.h>

#ifdef AESNI
    #include <aes_ni.h>
#elif AESARM
    #include <aes_arm.h>
#else
    #include <aes.h>
#endif

/**
 * @brief XOR function
 * 
 * @param dst 
 * @param src 
 * @param len 
 */
void xor_f(u64x2 *dst, u64x2 *src, uint64_t len)
{  
    // Try intel intrinsics
    for(int i =0; i< 20;i++){
        dst[i] ^= src[i]; 
    }
}

/**
 * @brief This Shift Rows Function it take the stat and shift its rows.
 * 
 * @param state Quicksand State
 */
void shift_rows(qs_state *state)
{  
    // 0-th row is fixed

    // 1-st row
    u64x2 tmp = state->u128_square_state[1][0];
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

#ifdef AESNI
    #define AES_ENC(state, i, j)\
        aes128_enc(state->m128_square_state[i]);
#elif AESARM
    #define AES_ENC(state, i, j)\
        arm_aes128_enc(state->u8_square_state[i]);
#else
    #define AES_ENC(state, i, j)\
        AES_ECB_encrypt((state->u8_square_state[i][j]));
#endif

/**
 * @brief This function will add the round constants and also sub byte.
 * 
 * @param state Quicksand state
 * @param rcon_ptr 
 */
void add_round_constants_and_sub_byte(qs_state *state, const qs_rcons *rcon_ptr)
{
    int idx = 0;
    
    for(int i = 0; i < QS_SQUARE_STATE_DIM; i++)
    {   
        state->u128_square_state[i][0] ^= rcon_ptr[idx++].rcon128;
        state->u128_square_state[i][1] ^= rcon_ptr[idx++].rcon128;
        state->u128_square_state[i][2] ^= rcon_ptr[idx++].rcon128;
        state->u128_square_state[i][3] ^= rcon_ptr[idx++].rcon128;
        state->u128_square_state[i][4] ^= rcon_ptr[idx++].rcon128;
        state->u128_square_state[i][5] ^= rcon_ptr[idx++].rcon128;

        AES_ENC(state, i, QS_SQUARE_STATE_DIM);
    }
}

// DEFIN GF(2^128)

#define GF128_XOR(in, out)\
   out.HIGH ^= in.HIGH; \
   out.LOW  ^= in.LOW;

#define GF128_MUL1(in, out)\
   out.HIGH = in.HIGH;\
   out.LOW  = in.LOW;

#define GF128_MUL2(in, out)\
  out.HIGH = (in.HIGH<<1) ^ (in.LOW>>63) ;\
  out.LOW = (in.LOW<<1) ^ (in.HIGH>>63) * IRR;

#define GF128_MUL2_XOR_12(in, out_l, out_h)\
    out_h = (in.HIGH<<1) ^ (in.LOW>>63);\
    out_l = (in.LOW<<1) ^ (in.HIGH>>63) * IRR;

#define GF128_MUL2_XOR_22(in_l, in_h, out_l, out_h)\
    out_h = (in_h<<1) ^ (in_l>>63);\
    out_l = (in_l<<1) ^ (in_h>>63) * IRR;

#define GF128_MUL2_21(in_l, in_h, out)\
    out.HIGH = (in_h<<1) ^ (in_l>>63);\
    out.LOW = (in_l<<1) ^ (in_h>>63) * IRR;


#define GF128_MUL3(in, out)\
  out.HIGH = (in.HIGH<<1) ^ (in.LOW>>63) ^ in.HIGH;\
  out.LOW = (in.LOW<<1) ^ (in.HIGH>>63) * IRR ^ in.LOW;

#define GF128_MUL8(in, out)\
    GF128_MUL2_XOR_12(in, T1_L, T1_H);\
    GF128_MUL2_XOR_22(T1_L, T1_H, T2_L, T2_H);\
    GF128_MUL2_21(T2_L, T2_H, out)


#define LAST_XOR_EQ(a,b,c,d,e,f,mem)\
    mem.LOW = a.LOW ^ b.LOW ^ c.LOW ^ d.LOW ^ e.LOW ^ f.LOW;\
    mem.HIGH = a.HIGH ^ b.HIGH ^ c.HIGH ^ d.HIGH ^ e.HIGH ^ f.HIGH;
/**
 * @brief This is MDS mix colum fuction for Quicksand. 
 * 
 * @param state Quicksand state.
 */
void mix_column(qs_state *state)
{
    qs_state tmp_state = {0};
    memcpy(tmp_state.u8_state, state->u8_state, QS_STATE_SIZE);

    const register uint64_t IRR = 0x87;
    register uint64_t T1_L, T1_H, T2_L, T2_H;
    uint128_LH a,b,c,d,e,f;

    for(int j = 0; j < QS_SQUARE_STATE_DIM; j++)
    {
        // 1-st ROW
        GF128_MUL1(tmp_state.u128_LH_state[0][j], a);
        GF128_MUL1(tmp_state.u128_LH_state[1][j], b);
        GF128_MUL2(tmp_state.u128_LH_state[2][j], c);
        GF128_MUL3(tmp_state.u128_LH_state[3][j], d);
        GF128_MUL2(tmp_state.u128_LH_state[4][j], e);
        GF128_MUL8(tmp_state.u128_LH_state[5][j], f);
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[0][j]);
        
        // 2-nd ROW
        GF128_MUL8(tmp_state.u128_LH_state[0][j], a);
        // b remain the same value
        GF128_MUL1(tmp_state.u128_LH_state[2][j], c);
        GF128_XOR(tmp_state.u128_LH_state[3][j], d); // From MUL3 to MUL2
        GF128_XOR(tmp_state.u128_LH_state[4][j], e); // From MUL2 to MUL3
        GF128_MUL2(tmp_state.u128_LH_state[5][j], f);
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[1][j]);
        
        // 3-rd ROW
        GF128_MUL2(tmp_state.u128_LH_state[0][j], a);
        GF128_MUL8(tmp_state.u128_LH_state[1][j], b);
        // c remains the same value
        GF128_MUL1(tmp_state.u128_LH_state[3][j], d);
        GF128_XOR(tmp_state.u128_LH_state[4][j], e);// From MUL3 to MUL2
        GF128_XOR(tmp_state.u128_LH_state[5][j], f); // From MUL2 to MUL3
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[2][j]);
        
        // 4-th ROW
        GF128_XOR(tmp_state.u128_LH_state[0][j], a); // From MUL2 to MUL3
        GF128_MUL2(tmp_state.u128_LH_state[1][j], b);
        GF128_MUL8(tmp_state.u128_LH_state[2][j], c);
        // d remains the same
        GF128_MUL1(tmp_state.u128_LH_state[4][j], e);
        GF128_XOR(tmp_state.u128_LH_state[5][j], f); // From MUL3 to MUL2
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[3][j]);

        // 5-th ROW
        GF128_XOR(tmp_state.u128_LH_state[0][j], a); // From MUL3 to MUL2
        GF128_XOR(tmp_state.u128_LH_state[1][j], b); // From MUL2 to MUL3
        GF128_MUL2(tmp_state.u128_LH_state[2][j], c);
        GF128_MUL8(tmp_state.u128_LH_state[3][j], d);
        // e remains the same value
        GF128_MUL1(tmp_state.u128_LH_state[5][j], f);
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[4][j]);

        // 6-th ROW
        GF128_MUL1(tmp_state.u128_LH_state[0][j], a);
        GF128_XOR(tmp_state.u128_LH_state[1][j], b); // From MUL3 to MUL2
        GF128_XOR(tmp_state.u128_LH_state[2][j], c); // From MUL2 to MUL3
        GF128_MUL2(tmp_state.u128_LH_state[3][j], d);
        GF128_MUL8(tmp_state.u128_LH_state[4][j], e);
        // f remains the same
        LAST_XOR_EQ(a,b,c,d,e,f,state->u128_LH_state[5][j]);
    }
}

/**
 * @brief This is the permutation function for Quicksand.
 * 
 * @param state Quicksand state
 */
void permute(qs_state *state)
{
    int rn_idx = 0;
    for(int rn = 0; rn < QS_PERM_ROUNDS; rn++)
    {
        // Add Round Constants And Sub Byte
        add_round_constants_and_sub_byte(state, rcons + rn_idx);
        
        // Shift Rows
        shift_rows(state);

        // Mix Columns
        mix_column(state);

        rn_idx += 36;
    }
}
/**
 * @brief This is the absorb function for Quicksand.
 * 
 * @param internal_state Quicksand internal state
 * @param input_block Input block
 */
void absorb(qs_ctx_st *internal_state, uint8_t *input_block)
{
    // Xor input_block with bitrate section of internal state
    xor_f(internal_state->state.u128_linear_state, (u64x2*)input_block, QS_BITRATE_SIZE);

    // perform permutation function
    permute(&(internal_state->state));
}
/**
 * @brief This is the squeeze function for Quicksand. 
 * 
 * @param internal_state Quicksand internal state.
 * @param output Quicksand output.
 * @param output_len Length of Quicksand output.
 */
void squeeze(qs_ctx_st *internal_state, uint8_t *output, uint64_t output_len)
{
    memcpy(output, internal_state->state.u8_state, output_len);
}