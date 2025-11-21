#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h> 
#include <stdio.h>
#include <signal.h>
#include <string.h>


int main(void) {
    pid_t child_pid;
    int wait_status;
    long syscall_nr;
    char buffer[128]; // local_a0: Shellcode'un yazılacağı yer

    // Buffer temizleme (O garip döngünün yaptığı iş)
    memset(buffer, 0, 128);

    child_pid = fork();

    if (child_pid == 0) {
        prctl(PR_SET_PDEATHSIG, SIGHUP); 
        
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        
        puts("Give me some shellcode, k");
        
        // ZAFİYET BURADA: gets() sınır kontrolü yapmaz.
        // 128 byte'lık buffer'a sınırsız veri yazarak Return Address'i ezebilirsin.
        gets(buffer); 
    }
    
    else {
        do {
            wait(&wait_status);

            if (WIFEXITED(wait_status) || WIFSIGNALED(wait_status)) {
                puts("child is exiting...");
                return 0;
            }

            syscall_nr = ptrace(PTRACE_PEEKUSER, child_pid, 44, 0);

            // Eğer child "execve" (Syscall 11) yapmaya çalışıyorsa yakala!
            // Syscall 11 = sys_execve (/bin/sh çalıştırmak için genelde bu kullanılır)
        } while (syscall_nr != 11); // 0xb = 11

        // Yasaklı syscall yakalandı!
        puts("no exec() for you");
        kill(child_pid, SIGKILL);
    }
    
    return 0;
}