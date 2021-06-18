/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#include "hash_function.h"
#include "hash_functions/sha256.h"
#include <immintrin.h>
#include <stdio.h>
#include <string.h>
/*
   The custom hash function based on SHA256 used for your testing.

   In the auto-grader, we will use another hash function (called PVG256) derived
   from SHA256 for testing.
*/
void blockchain_do_hash(unsigned char *data, size_t data_size,
                        unsigned char buf[HASH_BLOCK_SIZE]) {
  SHA256_CTX ctx;

  sha256_init(&ctx);
  sha256_update(&ctx, data, data_size);
  sha256_final(&ctx, buf);
}

#define VADD(x, y) (_mm256_add_epi32(x, y))
#define VADD4(x, y, z, w) (VADD(VADD(x, y), VADD(z, w)))
#define VINC(x, y) ((x) = VADD(x, y))
#define VINC4(x, y, z, w) ((x) = VADD4(x, y, z, w))
#define VXOR(x, y) (_mm256_xor_si256(x, y))
#define VXOR3(x, y, z) (VXOR(VXOR(x, y), z))
#define VOR(x, y) (_mm256_or_si256(x, y))
#define VAND(x, y) (_mm256_and_si256(x, y))
#define VSHR(x, n) (_mm256_srli_epi32(x, n))
#define VSHL(x, n) (_mm256_slli_epi32(x, n))

#define VCH(x, y, z) (VXOR(z, VAND(x, VXOR(y, z))))
#define VMAJ(x, y, z) (VOR(VAND(x, y), VAND(z, VOR(x, y))))
#define VEP0(x) (VXOR3(VOR(VSHR(x, 2), VSHL(x, 30)), VOR(VSHR(x, 13), VSHL(x, 19)), VOR(VSHR(x, 22), VSHL(x, 10))))
#define VEP1(x) (VXOR3(VOR(VSHR(x, 6), VSHL(x, 26)), VOR(VSHR(x, 11), VSHL(x, 21)), VOR(VSHR(x, 25), VSHL(x, 7))))
#define VSIG0(x) (VXOR3(VOR(VSHR(x, 7), VSHL(x, 25)), VOR(VSHR(x, 18), VSHL(x, 14)), VSHR(x, 3)))
#define VSIG1(x) (VXOR3(VOR(VSHR(x, 17), VSHL(x, 15)), VOR(VSHR(x, 19), VSHL(x, 13)), VSHR(x, 10)))
#define AVX_1EPI32(x) (_mm256_set1_epi32(x))
#define VSHA256_ROUND(a, b, c, d, e, f, g, h, k, t1, t2) \
  t1 = VADD4(h, VEP1(e), VCH(e, f, g), (k));               \
  t2 = VADD(VEP0(a), VMAJ(a, b, c));                       \
  d = VADD(d, t1);                                       \
  h = VADD(t1, t2)

#define vbigEndian32(x) (__builtin_bswap32(*(WORD *)(x)))
#define AVX_1EPI32C(x) (AVX_1EPI32(vbigEndian32((x))))
#define vwLittleEndian32(addr, x) ((*((WORD *)(addr))) = __builtin_bswap32(x))

#define LOAD_DATA_AVX(x, offset) (_mm256_set1_epi32(vbigEndian32(x + (offset))))
#define LOAD_NONCE_AVX(x, offset) (_mm256_set_epi32(vbigEndian32(x[0] + (offset)), vbigEndian32(x[1] + (offset)), vbigEndian32(x[2] + (offset)), vbigEndian32(x[3] + (offset)), vbigEndian32(x[4] + (offset)), vbigEndian32(x[5] + (offset)), vbigEndian32(x[6] + (offset)), vbigEndian32(x[7] + (offset))))

