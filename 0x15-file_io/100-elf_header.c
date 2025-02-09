#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

#define BUF_SIZE 128

/**
 * print_elf_header - program that displays information contained in ELF header
 * *header: pointer to a structure of type Elf64_Ehdr
 */

void print_elf_header(const Elf64_Ehdr *header)
{
	printf("Magic:   ");
	for (int i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x ", header->e_ident[i]);
	}

	printf("\n");
	printf("Class:
			%s\n", header->e_ident[EI_CLASS] == ELFCLASS64 ? "ELF64" : "unknown");
	printf("Data:
			%s\n", header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" : "unknown");
	printf("Version:
			%d (current)\n", header->e_ident[EI_VERSION]);
	printf("OS/ABI: ");
	
	switch (header->e_ident[EI_OSABI])
	{
		case ELFOSABI_SYSV:
			printf("UNIX System V ABI\n");
			break;
		case ELFOSABI_LINUX:
			printf("Linux ABI\n");
			break;
		default:
			printf("unknown\n");
	}

	printf("ABI Version: %d\n", header->e_ident[EI_ABIVERSION]);
	printf("Type:");
	
	switch (header->e_type)
	{
		case ET_NONE:
			printf("NONE (Unknown type)\n");
			break;
		case ET_REL:
			printf("REL (Relocatable file)\n");
			break;
		case ET_EXEC:
			printf("EXEC (Executable file)\n");
			break;
		case ET_DYN:
			printf("DYN (Shared object file)\n");
			break;
		case ET_CORE:
			printf("CORE (Core file)\n");
			break;
		default:
			printf("unknown\n");
	}

	printf("Entry point address: 0x%lx\n", header->e_entry);
}

/**
 * main - function calls other functions to perform the main task of the program
 * @argc: number of arguments
 * @argv: string of arguments
 * Return: 0 on success
 */

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		exit(98);
	}

	int fd = open(argv[1], O_RDONLY);

	if (fd == -1)
	{
		fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
		exit(98);
	}

	Elf64_Ehdr header;

	if (read(fd, &header, sizeof(header)) != sizeof(header))
	{
		fprintf(stderr, "Error: Can't read ELF header from file %s\n", argv[1]);
		close(fd);
		exit(98);
	}

	if (header.e_ident[EI_MAG0] != ELFMAG0 || header.e_ident[EI_MAG1] != ELFMAG1 || header.e_ident[EI_MAG2] != ELFMAG2 || header.e_ident[EI_MAG3] != ELFMAG3)
	{
		fprintf(stderr, "Error: File %s is not an ELF file\n", argv[1]);
		close(fd);
		exit(98);
	}

	print_elf_header(&header);

	close(fd);
	return 0;
}
