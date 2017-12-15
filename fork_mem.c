/* Licensed under GPL V2 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define NR_MAPS 1024

void memory_operations(unsigned long size)
{
	char *ptr[NR_MAPS];
	unsigned long count, asize = size / NR_MAPS;

	for (count = 0; count < NR_MAPS; count++) {
		ptr[count] = (char *)mmap((void *)0, asize, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
		if (ptr[count] == (char *)-1) {
			printf("ptr[%d] = %lx\n", count, ptr[count]);
			perror("");
		}
	}

	for (count = 0; count < NR_MAPS; count++)
		memset(ptr[count], 0, asize);

	for (count = 0; count < NR_MAPS; count++)
		munmap(ptr[count], asize);
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
