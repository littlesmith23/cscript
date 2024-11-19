/**
* @file script_file.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Contains the cache related function for cscript.
 *
 * Provides functionality to manage the cache..
 */

#pragma once

#include "script_file.h"

/**
 * @brief Checks if a cache for a given script file exists.
 *
 * Checks if a cache for a given script file exists.  If yes, it also
 * checks if the script file has changed since the last execution.
 * Returns true if the cache exists and the file has not changed.
 * The cache directory ({~/.cscript/cache/{hash-of-filepath}) will be
 * created if it does not exist yet.
 *
 * @param handle The handle of the script information
 * @return true if the file is cached and has not changed.
 */
bool cache_check(sf_handle handle);

/**
 * @brief Updates the cache for script file
 *
 * Used when a script file has changed or is being executed for the first time.
 * Updates tha hash file that determines if the script file has changed.
 *
 * @param handle The handle of the script information
 */
void cache_update(sf_handle handle);

/**
 * @brief Clears the complete cache
 *
 * Deletes the complete cache for the current user
 */
void cache_clear();

/**
 * @brief Clears the cache for the script file
 *
  * @param handle The handle of the script information
 */
void cache_clear_single(sf_handle handle);