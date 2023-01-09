#ifdef AESNI
#ifndef AES_NI_H
#define AES_NI_H

#include <stdint.h>
#include <wmmintrin.h>  //for intrinsics for AES-NI

void aes128_load_key(uint8_t *enc_key);
void aes128_enc(__m128i *buffer);

#endif
#endif
