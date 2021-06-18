/*********************************************************************
Homework 5
CS 110: Computer Architecture, Spring 2021
ShanghaiTech University

* Last Modified: 03/28/2021
*********************************************************************/

#include "blockchain.h"
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "hash_functions/sha256.h"
#include <stddef.h>
#include <stdio.h>

#define bwLittleEndian32(addr, x) ((*((WORD *)(addr))) = __builtin_bswap32(x))

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

void getSha256State(blk_t *node, WORD state[8]) {
  SHA256_CTX ctx;
  sha256_init(&ctx);
  sha256_update(&ctx, (unsigned char *)node, 256);
  memcpy(state, ctx.state, 8 * sizeof(WORD));
}

void blockchain_node_init(blk_t *node, uint32_t index, uint32_t timestamp,
                          unsigned char prev_hash[32], unsigned char *data,
                          size_t data_size) {
  if (!node || !data || !prev_hash)
    return;

  node->header.index = index;
  node->header.timestamp = timestamp;
  node->header.nonce = -1;

  memset(node->header.data, 0, sizeof(unsigned char) * 256);
  memcpy(node->header.prev_hash, prev_hash, HASH_BLOCK_SIZE);
  memcpy(node->header.data, data,
         sizeof(unsigned char) * ((data_size < 256) ? data_size : 256));
}

void blockchain_node_hash(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          hash_func func) {
  if (node)
    func((unsigned char *)node, sizeof(blkh_t), (unsigned char *)hash_buf);
}

BOOL blockchain_node_verify(blk_t *node, blk_t *prev_node, hash_func func) {
  unsigned char hash_buf[HASH_BLOCK_SIZE];

  if (!node || !prev_node)
    return False;

  blockchain_node_hash(node, hash_buf, func);
  if (memcmp(node->hash, hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE))
    return False;

  blockchain_node_hash(prev_node, hash_buf, func);
  if (memcmp(node->header.prev_hash, hash_buf,
             sizeof(unsigned char) * HASH_BLOCK_SIZE))
    return False;

  return True;
}
#define THREAD_NUM 20
#define NONCE_STEP (THREAD_NUM)

#define USE_AVX 1
#ifndef USE_AVX
#define USE_AVX 0
#endif

struct __attribute__((aligned(64))) ThreadData {
  blkh_t header;
  unsigned char hash_buf[HASH_BLOCK_SIZE];
  int flag;
};

struct __attribute__((aligned(64))) ThreadDataAvx {
  uint64_t nonce;
  unsigned char hash_buf[HASH_BLOCK_SIZE];
  int flag;
};

// extern void sha256_transform(SHA256_CTX *ctx, const BYTE data[]);
extern void transform8way(WORD state[8], const unsigned char *data, uint64_t nonce, WORD outputState[8][8]);

