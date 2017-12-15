/* Licensed under GPL V2 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#define MAX_NAME 30
 
void memory_operations(unsigned long size)
{
	char *ptr, filename[MAX_NAME], mypid[10];
	int fd;

	memset(filename, 0, sizeof(filename));
	strcpy(filename, "test");
	sprintf(mypid, "%d", getpid());
	strcat(filename, mypid);
	printf("filename %s\n", filename);

	fd = open(filename, O_CREAT | O_RDWR, S_IRWXU | S_IRWXO | S_IRWXG);
	 if (fd == -1) {
		perror("open() failed");
		exit(1);
	}

	if (ftruncate(fd, size)) {
		perror("ftruncate() failed");
		exit(1);
	}

	ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE , fd, 0);
	if (ptr == (char *)-1) {
		printf("ptr = %lx\n", ptr);
		perror("");
	}
	memset(ptr, 0, size);
	munmap(ptr, size);
	close(fd);
	unlink(filename);
}

int main(int argc, char *argv[])
{
	unsigned long size, procs;
	int pid, threads, status;
	char *ptr;

	if ((argc <= 1) || (argc >3)) {
		printf("%s <memory_in_GB> <nr_child_procs>\n", argv[0]);
		exit(-1);
	}

	size = ((long)atol(argv[1]) * 1024 * 1024 * 1024);
	procs = atol(argv[2]);
	threads = procs;
	do {
		pid = fork();
		if (pid == -1) {
			perror("fork() failed");
			exit(-1);
		} else if (!pid) {
			memory_operations(size);
			exit(0);
		}
	} while(--threads);

	while (procs-- && wait(&status));
}
