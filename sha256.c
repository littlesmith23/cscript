/**
* @file sha256.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Simple SHA256 hashing API
 *
 * This files contains an implementation of the SHA246 algorithm
 * and two API implementations.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sha256.h"
#include "tools.h"

typedef struct ctx_ {
    uint8_t messageBlock[64];
    uint32_t messageBlockLength;

    uint32_t w[64];
    uint32_t h[8];
    uint64_t length;
} sha256ctx;

//first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19
constexpr uint32_t fracSquareRootPrimeTable[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
};

//first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311
constexpr uint32_t fracCubeRootPrimeTable[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

void sha256_init(sha256_ctx *handle) {
    auto ctx = (sha256ctx**)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_init: ctx must not be null/n");
        exit(EXIT_FAILURE);
    }
    *ctx = (sha256ctx*)malloc(sizeof(sha256ctx));
    memset(*ctx, 0, sizeof(sha256_ctx));
    memcpy((*ctx)->h, fracSquareRootPrimeTable, sizeof(fracSquareRootPrimeTable));
}

void sha256_destroy(sha256_ctx handle) {
    auto ctx = (sha256ctx*)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_init: ctx must not be null/n");
        exit(EXIT_FAILURE);
    }
    free(ctx);
}

void sha256_block(sha256_ctx handle) {
    auto ctx = (sha256ctx*)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_block: handle must not be null/n");
        exit(EXIT_FAILURE);
    }

    unsigned i;
    uint32_t s0, s1;
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t t1, t2, maj, ch;

    for (i = 0; i < 16; i++) ctx->w[i] = swapByteOrder(*(uint32_t*)(ctx->messageBlock + i * 4));

    for (i = 16; i < 64; i++) {
        s0 = rotateRight(ctx->w[i - 15], 7) ^ rotateRight(ctx->w[i - 15], 18) ^ shiftRight(ctx->w[i - 15], 3);
        s1 = rotateRight(ctx->w[i - 2], 17) ^ rotateRight(ctx->w[i - 2], 19) ^ shiftRight(ctx->w[i - 2], 10);
        ctx->w[i] = ctx->w[i - 16] + s0 + ctx->w[i - 7] + s1;
    }

    a = ctx->h[0];
    b = ctx->h[1];
    c = ctx->h[2];
    d = ctx->h[3];
    e = ctx->h[4];
    f = ctx->h[5];
    g = ctx->h[6];
    h = ctx->h[7];

    for (i = 0; i < 64; i++) {
        s0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        t2 = s0 + maj;
        s1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
        ch = (e & f) ^ (~e & g);
        t1 = h + s1 + ch + fracCubeRootPrimeTable[i] + ctx->w[i];

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->h[0] += a;
    ctx->h[1] += b;
    ctx->h[2] += c;
    ctx->h[3] += d;
    ctx->h[4] += e;
    ctx->h[5] += f;
    ctx->h[6] += g;
    ctx->h[7] += h;

    //next block
    ctx->messageBlockLength = 0;
}

void sha256_update(sha256_ctx handle, const uint8_t *message, unsigned length) {
    auto ctx = (sha256ctx*)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_ctx: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    unsigned l;
    ctx->length += length;

    while (length) {
        l = 64 - ctx->messageBlockLength;
        l = (length < l) ? length : l;

        memcpy(ctx->messageBlock + ctx->messageBlockLength, message, l);
        message += l;
        ctx->messageBlockLength += l;
        length -= l;

        if (ctx->messageBlockLength == 64) sha256_block(ctx);
    }
}

void sha256_final(sha256_ctx handle) {
    auto ctx = (sha256ctx*)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_final: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    ctx->messageBlock[ctx->messageBlockLength++] = 0x80;

    if (ctx->messageBlockLength > 56) {
        memset(ctx->messageBlock + ctx->messageBlockLength, 0, 64 - ctx->messageBlockLength);
        sha256_block(ctx);
    }

    memset(ctx->messageBlock + ctx->messageBlockLength, 0, 56 - ctx->messageBlockLength);

    uint64_t len = ctx->length << 3;
    *(uint32_t*)(ctx->messageBlock + 56) = swapByteOrder(len >> 32);
    *(uint32_t*)(ctx->messageBlock + 60) = swapByteOrder(len);
    sha256_block(ctx);
}

void sha256_hash(sha256_ctx handle, uint8_t *hash) {
    auto ctx = (sha256ctx*)handle;
    if (ctx == nullptr) {
        fprintf(stderr, "sha256_ctx: handle must not be null/n");
        exit(EXIT_FAILURE);
    }
    auto t = (uint32_t *) hash;
    for (unsigned i = 0; i < 8; i++) {
        *t++ = swapByteOrder(ctx->h[i]);
    }
}


char *formatHash(const unsigned char *bin_hash) {
    static char formatted[SHA256_HASH_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_HASH_LENGTH; i++) {
        sprintf(formatted + i * 2, "%02x", bin_hash[i]);
    }
    return formatted;
}

char *sha256_string(const char *value) {
    unsigned char bin_hash[SHA256_HASH_LENGTH];
    sha256_ctx sha;
    sha256_init(&sha);
    sha256_update(sha, (const unsigned char *) value, strlen(value));
    sha256_final(sha);
    sha256_hash(sha, bin_hash);
    sha256_destroy(sha);
    return formatHash(bin_hash);
}


char *sha256_file(const char *file_path) {
    unsigned char bin_hash[SHA256_HASH_LENGTH];
    FILE *fp = fopen(file_path, "r");
    char *line = nullptr;
    alloc_string(&line, 16384);
    size_t len = 0;
    sha256_ctx sha;
    sha256_init(&sha);
    while (getline(&line, &len, fp) != -1) {
        sha256_update(sha, (const unsigned char *) line, strlen(line));
    }
    sha256_final(sha);
    sha256_hash(sha, bin_hash);
    sha256_destroy(sha);
    free_string(&line);
    return formatHash(bin_hash);
}

