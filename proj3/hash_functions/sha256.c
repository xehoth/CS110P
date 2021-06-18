/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
* Copyright Notice: This file is dervived from work of Brad Conte at
  https://github.com/B-Con/crypto-algorithms/
*********************************************************************/

/*********************************************************************
* Filename:   sha256.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the SHA-256 hashing algorithm.
              SHA-256 is one of the three algorithms in the SHA2
              specification. The others, SHA-384 and SHA-512, are not
              offered in this implementation.
              Algorithm specification can be found here:
               * http://csrc.nist.gov/publications/fips/fips180-2/fips180-2withchangenotice.pdf
              This implementation uses little endian byte order.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include "sha256.h"
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

/****************************** MACROS ******************************/
// #define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
// #define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))

// #define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
// #define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
// #define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
// #define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
// #define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
// #define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
#define ADD(x, y) ((x) + (y))
#define ADD4(x, y, z, w) (ADD(ADD(x, y), ADD(z, w)))
#define INC(x, y) ((x) = ADD(x, y))
#define INC4(x, y, z, w) ((x) = ADD4(x, y, z, w))
#define XOR(x, y) ((x) ^ (y))
#define XOR3(x, y, z) (XOR(XOR(x, y), z))
#define OR(x, y) ((x) | (y))
#define AND(x, y) ((x) & (y))
#define SHR(x, n) ((x) >> (n))
#define SHL(x, n) ((x) << (n))

#define CH(x, y, z) (XOR(z, AND(x, XOR(y, z))))
#define MAJ(x, y, z) (OR(AND(x, y), AND(z, OR(x, y))))
#define EP0(x) (XOR3(OR(SHR(x, 2), SHL(x, 30)), OR(SHR(x, 13), SHL(x, 19)), OR(SHR(x, 22), SHL(x, 10))))
#define EP1(x) (XOR3(OR(SHR(x, 6), SHL(x, 26)), OR(SHR(x, 11), SHL(x, 21)), OR(SHR(x, 25), SHL(x, 7))))
#define SIG0(x) (XOR3(OR(SHR(x, 7), SHL(x, 25)), OR(SHR(x, 18), SHL(x, 14)), SHR(x, 3)))
#define SIG1(x) (XOR3(OR(SHR(x, 17), SHL(x, 15)), OR(SHR(x, 19), SHL(x, 13)), SHR(x, 10)))


/**************************** VARIABLES *****************************/

#define SHA256_ROUND(a, b, c, d, e, f, g, h, k, t1, t2) \
  t1 = ADD4(h, EP1(e), CH(e, f, g), (k));               \
  t2 = ADD(EP0(a), MAJ(a, b, c));                       \
  d = ADD(d, t1);                                       \
  h = ADD(t1, t2)

#define bigEndian32(x) (__builtin_bswap32(*(WORD *)(x)))
#define wLittleEndian32(addr, x) ((*((WORD *)(addr))) = __builtin_bswap32(x))

