/**
 * @file script_file_type.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Defines the structure for script information
 *
 * This file defines the structure for script information.
 * A pointer to such a structure is used as the sf_handle.
 */
#pragma once
#include <linux/limits.h>

/**
 * @brief Defines the structure for script information
 *
 * The structure for script information. A pointer to
 * such a structure is used as the sf_handle.
 */
typedef struct sf {
    char file_path[PATH_MAX]; /**< The file path to the script file that has been called. */
    char file_name[PATH_MAX]; /**< The file name of the script file that has been called. */
    char hash[256]; /**< The hash (SHA256) of the script file that has been called. */
    char gcc_args[16284]; /**< The command line arguments for gcc provided in the @#gcc line. */
    char source_path[PATH_MAX]; /**< The path to the temporary source file for compilation. */
    char executable_path[PATH_MAX]; /**<  The path to the compiled executable. */

    int start_line; /**<  The first line of the c-source in the script file. */
} script_file;
