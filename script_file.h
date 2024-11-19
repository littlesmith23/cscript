/**
 * @file script_file.h
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Contains the script file related function for cscript.
 *
 * Provides a handle to an internal structure managing the information
 * around a c script as well as function to load, manage and execute.
 */
#pragma once

/**
 * @brief A handle to a script file
 */
typedef const void* sf_handle;

/**
 * @brief Opens a script file
 *
 * Opens the script file at @p file_path and loads the information.
 * @param file_path The path to the script file
 * @return A handle to the script file information
 */
sf_handle script_file_open(const char* file_path);

/**
 * @brief Sets the file path for the executable into the structure
 * @param handle A handle to the script file information
 * @param path The path to the executable file
 */
void script_file_set_executable_path(sf_handle handle, const char* path);

/**
 * @brief Compiles the script file
 *
 * Compiles the script file according to the information in the file
 * and writes the executable to the path defined in script_file_set_executable_path().
 *
 * @param handle A handle to the script file information
 */
void script_file_compile(sf_handle handle);

/**
 * @brief Executes the script file
 *
 * Executes the executable compiled from the script file. Provides
 * the arguments argv[2] to argv[argc-1] to the executable (These
 * are the arguments provided to the script on the shell).
 *
 * @param handle A handle to the script file information
 * @param argc The number of arguments provided.
 * @param argv array of strings containing the arguments.
 */
void script_file_execute(sf_handle handle, int argc, char** argv);

/**
 * @brief Dumps the script file
 *
 * Dumps the information on the script file.
 *
 * @param handle A handle to the script file information
 */
void script_file_dump(sf_handle handle);
