








示例 - 生成 core dump 
```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>

pid_t target_pid;

void generate_core_dump(int signum) {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process
        if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
            perror("ptrace attach");
            exit(1);
        }

        waitpid(target_pid, NULL, 0);

        if (ptrace(PTRACE_CONT, target_pid, NULL, NULL) == -1) {
            perror("ptrace continue");
            exit(1);
        }

        if (ptrace(PTRACE_INTERRUPT, target_pid, NULL, NULL) == -1) {
            perror("ptrace interrupt");
            exit(1);
        }

        waitpid(target_pid, NULL, 0);

        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) == -1) {
            perror("ptrace getregs");
            exit(1);
        }

        long orig_rax = regs.orig_rax;
        regs.orig_rax = SYS_write;
        regs.rdi = 2;  // stderr
        regs.rsi = (long)"Core dump generated\n";
        regs.rdx = 21;  // length of the string

        if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) == -1) {
            perror("ptrace setregs");
            exit(1);
        }

        if (ptrace(PTRACE_SYSCALL, target_pid, NULL, NULL) == -1) {
            perror("ptrace syscall");
            exit(1);
        }

        waitpid(target_pid, NULL, 0);

        if (ptrace(PTRACE_SYSCALL, target_pid, NULL, NULL) == -1) {
            perror("ptrace syscall");
            exit(1);
        }

        waitpid(target_pid, NULL, 0);

        regs.orig_rax = orig_rax;
        if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) == -1) {
            perror("ptrace setregs");
            exit(1);
        }

        if (ptrace(PTRACE_DETACH, target_pid, NULL, NULL) == -1) {
            perror("ptrace detach");
            exit(1);
        }

        exit(0);
    } else if (child_pid > 0) {
        // Parent process
        waitpid(child_pid, NULL, 0);
    } else {
        perror("fork");
    }
}

int main() {
    target_pid = getpid();
    signal(SIGUSR1, generate_core_dump);
    
    printf("Process ID: %d\n", target_pid);
    printf("Send SIGUSR1 to generate a core dump\n");
    
    while(1) {
        sleep(1);
        printf("Still running...\n");
    }
    
    return 0;
}
```

