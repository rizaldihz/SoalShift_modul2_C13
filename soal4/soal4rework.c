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

void dietyuk()
{
    time_t now;
    double det;
    time(&now);
    struct stat st = {0};
    if (stat("/home/duhbuntu/Documents/makanan/makan_enak.txt", &st) == -1) {
        perror("Gagal ");
        return;
    }
    det = difftime(now,st.st_atime);
    int iterasi = 1;
    char* namafile = malloc(1000000);
    char* urutan = malloc(1000000);
    snprintf(urutan, sizeof urutan, "%d", iterasi);
    struct stat fp = {0};
    strcpy(namafile, "/home/duhbuntu/Documents/makanan/makan_sehat");
    while (det <= 30.0){
        FILE* fd;
        snprintf(urutan, sizeof urutan, "%d", iterasi);
        strcat(namafile,urutan);
        strcat(namafile,".txt");
        if(stat(namafile,&fp) == -1){
            fd = fopen(namafile,"w+");
            fclose(fd);
            break;
        }
        strcpy(namafile, "/home/duhbuntu/Documents/makanan/makan_sehat");
        iterasi++;
    }
        
}

int main(){
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
        dietyuk();
        sleep(5);
    }
    exit(EXIT_SUCCESS);
}