void transform8way(WORD state[8], const unsigned char data[], uint64_t nonce, WORD outputState[8][8]) {
  __m256i a = AVX_1EPI32(state[0]), b = AVX_1EPI32(state[1]), c = AVX_1EPI32(state[2]), d = AVX_1EPI32(state[3]), e = AVX_1EPI32(state[4]), f = AVX_1EPI32(state[5]), g = AVX_1EPI32(state[6]), h = AVX_1EPI32(state[7]), t1, t2;
  __m256i m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15;
  unsigned char nonceData[8][8];
  memcpy(nonceData[0], &nonce, 8);
  nonce++;
  memcpy(nonceData[1], &nonce, 8);
  nonce++;
  memcpy(nonceData[2], &nonce, 8);
  nonce++;
  memcpy(nonceData[3], &nonce, 8);
  nonce++;
  memcpy(nonceData[4], &nonce, 8);
  nonce++;
  memcpy(nonceData[5], &nonce, 8);
  nonce++;
  memcpy(nonceData[6], &nonce, 8);
  nonce++;
  memcpy(nonceData[7], &nonce, 8);

  // 0 - 15
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0x428a2f98), (m0 = AVX_1EPI32C(data + 0))), t1, t2);
  // printf("avx a = %u\n", vbigEndian32(data + 0));//*(unsigned int *)&m0);
  // exit(0);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0x71374491), (m1 = AVX_1EPI32C(data + 4))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0xb5c0fbcf), (m2 = AVX_1EPI32C(data + 8))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0xe9b5dba5), (m3 = AVX_1EPI32C(data + 12))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x3956c25b), (m4 = AVX_1EPI32C(data + 16))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0x59f111f1), (m5 = AVX_1EPI32C(data + 20))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x923f82a4), (m6 = AVX_1EPI32C(data + 24))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0xab1c5ed5), (m7 = AVX_1EPI32C(data + 28))), t1, t2);
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0xd807aa98), (m8 = AVX_1EPI32C(data + 32))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0x12835b01), (m9 = AVX_1EPI32C(data + 36))), t1, t2);

  // VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x243185be), (m10 = AVX_1EPI32C(data + 40))), t1, t2);
  // VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x550c7dc3), (m11 = AVX_1EPI32C(data + 44))), t1, t2);  
  // VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x243185be), (m10 = AVX_1EPI32C(nonceData[0] + 0))), t1, t2);
  // VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x550c7dc3), (m11 = AVX_1EPI32C(nonceData[0] + 4))), t1, t2);  
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x243185be), (m10 = LOAD_NONCE_AVX(nonceData, 0))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x550c7dc3), (m11 = LOAD_NONCE_AVX(nonceData, 4))), t1, t2);

  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x72be5d74), (m12 = AVX_1EPI32C(data + 48))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0x80deb1fe), (m13 = AVX_1EPI32C(data + 52))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x9bdc06a7), (m14 = AVX_1EPI32C(data + 56))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0xc19bf174), (m15 = AVX_1EPI32C(data + 60))), t1, t2);
  
  // 16 - 31
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0xe49b69c1), VINC4(m0, VSIG1(m14), m9, VSIG0(m1))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0xefbe4786), VINC4(m1, VSIG1(m15), m10, VSIG0(m2))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x0fc19dc6), VINC4(m2, VSIG1(m0), m11, VSIG0(m3))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x240ca1cc), VINC4(m3, VSIG1(m1), m12, VSIG0(m4))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x2de92c6f), VINC4(m4, VSIG1(m2), m13, VSIG0(m5))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0x4a7484aa), VINC4(m5, VSIG1(m3), m14, VSIG0(m6))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x5cb0a9dc), VINC4(m6, VSIG1(m4), m15, VSIG0(m7))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0x76f988da), VINC4(m7, VSIG1(m5), m0, VSIG0(m8))), t1, t2);
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0x983e5152), VINC4(m8, VSIG1(m6), m1, VSIG0(m9))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0xa831c66d), VINC4(m9, VSIG1(m7), m2, VSIG0(m10))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0xb00327c8), VINC4(m10, VSIG1(m8), m3, VSIG0(m11))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0xbf597fc7), VINC4(m11, VSIG1(m9), m4, VSIG0(m12))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0xc6e00bf3), VINC4(m12, VSIG1(m10), m5, VSIG0(m13))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0xd5a79147), VINC4(m13, VSIG1(m11), m6, VSIG0(m14))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x06ca6351), VINC4(m14, VSIG1(m12), m7, VSIG0(m15))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0x14292967), VINC4(m15, VSIG1(m13), m8, VSIG0(m0))), t1, t2);
  // 32 - 47
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0x27b70a85), VINC4(m0, VSIG1(m14), m9, VSIG0(m1))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0x2e1b2138), VINC4(m1, VSIG1(m15), m10, VSIG0(m2))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x4d2c6dfc), VINC4(m2, VSIG1(m0), m11, VSIG0(m3))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x53380d13), VINC4(m3, VSIG1(m1), m12, VSIG0(m4))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x650a7354), VINC4(m4, VSIG1(m2), m13, VSIG0(m5))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0x766a0abb), VINC4(m5, VSIG1(m3), m14, VSIG0(m6))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x81c2c92e), VINC4(m6, VSIG1(m4), m15, VSIG0(m7))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0x92722c85), VINC4(m7, VSIG1(m5), m0, VSIG0(m8))), t1, t2);
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0xa2bfe8a1), VINC4(m8, VSIG1(m6), m1, VSIG0(m9))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0xa81a664b), VINC4(m9, VSIG1(m7), m2, VSIG0(m10))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0xc24b8b70), VINC4(m10, VSIG1(m8), m3, VSIG0(m11))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0xc76c51a3), VINC4(m11, VSIG1(m9), m4, VSIG0(m12))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0xd192e819), VINC4(m12, VSIG1(m10), m5, VSIG0(m13))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0xd6990624), VINC4(m13, VSIG1(m11), m6, VSIG0(m14))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0xf40e3585), VINC4(m14, VSIG1(m12), m7, VSIG0(m15))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0x106aa070), VINC4(m15, VSIG1(m13), m8, VSIG0(m0))), t1, t2);
  // 48 - 63
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0x19a4c116), VINC4(m0, VSIG1(m14), m9, VSIG0(m1))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0x1e376c08), VINC4(m1, VSIG1(m15), m10, VSIG0(m2))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x2748774c), VINC4(m2, VSIG1(m0), m11, VSIG0(m3))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x34b0bcb5), VINC4(m3, VSIG1(m1), m12, VSIG0(m4))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x391c0cb3), VINC4(m4, VSIG1(m2), m13, VSIG0(m5))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0x4ed8aa4a), VINC4(m5, VSIG1(m3), m14, VSIG0(m6))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0x5b9cca4f), VINC4(m6, VSIG1(m4), m15, VSIG0(m7))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0x682e6ff3), VINC4(m7, VSIG1(m5), m0, VSIG0(m8))), t1, t2);
  VSHA256_ROUND(a, b, c, d, e, f, g, h, VADD(AVX_1EPI32(0x748f82ee), VINC4(m8, VSIG1(m6), m1, VSIG0(m9))), t1, t2);
  VSHA256_ROUND(h, a, b, c, d, e, f, g, VADD(AVX_1EPI32(0x78a5636f), VINC4(m9, VSIG1(m7), m2, VSIG0(m10))), t1, t2);
  VSHA256_ROUND(g, h, a, b, c, d, e, f, VADD(AVX_1EPI32(0x84c87814), VINC4(m10, VSIG1(m8), m3, VSIG0(m11))), t1, t2);
  VSHA256_ROUND(f, g, h, a, b, c, d, e, VADD(AVX_1EPI32(0x8cc70208), VINC4(m11, VSIG1(m9), m4, VSIG0(m12))), t1, t2);
  VSHA256_ROUND(e, f, g, h, a, b, c, d, VADD(AVX_1EPI32(0x90befffa), VINC4(m12, VSIG1(m10), m5, VSIG0(m13))), t1, t2);
  VSHA256_ROUND(d, e, f, g, h, a, b, c, VADD(AVX_1EPI32(0xa4506ceb), VINC4(m13, VSIG1(m11), m6, VSIG0(m14))), t1, t2);
  VSHA256_ROUND(c, d, e, f, g, h, a, b, VADD(AVX_1EPI32(0xbef9a3f7), VADD4(m14, VSIG1(m12), m7, VSIG0(m15))), t1, t2);
  VSHA256_ROUND(b, c, d, e, f, g, h, a, VADD(AVX_1EPI32(0xc67178f2), VADD4(m15, VSIG1(m13), m8, VSIG0(m0))), t1, t2);

  a = VADD(a, AVX_1EPI32(state[0]));
  b = VADD(b, AVX_1EPI32(state[1]));
  c = VADD(c, AVX_1EPI32(state[2]));
  d = VADD(d, AVX_1EPI32(state[3]));
  e = VADD(e, AVX_1EPI32(state[4]));
  f = VADD(f, AVX_1EPI32(state[5]));
  g = VADD(g, AVX_1EPI32(state[6]));
  h = VADD(h, AVX_1EPI32(state[7]));
  
  _mm256_storeu_si256((__m256i *)&outputState[0], a);
  // printf("output = 0x%08x", outputState[0][7]);
  // exit(0);
  _mm256_storeu_si256((__m256i *)&outputState[1], b);
  _mm256_storeu_si256((__m256i *)&outputState[2], c);
  _mm256_storeu_si256((__m256i *)&outputState[3], d);
  _mm256_storeu_si256((__m256i *)&outputState[4], e);
  _mm256_storeu_si256((__m256i *)&outputState[5], f);
  _mm256_storeu_si256((__m256i *)&outputState[6], g);
  _mm256_storeu_si256((__m256i *)&outputState[7], h);
}