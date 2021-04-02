#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <elf.h>

bool is_elf(Elf64_Ehdr eh);
void read_elf_header(int32_t fd, Elf64_Ehdr *elf_header);
void print_elf_header(Elf64_Ehdr elf_header);
char *read_section(int32_t fd, Elf64_Shdr sh);
void print_section_headers(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]);

//Implement this function to check whether an input file is an elf file or not.
bool is_elf(Elf64_Ehdr eh) {
	if (!strncmp((char*)eh.e_ident, "\177ELF", 4)) {
		return true;
	} else {
		printf("It's not ELF\n");
		return false;
	}
}

void read_elf_header(int32_t fd, Elf64_Ehdr *elf_header) {
	assert(elf_header != NULL);
	assert(lseek(fd, (off_t)0, SEEK_SET) == (off_t)0);
	assert(read(fd, (void *)elf_header, sizeof(Elf64_Ehdr)) == sizeof(Elf64_Ehdr));
}

void print_elf_header(Elf64_Ehdr elf_header) {
	printf("Storage class\t= ");
	switch (elf_header.e_ident[EI_CLASS]) {
		case ELFCLASS32:
			printf("32-bit objects\n");
			break;

		case ELFCLASS64:
			printf("64-bit objects\n");
			break;

		default:
			printf("Unknwon CLASS\n");
			break;
	}

	printf("Data format\t= ");
	switch (elf_header.e_ident[EI_DATA]) {
		case ELFDATA2LSB:
			printf("2's complement, little endian\n");
			break;

		case ELFDATA2MSB:
			printf("2's complement, big endian\n");
			break;

		default:
			printf("Unknwon Format\n");
			break;
	}

	printf("OS ABI\t\t= ");
	switch (elf_header.e_ident[EI_OSABI]) {
		case ELFOSABI_SYSV:
			printf("UNIX System V ABI\n");
			break;

		case ELFOSABI_HPUX:
			printf("HP-UX\n");
			break;

		case ELFOSABI_NETBSD:
			printf("NetBSD\n");
			break;

		case ELFOSABI_LINUX:
			printf("Linux\n");
			break;

		case ELFOSABI_SOLARIS:
			printf("Sun Solaris\n");
			break;

		case ELFOSABI_AIX:
			printf("IBM AIX\n");
			break;

		case ELFOSABI_IRIX:
			printf("SGI Irix\n");
			break;

		case ELFOSABI_FREEBSD:
			printf("FreeBSD\n");
			break;

		case ELFOSABI_TRU64:
			printf("Compaq TRU64 UNIX\n");
			break;

		case ELFOSABI_MODESTO:
			printf("Novell Modesto\n");
			break;

		case ELFOSABI_OPENBSD:
			printf("OpenBSD\n");
			break;

		case ELFOSABI_ARM_AEABI:
			printf("ARM EABI\n");
			break;

		case ELFOSABI_ARM:
			printf("ARM\n");
			break;

		case ELFOSABI_STANDALONE:
			printf("Standalone (embedded) app\n");
			break;

		default:
			printf("Unknown (0x%x)\n", elf_header.e_ident[EI_OSABI]);
			break;
	}

	printf("Filetype \t= ");
	switch(elf_header.e_type) {
		case ET_NONE:
			printf("N/A (0x0)\n");
			break;

		case ET_REL:
			printf("Relocatable\n");
			break;

		case ET_EXEC:
			printf("Executable\n");
			break;

		case ET_DYN:
			printf("Shared Object\n");
			break;
		default:
			printf("Unknown (0x%x)\n", elf_header.e_type);
			break;
	}

	printf("Machine\t\t= ");
	switch(elf_header.e_machine) {
		case EM_NONE:
			printf("None (0x0)\n");
			break;

		case EM_386:
			printf("INTEL x86 (0x%x)\n", EM_386);
			break;

		case EM_X86_64:
			printf("AMD x86_64 (0x%x)\n", EM_X86_64);
			break;

		case EM_AARCH64:
			printf("AARCH64 (0x%x)\n", EM_AARCH64);
			break;

		default:
			printf(" 0x%x\n", elf_header.e_machine);
			break;
	}
	printf("\n");
}

