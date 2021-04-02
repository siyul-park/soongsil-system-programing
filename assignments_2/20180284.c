#include "20180284.h"

int32_t main(int32_t argc, char *argv[])
{
	int32_t fd;

	if (argc != 2) {
		printf("Usage: editro <file>\n");
		return 0;
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf("Error %d Unable to open %s\n", fd, argv[1]);
		return 0;
	}

	Elf64_Ehdr ehdr;
	Elf64_Shdr* sh_tbl;
	
	read_elf_header(fd, &ehdr);
	
    if (!is_elf(ehdr)) {
		return 0;
	}
	
	print_elf_header(ehdr);

	sh_tbl = malloc(ehdr.e_shentsize * ehdr.e_shnum);
	if (!sh_tbl) {
		printf("Failed to allocate %d bytes\n", (ehdr.e_shentsize * ehdr.e_shnum));
	}
	print_section_headers(fd, ehdr, sh_tbl);
  change_rodata(fd, ehdr, sh_tbl, "software", "hackers!");
	return 0;

}

