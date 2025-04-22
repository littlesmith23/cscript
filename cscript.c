/**
 * @file cscript.c
 * @author Stefan Kleinschmiodt
 * @date 13. Nov 2024
 * @brief Contains the main function of the cscript tool.
 *
 * cscript is a simple tool that enables the user to run c source code like a script file.
 * Just add a proper shebang line, usually #!/usr/local/script as the first line of the c-file.
 * If necessary, add a second line starting with #gcc to provide additional command line arguments
 * to gcc.
 * Then just make the c-file executable and you can run it from the command line like any script file.
 * When a c-script is called for the first time, cscript will compile the c-code with gcc and will store the output in
 * a cache directory: ~/.cscript/cache/{hash of c file}/{c file}.bin. It also saves the hash of the c script so it will
 * do a new compilation only when the c source has changed.
 * Only one file source can be used, but libraries can be linked through the command line arguments provided in the
 * aforementioned #gcc line.
 *
 * Example:
 * A c-script that generates a time based uuid using libuui.d
 * @code
 * #!/usr/local/script
 * #gcc -luuid
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <uuid/uuid.h>
 *
 * int main() {
 *     uuid_t uuid;
 *     char uuid_str[37];
 *     uuid_generate_time_safe(uuid);
 *     uuid_unparse_lower(uuid, uuid_str);
 *     printf("generate uuid=%s\n", uuid_str);
 *     return 0;
 * }
 * @endcode
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "cache.h"
#include "script_file.h"

/**
 * @brief Entry point of cscript.
 *
 * When script has been called through the shell's shebang mechanism, it will
 * receive the path to the script file as @p argv[1] and the command line parameters
 * passed to the script file as @p argv[2] and following.
 * If the script file has been called with the argument --cscriptclear, cscript will
 * delete the cache files for the specific script.
 * If cscript has been called directly with the argument --cscriptclear, script will
 * delete all the cache files of all scripts run by the current user.
 * @param argc The number of arguments provided.
 * @param argv array of strings containing the arguments.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(const int argc, char *argv[]) {
#if DEBUG == 1
    printf("DBG: Arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("DBG: Arg #%d: %s\n", i, argv[i]);
    }
#endif
    //Check arguments
    if (argc < 2) {
        fprintf(stderr, "cscript called without any argument./n");
        exit(EXIT_FAILURE);
    }
    //Check if complete cache clear is due
    if (strcmp(argv[1], "--cscriptclear") == 0) {
        cache_clear();
        exit(EXIT_SUCCESS);
    }
    //argv[1] should contain the script file path
    sf_handle sf = script_file_open(argv[1]);
#if DEBUG == 1
    printf("DBG: initial script_file:\n");
    script_file_dump(sf);
#endif
    //Check if script cache clear is due
    if (argc > 2 && strcmp(argv[2], "--cscriptclear") == 0) {
        cache_clear_single(sf);
        exit(EXIT_SUCCESS);
    }

    //Check if there is a current build available
    if (!cache_check(sf)) {
        //If not, compile the script file
        script_file_compile(sf);
        //and update the hash in the cacha
        cache_update(sf);
    }
#if DEBUG == 1
    printf("DBG: after cache_check script_file:\n");
    script_file_dump(sf);
#endif
    //Execute the executable
    script_file_execute(sf, argc, argv);

    return 0;
}