/* The sequiental implementation of mining implemented for you. */
void blockchain_node_mine(blk_t *node, unsigned char hash_buf[HASH_BLOCK_SIZE],
                          size_t diff, hash_func func) {
  if (!USE_AVX || sizeof(blkh_t) != 304) {
    int any_find_flag = 0, i;
    unsigned char one_diff[HASH_BLOCK_SIZE];
    size_t diff_q, diff_m;
    diff_q = diff / 8;
    diff_m = diff % 8;
    memset(one_diff, 0xFF, sizeof(unsigned char) * HASH_BLOCK_SIZE);
    memset(one_diff, 0, sizeof(unsigned char) * diff_q);
    one_diff[diff_q] = ((uint8_t)0xFF) >> diff_m;
    struct ThreadData thData[THREAD_NUM];
    for (i = 0; i < THREAD_NUM; ++i) thData[i].flag = 0;
    const int BEFORE_SIZE = offsetof(blkh_t, nonce);
    const int AFTER_SIZE = sizeof(blkh_t) - BEFORE_SIZE - 8;
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, (unsigned char *)node, BEFORE_SIZE);
    #pragma omp parallel num_threads(THREAD_NUM)
    {
      const int id = omp_get_thread_num();
      memcpy(&thData[id].header, node, sizeof(blkh_t));
      thData[id].header.nonce = id;
      SHA256_CTX curCtx;
      for (; unlikely(!any_find_flag);) {
        memcpy(&curCtx, &ctx, sizeof(SHA256_CTX));
        sha256_update(&curCtx, (BYTE *)&thData[id].header.nonce, 8);
        sha256_update(&curCtx, ((BYTE *)&thData[id].header) + BEFORE_SIZE + 8, AFTER_SIZE);
        sha256_final(&curCtx, thData[id].hash_buf);
        // blockchain_node_hash((blk_t *)&thData[id].header, thData[id].hash_buf, func);
        if (unlikely(likely((!memcmp(thData[id].hash_buf, one_diff, sizeof(unsigned char) * diff_q))) &&
                             unlikely(memcmp(&thData[id].hash_buf[diff_q], &one_diff[diff_q],
                  sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q)) <= 0))) {
          any_find_flag = 1;
          thData[id].flag = 1;
          break;
        }
        thData[id].header.nonce += NONCE_STEP;
      }
    }
    for (i = 0; i < THREAD_NUM; ++i) {
      if (thData[i].flag) {
        node->header.nonce = thData[i].header.nonce;
        memcpy(hash_buf, thData[i].hash_buf, HASH_BLOCK_SIZE);
        memcpy(node->hash, thData[i].hash_buf, HASH_BLOCK_SIZE);
        break;
      }
    }
    (void)func;
  } else {
    int any_find_flag = 0, i;
    unsigned char one_diff[HASH_BLOCK_SIZE];
    size_t diff_q, diff_m;
    diff_q = diff / 8;
    diff_m = diff % 8;
    memset(one_diff, 0xFF, sizeof(unsigned char) * HASH_BLOCK_SIZE);
    memset(one_diff, 0, sizeof(unsigned char) * diff_q);
    one_diff[diff_q] = ((uint8_t)0xFF) >> diff_m;
    struct ThreadDataAvx thData[THREAD_NUM];
    for (i = 0; i < THREAD_NUM; ++i) thData[i].flag = 0;
    WORD state[8];
    getSha256State(node, state);
    unsigned char ctxData[64];
    memcpy(ctxData, ((BYTE *)node) + 256, 40);
    ctxData[48] = 0x80;
    memset(ctxData + 48 + 1, 0, 55 + 6 - 48);
    // bitlen = 4 * 512
    // datalen = 48
    // bitlen += datalen * 8
    // bitlen = 2432
    ctxData[63] = (BYTE)2432;
    ctxData[62] = (BYTE)(2432 >> 8);

    #pragma omp parallel num_threads(THREAD_NUM)
    {
      const int id = omp_get_thread_num();
      thData[id].nonce = id * 8;
      // SHA256_CTX curCtx;
      // unsigned char curData[64];
      // memcpy(curData, ctxData, 64);
      WORD outputState[8][8];

      for (; !any_find_flag;) {
        // *(uint64_t *)(curData + 40) = thData[id].nonce;
        // memcpy(curCtx.state, state, sizeof(state));
        // sha256_transform(&curCtx, curData);
        // bwLittleEndian32(thData[id].hash_buf, curCtx.state[0]);
        // bwLittleEndian32(thData[id].hash_buf + 4, curCtx.state[1]);
        // bwLittleEndian32(thData[id].hash_buf + 8, curCtx.state[2]);
        // bwLittleEndian32(thData[id].hash_buf + 12, curCtx.state[3]);
        // bwLittleEndian32(thData[id].hash_buf + 16, curCtx.state[4]);
        // bwLittleEndian32(thData[id].hash_buf + 20, curCtx.state[5]);
        // bwLittleEndian32(thData[id].hash_buf + 24, curCtx.state[6]);
        // bwLittleEndian32(thData[id].hash_buf + 28, curCtx.state[7]);

        // if ((!memcmp(thData[id].hash_buf, one_diff, sizeof(unsigned char) * diff_q)) &&
        //     memcmp(&thData[id].hash_buf[diff_q], &one_diff[diff_q],
        //           sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q)) <= 0) {
        //   any_find_flag = 1;
        //   thData[id].flag = 1;
        //   break;
        // }
        // thData[id].nonce += NONCE_STEP;

        transform8way(state, ctxData, thData[id].nonce, outputState);
        for (int now = 0; now < 8; ++now) {
          bwLittleEndian32(thData[id].hash_buf, outputState[0][now]);
          bwLittleEndian32(thData[id].hash_buf + 4, outputState[1][now]);
          bwLittleEndian32(thData[id].hash_buf + 8, outputState[2][now]);
          bwLittleEndian32(thData[id].hash_buf + 12, outputState[3][now]);
          bwLittleEndian32(thData[id].hash_buf + 16, outputState[4][now]);
          bwLittleEndian32(thData[id].hash_buf + 20, outputState[5][now]);
          bwLittleEndian32(thData[id].hash_buf + 24, outputState[6][now]);
          bwLittleEndian32(thData[id].hash_buf + 28, outputState[7][now]);
          if ((!memcmp(thData[id].hash_buf, one_diff, sizeof(unsigned char) * diff_q)) &&
            memcmp(&thData[id].hash_buf[diff_q], &one_diff[diff_q],
                  sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q)) <= 0) {
            thData[id].nonce += 7 - now;
            any_find_flag = 1;
            thData[id].flag = 1;
            break;
          }
        }
        if (thData[id].flag) break;
        thData[id].nonce += NONCE_STEP * 8;
      }
    }
    for (i = 0; i < THREAD_NUM; ++i) {
      if (thData[i].flag) {
        node->header.nonce = thData[i].nonce;
        memcpy(hash_buf, thData[i].hash_buf, HASH_BLOCK_SIZE);
        memcpy(node->hash, hash_buf, HASH_BLOCK_SIZE);
        break;
      }
    }
    (void)func;
  }

  // while (True) {
  //   blockchain_node_hash(node, hash_buf, func);
  //   if ((!memcmp(hash_buf, one_diff, sizeof(unsigned char) * diff_q)) &&
  //       memcmp(&hash_buf[diff_q], &one_diff[diff_q],
  //              sizeof(unsigned char) * (HASH_BLOCK_SIZE - diff_q)) <= 0) {

  //     memcpy(node->hash, hash_buf, sizeof(unsigned char) * HASH_BLOCK_SIZE);
  //     break;
  //   }
  //   node->header.nonce++;
  // }
  // printf("nonce: %lu\n", node->header.nonce);
}
