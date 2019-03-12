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

int main()
{
    chdir("/home/duhbuntu/sisop/");
    pid_t child1;
    pid_t child2;
    pid_t child3;
    int p1;
    int ag[4];
    char temp[100000];

    child1 = fork();

    if(child1 == 0){
        struct stat st = {0};
        if(stat("campur2",&st)!=0) execlp("unzip","unzip","campur2.zip", NULL);
        exit(1);
    }else{
        int p2;
        while(wait(&p1)>0);

        pipe(ag);
        pipe(ag + 2);
       
        child2 = fork();
        if(child2==0){
            close(ag[2]);
            close(ag[3]);
            close(ag[0]);
            dup2(ag[1],STDOUT_FILENO);
            close(ag[1]);
            execlp("ls","ls","campur2",NULL);
        }else{
            wait(NULL);
            child3 = fork();
            if(child3==0){
                close(ag[2]);
                dup2(ag[3],STDOUT_FILENO);
                close(ag[3]);
                
                close(ag[1]);
                dup2(ag[0],STDIN_FILENO);
                close(ag[0]);
                
                execlp("grep","grep",".txt$",NULL);
            }else{
                close(ag[3]);
                close(ag[0]);
                close(ag[1]);
                int byte = read(ag[2],temp,sizeof(temp));
                FILE* out;
                out = fopen("data.txt","w+");
                fputs(temp,out);
                fclose(out);
                close(ag[2]);
            }
            
        }
    }
}