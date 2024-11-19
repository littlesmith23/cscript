/**
* @file tools.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Contains declarations for convenience functions.
 *
 * Convenience function to improve readability and ease development
 */
#pragma once
#include <dirent.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

/**
 * @brief Creates a directory path on the filesystem
 *
 * Creates a directory path on the filesystem. Creates every
 * directory on the given path that doesn't exist yet.
 * The provided mode is only applied to newly created dirctories.
 * To change the mode of existing directories, use chmod.
 * Returns 0 if the complete path has been created or already exists.
 *
 * @param path The path to create
 * @param mode The mode of the newly created directories
 * @return 0 on success, otherwise errno contains the last error
 */
int mkdir_p(const char *path, const mode_t mode);
/**
 * @brief Checks if a directory exists
 *
 * @param path The path to check
 * @return true if the path exists and is a directory
 */
bool dir_exists(const char *path);
/**
 * @brief Checks if a file exists
 *
 * @param path The path to check
 * @return true if the path exists and is a file
 */
bool file_exists(const char *path);
/**
 * @brief Get the absolute path
 *
 * Returns the canonical absolute path for the given path
 * The pointer to the buffer containing the path will be overwritten
 * on a subsequent use of this function. Not thread safe!
 *
 * @param path The path to check
 * @return true if the path exists and is a file
 */
const char * get_real_path(const char *path);
/**
 * @brief Get the filename from a path
 *
 * Returns a pointer to the start of the filename
 * in the given path.
 *
 * @param path The path to check
 * @return Pointer to the filename
 */
const char* get_file_name(const char* filePath);
/**
 * @brief Get the temporary directory
 *
 * Returns the path to the temporary directory. Tries to get
 * the value from the environment, otherwise defaults to /tmp/.
 * The pointer to the buffer containing the path will be overwritten
 * on a subsequent use of this function. Not thread safe!
 *
 * @return the path to the temporary directory
 */
const char* get_temp_dir();
/**
 * @brief Allocates a string
 *
 * Allocates a string buffer of the given size and
 * sets the *string to the address. The buffer must
 * be freed either using free_string() or free().
 *
 * @param string Pointer to a char pointer
 * @param size The desired size
 */
void alloc_string(char ** string, size_t size);
/**
 * @brief Frees a string
 *
 * Deallocates the string buffer pointed to by *string
 * and sets *string to nullptr.
 *
 * @param string Pointer to a char pointer
 */
void free_string(char ** string);
/**
 * @brief Get a random guid
 *
 * Returns a string with a rondom guid in hex format.
 * The pointer to the buffer containing the guid will be overwritten
 * on a subsequent use of this function. Not thread safe!
 *
 * @return ta guid in hex format
 */
const char* make_guid();

/**
 * @brief Reverses the byte order of 4 byte integer
 *
 * Returns the provided 4 byte integer with reversed byte order.
 *
 * @param word The value to reverse
 * @return The reversed value
 */
uint32_t swapByteOrder(uint32_t word);
/**
 * @brief Shifts the bits of 4 byte integer to the left
 *
 * Returns the provided 4 byte integer shifted by shift bytes to the left.
 *
 * @param word The value to shift
 * @param shift The number of bits to shift by
 * @return The shifted value
 */
uint32_t shiftLeft(uint32_t word, uint32_t shift);
/**
 * @brief Shifts the bits of 4 byte integer to the right
 *
 * Returns the provided 4 byte integer shifted by shift bytes to the right.
 *
 * @param word The value to shift
 * @param shift The number of bits to shift by
 * @return The shifted value
 */
uint32_t shiftRight(uint32_t word, uint32_t shift);
/**
 * @brief Rotates the bits of 4 byte integer to the left
 *
 * Returns the provided 4 byte integer rotated by shift bytes to the left.
 *
 * @param word The value to rotate
 * @param shift The number of bits to rotate by
 * @return The rotated value
 */
uint32_t rotateLeft(uint32_t word, uint32_t shift);
/**
 * @brief Rotates the bits of 4 byte integer to the right
 *
 * Returns the provided 4 byte integer rotated by shift bytes to the right.
 *
 * @param word The value to rotate
 * @param shift The number of bits to rotate by
 * @return The rotated value
 */
uint32_t rotateRight(uint32_t word, uint32_t shift);
