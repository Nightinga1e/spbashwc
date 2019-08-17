#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define COM_SIZE 10

int main()
{
	char path[] ="/bin/ls";
	pid_t curpid;
	char buf[40];
	char buf2[1000];
	fgets(buf, 38, stdin);
	strtok(buf, "\n");
	char *pch[COM_SIZE];
	int i = 0;
	pch[i] = strtok(buf, " ");
	do
	{
		i++;
		pch[i] = strtok(NULL, " ");
	}while ((pch[i] != NULL) && (i<COM_SIZE));
	i--;
//
	int fd[2];
	int check = 0;
	check = pipe(fd);
	if (check < 0){
		printf("\nCan't create pipe\n");
		return -1;
	}
//
	curpid = fork();
	if (curpid == 0)
	{
		close (fd[0]);
		check = 0;
		check = dup2(fd[1], 1);
		if( check < 0 ){
			printf(" \n CAN'T DUP2 fd1 \n");
			return -1;
		}
		execl(path, pch[0],pch[1], NULL);
	}else{
		wait(0);
		curpid = fork();
		if(curpid == 0){
			close (fd[1]);
			check = 0;
			check = dup2(fd[0], 0);
			if (check < 0){
				printf ("\n CAN'T DUP2 fd0 \n");
				return -1;
			}
			execl("/usr/bin/wc", pch[3], NULL);
			close (fd[0]);
		}
	}
	return 0;
}
