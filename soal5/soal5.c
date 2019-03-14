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

void logthefile(int* iter, char** dir)
{   
    chdir("/");
    FILE* fdsyslog;
    FILE* fdout;
    fdsyslog = fopen("var/log/syslog","r");
    printf("%d\n",*iter);

    char buff[20];

    if((*iter - 1)%30 == 0){
        memset(buff,0,sizeof(buff));

        time_t now = time(NULL);
        strftime(buff, 20, "%d:%m:%Y-%H:%M", localtime(&now));

        strcpy(*dir,"home/duhbuntu/log/");
        strcat(*dir,buff);
        
        mkdir(*dir,0777);
        // printf("disini buat folder\n");
    }

    char* angka = malloc(1000000);
    snprintf(angka, sizeof angka, "%d", *iter); 

    char* nama = malloc(1000000);
    strcpy(nama,*dir);
    strcat(nama,"/log");
    strcat(nama,angka);
    strcat(nama,".log");

    // printf("%s\n%s\n",*dir,nama);

    fdout = fopen(nama,"w+");
    char isi;
    while((isi = fgetc(fdsyslog)) != EOF){
        fputc(isi, fdout);
    }
    fclose(fdsyslog);
    fclose(fdout);

    (*iter)++;
}

int main()
{
    int iter = 1;
    char* dir = malloc(1000000);
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
        logthefile(&iter,&dir);
        sleep(60);
    }
    exit(EXIT_SUCCESS);
}