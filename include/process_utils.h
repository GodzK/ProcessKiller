#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

void list_processes(const char *filter);
int kill_process_by_pid(DWORD pid);

#endif