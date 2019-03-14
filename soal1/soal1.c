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

int checkpng(char* string)
{
  return !strcmp(string+(strlen(string)-4),".png") ? 1 : 0;
}

void checkandrename()
{   
    struct stat st = {0};
    if (stat("modul2", &st) == -1) {
        mkdir("modul2", 0777);
        mkdir("modul2/gambar", 0777);
    }

    char namafile[100];
    memset(namafile,'\0',sizeof(namafile));
    DIR* dir;
    struct dirent* rent;
    
    dir = opendir("sisop/dummy-modul2");
    if(dir==NULL){
        perror("Error: ");
        return;
    }
    else{
        // rent = readdir(dir);
        while((rent=readdir(dir))!=NULL){
            // rent=readdir(dir);
            printf("%s\n",rent->d_name);
            if(strlen(rent->d_name)<4){continue;}
            strcpy(namafile,rent->d_name);
            if(checkpng(namafile)){
                // printf("%s\n",namafile);
                char* temp = malloc((strlen(namafile))+10);
                char* from = malloc((strlen(namafile))+50);
                char* dest = malloc((strlen(namafile))+50);
                
                strcpy(from,"sisop/dummy-modul2/"); strcat(from,namafile);
                strncpy(temp,namafile,strlen(namafile)-4);
                strcpy(dest,"modul2/gambar/"); strcat(temp,"_grey.png"); strcat(dest,temp);
                printf("%s\n%s\n",from,dest);
                if(rename(from,dest)<0) perror("Gagal ");
                memset(namafile,'\0',sizeof(namafile));
                memset(temp,'\0',sizeof(temp));
                memset(from,'\0',sizeof(from));
                memset(dest,'\0',sizeof(dest));
            }
        }
        closedir(dir);
    }
}

int main()
{
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

    if ((chdir("/home/duhbuntu/")) < 0) {
      exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    while(1) {
      checkandrename();
      sleep(2);
    }

    exit(EXIT_SUCCESS); 
}