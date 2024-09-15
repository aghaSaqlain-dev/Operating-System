#include <stdio.h>   // For printf
#include <stdlib.h>  // For exit
#include <unistd.h>  // For fork, pipe, read, write, close, and wait
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For wait

#define stdBufSize 1

int main(){

int p1[2], p2[2]; //file descriptor for pipes  p1[0],p2[0] -> read and p1[1],p2[1] -> write end
char buf[stdBufSize]; // buffer for holding the data

pipe(p1);pipe(p2);
//creating two pipes: 
//////////p1 for parent to child
/////////p2 for child to parent

if(fork() == 0){///////////child process
	////here child reads data from pipe which was sent by parent. parent(sender) and child(receiver)
	////so we close the unused ends which are parent receiving end and child's sending end
	close(p1[1]);//closes write
	close(p2[0]);//closes read
	
	//read data from first pipe
	read(p1[0], buf, stdBufSize);
	printf( "child got: %c\n",buf[0]);
	
	//send data back through second pipe
	//we can send the same data but here we will update
	buf[0] = 'S'; // buf size is 1 so we are exchanging character only
	write(p2[1], buf, stdBufSize); // write stdBufSize bytes into fd from buf
	close(p1[0]);//closes read end
	close(p2[1]);//closes write end
	exit(0);
}else{//parent process
	//closing unused ends
	close(p1[0]);
	close(p2[1]);
	
	//sending data to child process
	buf[0] = 'A';
	write(p1[1], buf, stdBufSize);

	//reading data from second pipe
	read(p2[0], buf, stdBufSize);
	printf( "parent got: %c\n" , buf[0]);
	
	close(p1[1]);//close write end
	close(p2[0]);//close read end
	wait(NULL); // waiting for the child

}


return 0;
}
