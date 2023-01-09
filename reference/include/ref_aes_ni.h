#ifndef REF_AES_NI_H
#define REF_AES_NI_H

#include <stdint.h>
#include <wmmintrin.h>  //for intrinsics for AES-NI


void ref_aes128_load_key(uint8_t *enc_key);
void ref_aes128_enc(__m128i *buffer);

#endif