char *read_section(int32_t fd, Elf64_Shdr sh) {
	char* buff = malloc(sh.sh_size);
	if (!buff) {
		printf("%s:Failed to allocate %ldbytes\n",
				__func__, sh.sh_size);
	}

	assert(buff != NULL);
	assert(lseek(fd, (off_t)sh.sh_offset, SEEK_SET) == (off_t)sh.sh_offset);
	assert(read(fd, (void *)buff, sh.sh_size) == sh.sh_size);

	return buff;
}

void print_section_headers(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
	uint32_t i;
	char* sh_str;

	assert(lseek(fd, (off_t)eh.e_shoff, SEEK_SET) == (off_t)eh.e_shoff);

	for (i = 0; i < eh.e_shnum; i++) {
		assert(read(fd, (void *)&sh_table[i], eh.e_shentsize) == eh.e_shentsize);
	}

	/* section-header string-table */
	sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

	for (i = 0; i < eh.e_shnum; i++) {
		if (!strncmp((sh_str + sh_table[i].sh_name), ".rodata", 7)) {
			printf("%s section info\n", (sh_str + sh_table[i].sh_name));
			printf("    file offset = 0x%08lx\n", sh_table[i].sh_offset);
			printf("           size = 0x%08lx\n", sh_table[i].sh_size);
		}
	}

  free(sh_str);
}

size_t read_rodata_offset(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
  char* sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

  size_t offset = 0;
	for (size_t i = 0; i < eh.e_shnum; i++) {
		if (!strncmp((sh_str + sh_table[i].sh_name), ".rodata", 7)) {
      offset = sh_table[i].sh_offset;
		}
	}

  free(sh_str);

  return offset;
}

size_t read_rodata_size(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
  char* sh_str = read_section(fd, sh_table[eh.e_shstrndx]);

  size_t size = 0;
	for (size_t i = 0; i < eh.e_shnum; i++) {
		if (!strncmp((sh_str + sh_table[i].sh_name), ".rodata", 7)) {
      size = sh_table[i].sh_size;
		}
	}

  free(sh_str);

  return size;
}

char *read_rodata(int32_t fd, Elf64_Ehdr eh, Elf64_Shdr sh_table[]) {
  size_t offset = read_rodata_offset(fd, eh, sh_table);
  size_t size = read_rodata_size(fd, eh, sh_table);

	assert(offset != 0 && size != 0);

  char* buff = malloc(size);

  assert(lseek(fd, (off_t)offset, SEEK_SET) == (off_t)offset);
	assert(read(fd, (void *)buff, size) == size);

	return buff;
}

void write_rodata(
  int32_t fd, 
  Elf64_Ehdr eh, 
  Elf64_Shdr sh_table[],
  char *buff
) {
	size_t offset = read_rodata_offset(fd, eh, sh_table);
  size_t size = read_rodata_size(fd, eh, sh_table);

	assert(offset != 0 && size != 0);

  assert(lseek(fd, (off_t)offset, SEEK_SET) == (off_t)offset);
	assert(write(fd, (void *)buff, size) == size);
}

void str_replace(char *origin, size_t size, char *source, char *target) {
    size_t source_length = strlen(source);
    size_t target_length = strlen(target);

	  assert(source_length == target_length);
    
    size_t correct = 0;
    size_t i;
    for (i = 0; i < size; i++) {
      if (origin[i] == source[correct]) {
        correct++;
      } else {
        correct = 0;
      }
      if (correct == source_length) {
        break;
      }
    }
    
    assert(correct == source_length);

    if (correct == source_length) {
      i -= correct - 1;
      for (size_t j = 0; j < target_length; j++) {
        origin[i + j] = target[j];
      }
    }
}

void change_rodata(
  int32_t fd, 
  Elf64_Ehdr eh, 
  Elf64_Shdr sh_table[],
  char *source,
  char *target
) {
	char *rodata = read_rodata(fd, eh, sh_table);
  size_t size = read_rodata_size(fd, eh, sh_table);

  str_replace(rodata, size, source, target);

  write_rodata(fd, eh, sh_table, rodata);

  free(rodata);
}

