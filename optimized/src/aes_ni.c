#ifdef AESNI
#include <stdint.h>     //for uint8_t
#include <string.h>     //for memcmp
#include <aes_ni.h>

//compile using gcc and following arguments: -g;-O0;-Wall;-msse2;-msse;-march=native;-maes

//internal stuff

//macros
#define DO_ENC_BLOCK(m,k) \
        m = _mm_xor_si128       (m, k[ 0].key128); \
        m = _mm_aesenc_si128    (m, k[ 1].key128); \
        m = _mm_aesenc_si128    (m, k[ 2].key128); \
        m = _mm_aesenc_si128    (m, k[ 3].key128); \
        m = _mm_aesenc_si128    (m, k[ 4].key128); \
		m = _mm_aesenc_si128    (m, k[ 5].key128); \
		m = _mm_aesenc_si128    (m, k[ 6].key128); \
		m = _mm_aesenc_si128    (m, k[ 7].key128); \
		m = _mm_aesenc_si128    (m, k[ 8].key128); \
		m = _mm_aesenc_si128    (m, k[ 9].key128); \
        m = _mm_aesenclast_si128(m, k[ 10].key128);


typedef union _key_st{
	uint8_t  key8[16];
	__m128i  key128;	
}key_st;

static const key_st key_schedule[11] = {
	{.key8={0xBD, 0xCF, 0x6F, 0x64, 0xB7, 0x71, 0xF2, 0x41, 0xE4, 0x1D, 0xD4, 0xC9, 0xFC, 0x89, 0x77, 0x51, }},
    {.key8={0x1B, 0x3A, 0xBE, 0xD4, 0xAC, 0x4B, 0x4C, 0x95, 0x48, 0x56, 0x98, 0x5C, 0xB4, 0xDF, 0xEF, 0x0D, }},
    {.key8={0x87, 0xE5, 0x69, 0x59, 0x2B, 0xAE, 0x25, 0xCC, 0x63, 0xF8, 0xBD, 0x90, 0xD7, 0x27, 0x52, 0x9D, }},
    {.key8={0x4F, 0xE5, 0x37, 0x57, 0x64, 0x4B, 0x12, 0x9B, 0x07, 0xB3, 0xAF, 0x0B, 0xD0, 0x94, 0xFD, 0x96, }},
    {.key8={0x65, 0xB1, 0xA7, 0x27, 0x01, 0xFA, 0xB5, 0xBC, 0x06, 0x49, 0x1A, 0xB7, 0xD6, 0xDD, 0xE7, 0x21, }},
    {.key8={0xB4, 0x25, 0x5A, 0xD1, 0xB5, 0xDF, 0xEF, 0x6D, 0xB3, 0x96, 0xF5, 0xDA, 0x65, 0x4B, 0x12, 0xFB, }},
    {.key8={0x27, 0xEC, 0x55, 0x9C, 0x92, 0x33, 0xBA, 0xF1, 0x21, 0xA5, 0x4F, 0x2B, 0x44, 0xEE, 0x5D, 0xD0, }},
    {.key8={0x4F, 0xA0, 0x25, 0x87, 0xDD, 0x93, 0x9F, 0x76, 0xFC, 0x36, 0xD0, 0x5D, 0xB8, 0xD8, 0x8D, 0x8D, }},
    {.key8={0xAE, 0xFD, 0x78, 0xEB, 0x73, 0x6E, 0xE7, 0x9D, 0x8F, 0x58, 0x37, 0xC0, 0x37, 0x80, 0xBA, 0x4D, }},
    {.key8={0x78, 0x09, 0x9B, 0x71, 0x0B, 0x67, 0x7C, 0xEC, 0x84, 0x3F, 0x4B, 0x2C, 0xB3, 0xBF, 0xF1, 0x61, }},
    {.key8={0x46, 0xA8, 0x74, 0x1C, 0x4D, 0xCF, 0x08, 0xF0, 0xC9, 0xF0, 0x43, 0xDC, 0x7A, 0x4F, 0xB2, 0xBD}}
}; 

//public API

void aes128_enc(__m128i *buffer){
    __m128i m[6];
	 m[0] = _mm_loadu_si128(&buffer[0]);
	 m[1] = _mm_loadu_si128(&buffer[1]);
	 m[2] = _mm_loadu_si128(&buffer[2]);
	 m[3] = _mm_loadu_si128(&buffer[3]);
	 m[4] = _mm_loadu_si128(&buffer[4]);
	 m[5] = _mm_loadu_si128(&buffer[5]);

    DO_ENC_BLOCK(m[0] ,key_schedule);
	DO_ENC_BLOCK(m[1] ,key_schedule);
	DO_ENC_BLOCK(m[2] ,key_schedule);
	DO_ENC_BLOCK(m[3] ,key_schedule);
	DO_ENC_BLOCK(m[4] ,key_schedule);
	DO_ENC_BLOCK(m[5] ,key_schedule);

    _mm_storeu_si128(&buffer[0], m[0]);
	_mm_storeu_si128(&buffer[1], m[1]);
	_mm_storeu_si128(&buffer[2], m[2]);
	_mm_storeu_si128(&buffer[3], m[3]);
	_mm_storeu_si128(&buffer[4], m[4]);
	_mm_storeu_si128(&buffer[5], m[5]);
}

#endif
