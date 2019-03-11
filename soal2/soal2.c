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
#include <pwd.h>
#include <grp.h>
#include <pwd.h>


void checkanddelete()
{   
    

    char namafile[100];
    DIR* dir;
    struct dirent* rent;
    chdir("/home/duhbuntu/sisop/hatiku/");
    dir = opendir(".");
    if(dir==NULL){
        perror("Error: ");
        return;
    }
    else{
        while((rent=readdir(dir))!=NULL){
            strcpy(namafile,rent->d_name);
            // printf("%s\n",namafile);
            if(strcmp(namafile,"elen.ku")==0){
                struct stat st = {0};
                if (stat(namafile, &st)<0) {
                    perror("Gagal ");
                    continue;
                }
                struct passwd *name = getpwuid(st.st_uid);
                struct group *grup = getgrgid(st.st_gid);
                // printf("%s %s\n",name->pw_name,grup->gr_name);
                char mode[]="0777";
                if((strcmp(name->pw_name,"www-data"))==0 && (strcmp(grup->gr_name,"www-data"))==0){
                    if(chmod(namafile,S_IRWXU|S_IRWXG|S_IRWXO)<0){
                        perror("Gagal ");
                    }
                    remove(namafile);
                }
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

    if ((chdir("/")) < 0) {
      exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    while(1) {
      checkanddelete();
      sleep(3);
    }

    exit(EXIT_SUCCESS); 
}