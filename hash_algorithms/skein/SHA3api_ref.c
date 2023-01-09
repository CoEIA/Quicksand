/***********************************************************************
**
** Implementation of the AHS API using the Skein hash function.
**
** Source code author: Doug Whiting, 2008.
**
** This algorithm and source code is released to the public domain.
** 
************************************************************************/

#include <string.h>     /* get the memcpy/memset functions */
#include "include/skein.h"      /* get the Skein API definitions   */
#include "include/SHA3api_ref.h"/* get the  AHS  API definitions   */

/******************************************************************/
/*     AHS API code                                               */
/******************************************************************/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* select the context size and init the context */
skein_HashReturn skein_Init(skein_hashState *state, int hashbitlen)
    {
    if (hashbitlen <= SKEIN_256_NIST_MAX_HASHBITS)
        {
        Skein_Assert(hashbitlen > 0,skein_BAD_HASHLEN);
        state->statebits = 64*SKEIN_256_STATE_WORDS;
        return Skein_256_Init(&state->u.ctx_256,(size_t) hashbitlen);
        }
    if (hashbitlen <= SKEIN_512_NIST_MAX_HASHBITS)
        {
        state->statebits = 64*SKEIN_512_STATE_WORDS;
        return Skein_512_Init(&state->u.ctx_512,(size_t) hashbitlen);
        }
    else
        {
        state->statebits = 64*SKEIN1024_STATE_WORDS;
        return Skein1024_Init(&state->u.ctx1024,(size_t) hashbitlen);
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* process data to be hashed */
skein_HashReturn skein_Update(skein_hashState *state, const skein_BitSequence *data, skein_DataLength databitlen)
    {
    /* only the final Update() call is allowed do partial bytes, else assert an error */
    Skein_Assert((state->u.h.T[1] & SKEIN_T1_FLAG_BIT_PAD) == 0 || databitlen == 0, skein_FAIL);

    Skein_Assert(state->statebits % 256 == 0 && (state->statebits-256) < 1024,skein_FAIL);
    if ((databitlen & 7) == 0)  /* partial bytes? */
        {
        switch ((state->statebits >> 8) & 3)
            {
            case 2:  return Skein_512_Update(&state->u.ctx_512,data,databitlen >> 3);
            case 1:  return Skein_256_Update(&state->u.ctx_256,data,databitlen >> 3);
            case 0:  return Skein1024_Update(&state->u.ctx1024,data,databitlen >> 3);
            default: return skein_FAIL;
            }
        }
    else
        {   /* handle partial final byte */
        size_t bCnt = (databitlen >> 3) + 1;                  /* number of bytes to handle (nonzero here!) */
        u08b_t b,mask;

        mask = (u08b_t) (1u << (7 - (databitlen & 7)));       /* partial byte bit mask */
        b    = (u08b_t) ((data[bCnt-1] & (0-mask)) | mask);   /* apply bit padding on final byte */

        switch ((state->statebits >> 8) & 3)
            {
            case 2:  Skein_512_Update(&state->u.ctx_512,data,bCnt-1); /* process all but the final byte    */
                     Skein_512_Update(&state->u.ctx_512,&b  ,  1   ); /* process the (masked) partial byte */
                     break;
            case 1:  Skein_256_Update(&state->u.ctx_256,data,bCnt-1); /* process all but the final byte    */
                     Skein_256_Update(&state->u.ctx_256,&b  ,  1   ); /* process the (masked) partial byte */
                     break;
            case 0:  Skein1024_Update(&state->u.ctx1024,data,bCnt-1); /* process all but the final byte    */
                     Skein1024_Update(&state->u.ctx1024,&b  ,  1   ); /* process the (masked) partial byte */
                     break;
            default: return skein_FAIL;
            }
        Skein_Set_Bit_Pad_Flag(state->u.h);                    /* set tweak flag for the final call */
        
        return skein_SUCCESS;
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* finalize hash computation and output the result (hashbitlen bits) */
skein_HashReturn skein_Final(skein_hashState *state, skein_BitSequence *hashval)
    {
    Skein_Assert(state->statebits % 256 == 0 && (state->statebits-256) < 1024,skein_FAIL);
    switch ((state->statebits >> 8) & 3)
        {
        case 2:  return Skein_512_Final(&state->u.ctx_512,hashval);
        case 1:  return Skein_256_Final(&state->u.ctx_256,hashval);
        case 0:  return Skein1024_Final(&state->u.ctx1024,hashval);
        default: return skein_FAIL;
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* all-in-one hash function */
skein_HashReturn skein_Hash(int hashbitlen, const skein_BitSequence *data, /* all-in-one call */
                skein_DataLength databitlen,skein_BitSequence *hashval)
    {
    skein_hashState  state;
    skein_HashReturn r = skein_Init(&state,hashbitlen);
    if (r == skein_SUCCESS)
        { /* these calls do not fail when called properly */
        r = skein_Update(&state,data,databitlen);
        skein_Final(&state,hashval);
        }
    return r;
    }