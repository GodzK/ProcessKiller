#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <direct.h>
#include "../include/logger.h"

void log_killed_process(const char *processName, DWORD pid) {
    _mkdir("logs");

    FILE *f = fopen("logs/killed_log.txt", "a");
    if (f == NULL) {
        printf("⚠️ Warning: Could not open log file.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] Killed %s (PID: %lu)\n",
            lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,
            lt->tm_hour, lt->tm_min, lt->tm_sec,
            processName, (unsigned long)pid);

    fflush(f);
    fclose(f);
}