/* Licensed under GPL V2 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define NR_MAPS 1024 /* 100GB * 1024 = 100TB VA space */
#define SIZE	100 * 1024 * 1024 * 1024UL /* 100GB */

void memory_operations(void)
{
	char *ptr[NR_MAPS];
	unsigned long count;

	for (count = 0; count < NR_MAPS; count++) {
		ptr[count] = (char *)mmap((void *)0, SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
		if (ptr[count] == (char *)-1) {
			printf("ptr[%d] = %lx\n", count, ptr[count]);
			perror("");
		}
	}

	for (count = 0; count < NR_MAPS; count++)
		munmap(ptr[count], SIZE);

}

int main(int argc, char *argv[])
{
	unsigned long procs;
	int pid, threads, status;
	char *ptr;

	if ((argc <= 1) || (argc > 2)) {
		printf("%s <nr_child_procs>\n", argv[0]);
		exit(-1);
	}

	procs = atol(argv[1]);
	threads = procs;
	do {
		pid = fork();
		if (pid == -1) {
			perror("fork() failed");
			exit(-1);
		} else if (!pid) {
			memory_operations();
			exit(0);
		}
	} while(--threads);

	while (procs-- && wait(&status));
}
