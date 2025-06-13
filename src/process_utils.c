#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/process_utils.h"
#include "../include/logger.h"
char *stristr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    for (; *haystack; ++haystack) {
        if (tolower(*haystack) == tolower(*needle)) {
            const char *h = haystack, *n = needle;
            while (*h && *n && tolower(*h) == tolower(*n)) {
                ++h;
                ++n;
            }
            if (!*n) return (char *)haystack;
        }
    }
    return NULL;
}
void list_processes(const char *filter) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        printf("Failed to create process snapshot. Error: %lu\n", GetLastError());
        return;
    }
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    printf("PID\tMemory (KB)\tProcess Name\n");
    printf("--------------------------------------------------\n");

    if (Process32First(hSnapshot, &pe)) {
        do {
            if (!filter || stristr(pe.szExeFile, filter)) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                printf("%u\t%lu\t\t%s\n", pe.th32ProcessID, (unsigned long)memUsage, pe.szExeFile);
            }
        } while (Process32Next(hSnapshot, &pe));
    } else {
        printf("❌ Failed to enumerate processes. Error: %lu\n", GetLastError());
    }
    CloseHandle(hSnapshot);
}

int kill_process_by_pid(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        printf("❌ Unable to open process with PID %lu. Error: %lu\n", (unsigned long)pid, GetLastError());
        return 0;
    }

    TCHAR name[MAX_PATH] = TEXT("<unknown>");
    DWORD size = sizeof(name) / sizeof(TCHAR);
    GetModuleBaseName(hProcess, NULL, name, size);

    if (TerminateProcess(hProcess, 0)) {
        printf("✅ Successfully killed PID %lu (%ls)\n", (unsigned long)pid, name);
        log_killed_process(name, pid);
        CloseHandle(hProcess);
        return 1;
    } else {
        printf("❌ Failed to kill PID %lu. Error: %lu\n", (unsigned long)pid, GetLastError());
        CloseHandle(hProcess);
        return 0;
    }
}