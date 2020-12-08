#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <sys/wait.h>

int 
main(int argc, char **argv)
{
    int fd2[2];
    pipe(fd2);
    write(fd2[1], "1", 1);
    if (fork() == 0){
        int fd;
        fd = open("/dev/null",O_WRONLY);
        dup2( fd, 1);
        dup2( fd, 2);
        execvp(argv[1], argv+1);
        char buf[1];
        read(fd2[0], buf, 1);
        write(fd2[1], "0", 1);
        return 1;
    } 
    int status;
    wait(&status);
    if (WIFEXITED(status) != 0){
        write(1, "No", 2);
        close(fd2[0]);
        close(fd2[1]);
        return 1;
    }
    //now we want to check if there are problems
    char buf[1];
    read(fd2[0], buf, 1);
    if (buf[0] != '0'){
        write(1, "Yes", 3);
        close(fd2[0]);
        close(fd2[1]);
        return 0; 
    } else {
        write(1, "No", 2);
        close(fd2[0]);
        close(fd2[1]);
        return 1;
    }
	return 0;
}
