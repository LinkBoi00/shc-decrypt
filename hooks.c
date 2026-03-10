#define _GNU_SOURCE
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdarg.h>

// Taken from sys/prctl.h to avoid conflicts
#define PR_SET_DUMPABLE 4
#define PR_SET_PTRACER 0x59616d61

// Hook prctl to bypass HARDENING checks
int prctl(int option, ...) {
    //printf("[HOOK] prctl() called with option: 0x%x\n", option);

    if (option == PR_SET_DUMPABLE) {
        printf("[HOOK] prctl(): PR_SET_DUMPABLE bypassed\n");
    } else if (option == PR_SET_PTRACER) {
        printf("[HOOK] prctl(): PR_SET_PTRACER bypassed\n");
    }

    // Always return success without actually doing anything
    return 0;
}

// Hook ptrace to bypass UNTRACEABLE checks
long ptrace(int request, pid_t pid, void *addr, void *data) {
    printf("[HOOK] ptrace called with request=%d\n", request);
    return 0;  // Always return sucess
}

// Hook strcmp to bypass more HARDENING checks
int strcmp(const char *s1, const char *s2) {
    //printf("[HOOK] strcmp called with args: '%s', '%s'\n", s1, s2);

    // Make any parent name match "bash". Trick shc into thinking we are in a normal bash shell.
    if (strstr(s2, "bash")) {
        printf("[HOOK] strcmp(): Called with bash arg: '%s', '%s'\n", s1, s2);
        return 0;
    }

    // Call original strcmp, don't mess with other string comparisons.
    int (*original_strcmp)(const char*, const char*);
    original_strcmp = dlsym(RTLD_NEXT, "strcmp");
    return original_strcmp(s1, s2);
}

// Hook system to get raw decrypted bash script before it executes
int system(const char *command) {
    int (*real_system)(const char*);
    real_system = dlsym(RTLD_NEXT, "system");

    // If command is NULL, ignore it
    if (!command)
        return real_system(command);

    // If command if for building shc internal libs, ignore it
    if (!strcmp(command, "cc -Wall -fpic -shared -o /tmp/shc_x.so /tmp/shc_x.c -ldl"))
        return real_system(command);

    // Print command
    printf("[HOOK] system() called (command size: %zu bytes)\n", strlen(command));
    printf("\n========== DECRYPTED SCRIPT START ==========\n");
    printf("%s\n", command);
    printf("=========== DECRYPTED SCRIPT END ===========\n\n");

    // Don't actually execute anything
    return 0;
}
