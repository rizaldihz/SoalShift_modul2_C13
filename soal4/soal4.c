#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>

void dietyuk(int* ke)
{
    time_t now;
    double det;
    char* namafile = malloc(1000000);
    char* urutan = malloc(1000000);
    struct stat st = {0};
    time(&now);
    if (stat("/home/duhbuntu/Documents/makanan/makan_enak.txt", &st) == -1) {
        perror("Gagal ");
        return;
    }
    det = difftime(now,st.st_atime);
    snprintf(urutan, sizeof urutan, "%d", *ke);
    strcpy(namafile, "/home/duhbuntu/Documents/makanan/makan_sehat");
    if (det <= 30.0){
        FILE* fd;
        strcat(namafile,urutan);
        strcat(namafile,".txt");
        fd = fopen(namafile,"w+");
        fclose(fd);
        (*ke)++;
    }
        
}

int main(){
    int sekarang = 1;
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
      exit(EXIT_FAILURE);
    }

    if (pid > 0) {
      exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0) {
      exit(EXIT_FAILURE);
    }

    if ((chdir("/")) < 0) {
      exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    while(1){
        dietyuk(&sekarang);
        sleep(5);
    }
    exit(EXIT_SUCCESS);
}