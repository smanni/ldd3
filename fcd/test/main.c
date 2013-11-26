#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define SIZE 40

int main()
{
	int fd = 0, ret = 0;
	char buff[SIZE + 1];
	char c ='x';
	
	if((fd = open("/dev/fcd", O_RDWR)) < 0)
	{
		printf("Unable to open device driver error %d\n", fd);
		return;
	}
	
	// Read
	ret = read(fd, buff, SIZE);
	buff[SIZE] = '\0';	
	printf("buff: %s ;length: %d bytes\n", buff, ret);
	
	// Write
	ret = write(fd, &c, 1);

	// Read again
	ret = read(fd, buff, SIZE);
	buff[SIZE] = '\0';	
	printf("buff: %s ;length: %d bytes\n", buff, ret);
		
	close(fd);
}
