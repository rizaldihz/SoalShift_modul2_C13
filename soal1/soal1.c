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

void checkandrename()
{   
    struct stat st = {0};
    if (stat("modul2", &st) == -1) {
        mkdir("modul2", 0777);
        mkdir("modul2/gambar", 0777);
    }

    char namafile[100];
    DIR* dir;
    struct dirent* rent;
    
    dir = opendir("sisop/dummy-modul2");
    if(dir==NULL){
        perror("Error: ");
        return;
    }
    else{
        while((rent=readdir(dir))!=NULL){
            strcpy(namafile,rent->d_name);
            if(strstr(namafile,".png")){
                // printf("%s\n",namafile);
                char* temp = malloc((strlen(namafile))+10);
                char* from = malloc((strlen(namafile))+50);
                char* dest = malloc((strlen(namafile))+50);
                if(strstr(namafile,"_grey"))
                {
                    strcpy(from,"sisop/dummy-modul2/"); strcat(from,namafile);
                    strcpy(dest,"modul2/gambar/"); strcat(dest,namafile);
                    printf("%s %s\n",from,dest);
                    int hasil = rename(from,dest);
                    if(hasil<0) perror("Gagal ");
                }
                else
                {
                    strcpy(from,"sisop/dummy-modul2/"); strcat(from,namafile);
                    strncpy(temp,namafile,strlen(namafile)-4);
                    strcpy(dest,"modul2/gambar/"); strcat(temp,"_grey.png"); strcat(dest,temp);
                    printf("%s %s\n",from,dest);
                    int hasil = rename(from,dest);
                    if(hasil<0) perror("Gagal ");
                }
                free(temp);
                free(from);
                free(dest);
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