/*********************** FUNCTION DEFINITIONS ***********************/
void sha256_transform(SHA256_CTX *ctx, const BYTE data[]) {
  WORD a = ctx->state[0], b = ctx->state[1], c = ctx->state[2], d = ctx->state[3], e = ctx->state[4], f = ctx->state[5], g = ctx->state[6], h = ctx->state[7], t1, t2;
  WORD m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
  // 0 - 15
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0x428a2f98, (m0 = bigEndian32(data + 0))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0x71374491, (m1 = bigEndian32(data + 4))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0xb5c0fbcf, (m2 = bigEndian32(data + 8))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0xe9b5dba5, (m3 = bigEndian32(data + 12))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x3956c25b, (m4 = bigEndian32(data + 16))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0x59f111f1, (m5 = bigEndian32(data + 20))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x923f82a4, (m6 = bigEndian32(data + 24))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0xab1c5ed5, (m7 = bigEndian32(data + 28))), t1, t2);
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0xd807aa98, (m8 = bigEndian32(data + 32))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0x12835b01, (m9 = bigEndian32(data + 36))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0x243185be, (m10 = bigEndian32(data + 40))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0x550c7dc3, (m11 = bigEndian32(data + 44))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x72be5d74, (m12 = bigEndian32(data + 48))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0x80deb1fe, (m13 = bigEndian32(data + 52))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x9bdc06a7, (m14 = bigEndian32(data + 56))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0xc19bf174, (m15 = bigEndian32(data + 60))), t1, t2);
  
  // 16 - 31
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0xe49b69c1, INC4(m0, SIG1(m14), m9, SIG0(m1))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0xefbe4786, INC4(m1, SIG1(m15), m10, SIG0(m2))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0x0fc19dc6, INC4(m2, SIG1(m0), m11, SIG0(m3))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0x240ca1cc, INC4(m3, SIG1(m1), m12, SIG0(m4))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x2de92c6f, INC4(m4, SIG1(m2), m13, SIG0(m5))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0x4a7484aa, INC4(m5, SIG1(m3), m14, SIG0(m6))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x5cb0a9dc, INC4(m6, SIG1(m4), m15, SIG0(m7))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0x76f988da, INC4(m7, SIG1(m5), m0, SIG0(m8))), t1, t2);
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0x983e5152, INC4(m8, SIG1(m6), m1, SIG0(m9))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0xa831c66d, INC4(m9, SIG1(m7), m2, SIG0(m10))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0xb00327c8, INC4(m10, SIG1(m8), m3, SIG0(m11))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0xbf597fc7, INC4(m11, SIG1(m9), m4, SIG0(m12))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0xc6e00bf3, INC4(m12, SIG1(m10), m5, SIG0(m13))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0xd5a79147, INC4(m13, SIG1(m11), m6, SIG0(m14))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x06ca6351, INC4(m14, SIG1(m12), m7, SIG0(m15))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0x14292967, INC4(m15, SIG1(m13), m8, SIG0(m0))), t1, t2);
  // 32 - 47
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0x27b70a85, INC4(m0, SIG1(m14), m9, SIG0(m1))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0x2e1b2138, INC4(m1, SIG1(m15), m10, SIG0(m2))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0x4d2c6dfc, INC4(m2, SIG1(m0), m11, SIG0(m3))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0x53380d13, INC4(m3, SIG1(m1), m12, SIG0(m4))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x650a7354, INC4(m4, SIG1(m2), m13, SIG0(m5))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0x766a0abb, INC4(m5, SIG1(m3), m14, SIG0(m6))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x81c2c92e, INC4(m6, SIG1(m4), m15, SIG0(m7))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0x92722c85, INC4(m7, SIG1(m5), m0, SIG0(m8))), t1, t2);
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0xa2bfe8a1, INC4(m8, SIG1(m6), m1, SIG0(m9))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0xa81a664b, INC4(m9, SIG1(m7), m2, SIG0(m10))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0xc24b8b70, INC4(m10, SIG1(m8), m3, SIG0(m11))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0xc76c51a3, INC4(m11, SIG1(m9), m4, SIG0(m12))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0xd192e819, INC4(m12, SIG1(m10), m5, SIG0(m13))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0xd6990624, INC4(m13, SIG1(m11), m6, SIG0(m14))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0xf40e3585, INC4(m14, SIG1(m12), m7, SIG0(m15))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0x106aa070, INC4(m15, SIG1(m13), m8, SIG0(m0))), t1, t2);
  // 48 - 63
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0x19a4c116, INC4(m0, SIG1(m14), m9, SIG0(m1))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0x1e376c08, INC4(m1, SIG1(m15), m10, SIG0(m2))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0x2748774c, INC4(m2, SIG1(m0), m11, SIG0(m3))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0x34b0bcb5, INC4(m3, SIG1(m1), m12, SIG0(m4))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x391c0cb3, INC4(m4, SIG1(m2), m13, SIG0(m5))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0x4ed8aa4a, INC4(m5, SIG1(m3), m14, SIG0(m6))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0x5b9cca4f, INC4(m6, SIG1(m4), m15, SIG0(m7))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0x682e6ff3, INC4(m7, SIG1(m5), m0, SIG0(m8))), t1, t2);
  SHA256_ROUND(a, b, c, d, e, f, g, h, ADD(0x748f82ee, INC4(m8, SIG1(m6), m1, SIG0(m9))), t1, t2);
  SHA256_ROUND(h, a, b, c, d, e, f, g, ADD(0x78a5636f, INC4(m9, SIG1(m7), m2, SIG0(m10))), t1, t2);
  SHA256_ROUND(g, h, a, b, c, d, e, f, ADD(0x84c87814, INC4(m10, SIG1(m8), m3, SIG0(m11))), t1, t2);
  SHA256_ROUND(f, g, h, a, b, c, d, e, ADD(0x8cc70208, INC4(m11, SIG1(m9), m4, SIG0(m12))), t1, t2);
  SHA256_ROUND(e, f, g, h, a, b, c, d, ADD(0x90befffa, INC4(m12, SIG1(m10), m5, SIG0(m13))), t1, t2);
  SHA256_ROUND(d, e, f, g, h, a, b, c, ADD(0xa4506ceb, INC4(m13, SIG1(m11), m6, SIG0(m14))), t1, t2);
  SHA256_ROUND(c, d, e, f, g, h, a, b, ADD(0xbef9a3f7, ADD4(m14, SIG1(m12), m7, SIG0(m15))), t1, t2);
  SHA256_ROUND(b, c, d, e, f, g, h, a, ADD(0xc67178f2, ADD4(m15, SIG1(m13), m8, SIG0(m0))), t1, t2);

  ctx->state[0] += a;
  ctx->state[1] += b;
  ctx->state[2] += c;
  ctx->state[3] += d;
  ctx->state[4] += e;
  ctx->state[5] += f;
  ctx->state[6] += g;
  ctx->state[7] += h;
}

