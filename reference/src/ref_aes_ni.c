#include <stdint.h>     //for uint8_t
#include <string.h>     //for memcmp
#include <ref_aes_ni.h>

//compile using gcc and following arguments: -g;-O0;-Wall;-msse2;-msse;-march=native;-maes

//internal stuff

//macros
#define REF_DO_ENC_BLOCK(m,k) \
	do{\
        m = _mm_xor_si128       (m, k[ 0]); \
        m = _mm_aesenc_si128    (m, k[ 1]); \
        m = _mm_aesenc_si128    (m, k[ 2]); \
        m = _mm_aesenc_si128    (m, k[ 3]); \
        m = _mm_aesenc_si128    (m, k[ 4]); \
		m = _mm_aesenc_si128    (m, k[ 5]); \
		m = _mm_aesenc_si128    (m, k[ 6]); \
		m = _mm_aesenc_si128    (m, k[ 7]); \
		m = _mm_aesenc_si128    (m, k[ 8]); \
		m = _mm_aesenc_si128    (m, k[ 9]); \
        m = _mm_aesenclast_si128(m, k[ 10]);\
    }while(0)


#define REF_AES_128_key_exp(k, rcon) ref_aes_128_key_expansion(k, _mm_aeskeygenassist_si128(k, rcon))

static __m128i ref_key_schedule[11];//the expanded key

static __m128i ref_aes_128_key_expansion(__m128i key, __m128i keygened){
	keygened = _mm_shuffle_epi32(keygened, _MM_SHUFFLE(3,3,3,3));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
	return _mm_xor_si128(key, keygened);
}

//public API
void ref_aes128_load_key(uint8_t *enc_key){
    ref_key_schedule[0] = _mm_loadu_si128((const __m128i*) enc_key);
	ref_key_schedule[1]  = REF_AES_128_key_exp(ref_key_schedule[0], 0x01);
	ref_key_schedule[2]  = REF_AES_128_key_exp(ref_key_schedule[1], 0x02);
	ref_key_schedule[3]  = REF_AES_128_key_exp(ref_key_schedule[2], 0x04);
	ref_key_schedule[4]  = REF_AES_128_key_exp(ref_key_schedule[3], 0x08);
	ref_key_schedule[5]  = REF_AES_128_key_exp(ref_key_schedule[4], 0x10);
	ref_key_schedule[6]  = REF_AES_128_key_exp(ref_key_schedule[5], 0x20);
	ref_key_schedule[7]  = REF_AES_128_key_exp(ref_key_schedule[6], 0x40);
	ref_key_schedule[8]  = REF_AES_128_key_exp(ref_key_schedule[7], 0x80);
	ref_key_schedule[9]  = REF_AES_128_key_exp(ref_key_schedule[8], 0x1B);
	ref_key_schedule[10] = REF_AES_128_key_exp(ref_key_schedule[9], 0x36);
}

void ref_aes128_enc(__m128i *buffer){
    __m128i m = _mm_loadu_si128(buffer);

    REF_DO_ENC_BLOCK(m ,ref_key_schedule);

    _mm_storeu_si128(buffer, m);
}

void ref_aes128_enc_uint128(uint8_t *buffer){
    __m128i m = _mm_loadu_si128((__m128i *) buffer);

    REF_DO_ENC_BLOCK(m ,ref_key_schedule);

    _mm_storeu_si128((__m128i *) buffer, m);
}
