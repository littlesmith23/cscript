/**
 * @file sha256.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Simple SHA256 hashing API
 *
 * This file defines a simple API to create SHA256
 * hashes from strings or files.
 */

#pragma once

#include <stdint.h>
/**
 * @brief The length of a SHA256 hash in bytes
 */
#define SHA256_HASH_LENGTH 32

/**
 * @brief The hashing context when using the basic hash functions directly
 */
typedef void* sha256_ctx;

/**
 * @brief Finalizes the hash
 *
 * Returns the SHA256 hash of the provided string
 * as a hex string.
 *
 * @param ctx The hashing context
 */
void sha256_init(sha256_ctx *ctx);

/**
 * @brief Finalizes the hash
 *
 * Returns the SHA256 hash of the provided string
 * as a hex string.
 *
 * @param ctx The hashing context
 */
void sha256_update(sha256_ctx ctx, const uint8_t *message, unsigned length);

/**
 * @brief Finalizes the hash
 *
 * Returns the SHA256 hash of the provided string
 * as a hex string.
 *
 * @param ctx The hashing context
 */
void sha256_final(sha256_ctx ctx);

/**
 * @brief Retrieves the hash
 *
 * Retrieves the generated hash into the provided buffer.
 * Make sure that the size of the buffer is at least
 * SHA256_HASH_LENGTH.
 *
 * @param ctx The hashing context
 * @param hash The buffer for the hash
  */
void sha256_hash(const sha256_ctx ctx, uint8_t *hash);

/**
 * @brief Hashes a string
 *
 * Returns the SHA256 hash of the provided string
 * as a hex string.
 *
 * @param value The string to be hashed
 * @return The hash of the string
 */
char *sha256_string(const char *value);

/**
 * @brief Hashes a file
 *
 * Returns the SHA256 hash of the contents of the
 * provided filae as a hex string.
 *
 * @param file_path The path of file to be hashed
 * @return The hash of the file contents
 */
char *sha256_file(const char *file_path);