void sha256_init(SHA256_CTX *ctx) {
  ctx->datalen = 0;
  ctx->bitlen = 0;
  ctx->state[0] = 0x6a09e667;
  ctx->state[1] = 0xbb67ae85;
  ctx->state[2] = 0x3c6ef372;
  ctx->state[3] = 0xa54ff53a;
  ctx->state[4] = 0x510e527f;
  ctx->state[5] = 0x9b05688c;
  ctx->state[6] = 0x1f83d9ab;
  ctx->state[7] = 0x5be0cd19;
}

void sha256_update(SHA256_CTX *ctx, const BYTE data[], size_t len) {
  WORD i;
  if (ctx->datalen + len < 64) {
    memcpy(ctx->data + ctx->datalen, data, len);
    ctx->datalen += len;
    return;
  }
  i = 64 - ctx->datalen;
  memcpy(ctx->data + ctx->datalen, data, i);
  sha256_transform(ctx, ctx->data);
  ctx->bitlen += 512;
  for (; i + 64 <= len; i += 64) {
    memcpy(ctx->data, data + i, 64);
    sha256_transform(ctx, ctx->data);
    ctx->bitlen += 512;
  }
  ctx->datalen = len - i;
  memcpy(ctx->data, data + i, ctx->datalen);
  // for (i = 0; i < len; ++i) {
  //   ctx->data[ctx->datalen] = data[i];
  //   ctx->datalen++;
  //   if (ctx->datalen == 64) {
  //     sha256_transform(ctx, ctx->data);
  //     ctx->bitlen += 512;
  //     ctx->datalen = 0;
  //   }
  // }
}

void sha256_final(SHA256_CTX *ctx, BYTE hash[]) {
  WORD i;

  i = ctx->datalen;

  /* Pad whatever data is left in the buffer. */
  if (ctx->datalen < 56) {
    ctx->data[i] = 0x80;
    memset(ctx->data + i + 1, 0, 55 - i);
    // while (i < 56)
    //   ctx->data[i++] = 0x00;
  } else {
    ctx->data[i] = 0x80;
    // while (i < 64)
    //   ctx->data[i++] = 0x00;
    memset(ctx->data + i + 1, 0, 63 - i);
    sha256_transform(ctx, ctx->data);
    memset(ctx->data, 0, 56);
  }

  /* Append to the padding the total message's length in bits and transform. */
  ctx->bitlen += ctx->datalen * 8;
  ctx->data[63] = ctx->bitlen;
  ctx->data[62] = ctx->bitlen >> 8;
  ctx->data[61] = ctx->bitlen >> 16;
  ctx->data[60] = ctx->bitlen >> 24;
  ctx->data[59] = ctx->bitlen >> 32;
  ctx->data[58] = ctx->bitlen >> 40;
  ctx->data[57] = ctx->bitlen >> 48;
  ctx->data[56] = ctx->bitlen >> 56;
  sha256_transform(ctx, ctx->data);

  /* Since this implementation uses little endian byte ordering and SHA uses big
     endian, reverse all the bytes when copying the final state to the output
     hash. */
  wLittleEndian32(hash, ctx->state[0]);
  wLittleEndian32(hash + 4, ctx->state[1]);
  wLittleEndian32(hash + 8, ctx->state[2]);
  wLittleEndian32(hash + 12, ctx->state[3]);
  wLittleEndian32(hash + 16, ctx->state[4]);
  wLittleEndian32(hash + 20, ctx->state[5]);
  wLittleEndian32(hash + 24, ctx->state[6]);
  wLittleEndian32(hash + 28, ctx->state[7]);
}
