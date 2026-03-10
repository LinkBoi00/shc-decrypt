#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#define LOG_TAG "[MAIN] "

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <script.sh.x> [hook_library.so]\n", argv[0]);
        fprintf(stderr, "  If hook_library.so is not provided, uses ./hooks.so\n");
        return 1;
    }

    char *script_path = argv[1];
    char *lib_name = (argc == 3) ? argv[2] : "./hooks.so";

    // Convert script path to absolute
    char abs_script_path[PATH_MAX];
    if (realpath(script_path, abs_script_path) == NULL) {
        fprintf(stderr, "Error: Cannot resolve script path '%s'\n", script_path);
        perror("realpath");
        return 1;
    }

    // Check if script exists and is executable
    if (access(abs_script_path, F_OK) != 0) {
        fprintf(stderr, "Error: File '%s' not found\n", script_path);
        return 1;
    }

    if (access(abs_script_path, X_OK) != 0) {
        fprintf(stderr, "Error: File '%s' is not executable\n", script_path);
        return 1;
    }

    // Convert library path to absolute
    char abs_lib_path[PATH_MAX];
    if (realpath(lib_name, abs_lib_path) == NULL) {
        fprintf(stderr, "Error: Cannot resolve library path '%s'\n", lib_name);
        perror("realpath");
        return 1;
    }

    // Check if library exists and is readable
    if (access(abs_lib_path, F_OK) != 0) {
        fprintf(stderr, "Error: Hook library '%s' not found\n", lib_name);
        return 1;
    }

    if (access(abs_lib_path, R_OK) != 0) {
        fprintf(stderr, "Error: Hook library '%s' is not readable\n", lib_name);
        return 1;
    }

    printf(LOG_TAG "Script: %s\n", abs_script_path);
    printf(LOG_TAG "Hook library: %s\n", abs_lib_path);

    // Set LD_PRELOAD
    setenv("LD_PRELOAD", abs_lib_path, 1);

    // Execute
    char *args[] = {abs_script_path, NULL};
    execv(abs_script_path, args);

    // If execv returns, there was an error
    perror("execv failed");
    return 1;
}
