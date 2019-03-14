#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

int main()
{
    char process[1000];
    FILE* hasil = popen("pidof soal5","r");

    fgets(process,1000,hasil);

    pid_t pid = strtoul(process,NULL,10);
    pclose(hasil);
    kill(pid,SIGKILL);

    exit(EXIT_SUCCESS);
}