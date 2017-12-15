/* Licensed under GPL V2 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

void memory_operations(unsigned long size)
{
	char *ptr;

	ptr = (char *)mmap((void *)0, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
	if (ptr == (char *)-1) {
		printf("ptr = %lx\n", ptr);
		perror("");
	}
	madvise(ptr, size, MADV_HUGEPAGE);
	memset(ptr, 0, size);
	munmap(ptr, size);
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
