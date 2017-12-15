objects = fork_unmap fork_thp fork_thp_prot fork_prot fork_vma fork_mem fork_file

all: $(objects)

$(objects): %: %.c
	$(CC) $(CFLAGS) -o $@ $<
clean:
	rm -rf $(objects)


