#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include "../include/process_utils.h"

int main() {
    char filter[256];
    char input[100];
    DWORD pid;

    printf("🔷🔷 ProcessPal – Smart Task Manager 🔷🔷\n");
    printf("Enter part of a process name to filter (or leave empty to show all): ");
    fgets(filter, sizeof(filter), stdin);
    filter[strcspn(filter, "\n")] = 0;

    list_processes(strlen(filter) > 0 ? filter : NULL);

    printf("\nEnter PID to kill (or 0 to exit): ");
    fgets(input, sizeof(input), stdin);

    int valid = 1;
    for (int i = 0; input[i] && input[i] != '\n'; i++) {
        if (!isdigit(input[i])) {
            valid = 0;
            break;
        }
    }

    if (!valid) {
        printf("❌ Invalid PID. Please enter a numeric value.\n");
    } else {
        pid = atoi(input);
        if (pid != 0) {
            kill_process_by_pid(pid);
        } else {
            printf("👋 Exiting...\n");
        }
    }

    return 0;
}