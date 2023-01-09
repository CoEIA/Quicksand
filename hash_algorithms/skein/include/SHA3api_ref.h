#ifndef _AHS_API_H_
#define _AHS_API_H_

/***********************************************************************
**
** Interface declarations of the AHS API using the Skein hash function.
**
** Source code author: Doug Whiting, 2008.
**
** This algorithm and source code is released to the public domain.
** 
************************************************************************/

#include "skein.h"

typedef enum
    {
    skein_SUCCESS     = SKEIN_SUCCESS,
    skein_FAIL        = SKEIN_FAIL,
    skein_BAD_HASHLEN = SKEIN_BAD_HASHLEN
    }
    skein_HashReturn;

typedef size_t   skein_DataLength;                /* bit count  type */
typedef u08b_t   skein_BitSequence;               /* bit stream type */

typedef struct
    {
    uint_t  statebits;                      /* 256, 512, or 1024 */
    union
        {
        Skein_Ctxt_Hdr_t h;                 /* common header "overlay" */
        Skein_256_Ctxt_t ctx_256;
        Skein_512_Ctxt_t ctx_512;
        Skein1024_Ctxt_t ctx1024;
        } u;
    }
    skein_hashState;

/* "incremental" hashing API */
skein_HashReturn skein_Init  (skein_hashState *state, int hashbitlen);
skein_HashReturn skein_Update(skein_hashState *state, const skein_BitSequence *data, skein_DataLength databitlen);
skein_HashReturn skein_Final (skein_hashState *state,       skein_BitSequence *hashval);

/* "all-in-one" call */
skein_HashReturn skein_Hash  (int hashbitlen,   const skein_BitSequence *data, 
                  skein_DataLength databitlen,  skein_BitSequence *hashval);


/*
** Re-define the compile-time constants below to change the selection
** of the Skein state size in the Init() function in SHA3api_ref.c.
**
** That is, the NIST API does not allow for explicit selection of the
** Skein block size, so it must be done implicitly in the Init() function.
** The selection is controlled by these constants.
*/
#ifndef SKEIN_256_NIST_MAX_HASHBITS
#define SKEIN_256_NIST_MAX_HASHBITS (256)
#endif

#ifndef SKEIN_512_NIST_MAX_HASHBITS
#define SKEIN_512_NIST_MAX_HASHBITS (512)
#endif

#endif  /* ifdef _AHS_API_H_ */