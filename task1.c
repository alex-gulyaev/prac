#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int 
main(int argc, char **argv)
{
    execvp(argv[0], argv);
    write(2, "No", strlen("No"));
	return 0;
}