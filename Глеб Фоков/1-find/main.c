#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "find.h"

#define CHECK_AND_PRINT_USAGE(v) do {if (v) { print_usage(argv[0]); return -1; }} while(0);
#define PRINT_USAGE() do { print_usage(argv[0]); return -1; } while(0);

void print_usage(const char* name) {
	const char* usage_msg = \
	"Usage: %s Path [--exclude-binaries] [--block-size Size] Query\n"
	"\n"
	"    Recurcively searches Path for the files containing\n"
	"    Query. Asynchronuosly prints the paths to them. The\n"
	"    files are to be read in portions of Size bytes.\n"
	"\n"
	"Options:\n"
	"    --exclude-binaries\tdo not print the files containing\n"
	"                      \tzero byte in the first block\n"
	"\n";
	//"\t--block-size\t\tset block_size - the number of bytes in portions the files to be read\n";

	fprintf(stderr, usage_msg, name);
}

enum option {EXCLUDE_BINARIES, BLOCK_SIZE};

enum option parse_option(const char* opt) {
	if (0 == strcmp("--exclude-binaries", opt)) {
		return EXCLUDE_BINARIES;
	}
	
	if (0 == strcmp("--block-size", opt)) {
		return BLOCK_SIZE;
	}
}

int main(int argc, char** argv) {

	bool exclude_binaries = false;
	int block_size = 4096;

	CHECK_AND_PRINT_USAGE(argc < 3);
	
	const char* path = argv[1];
	const char* query = argv[argc-1];

	for (int i = 2; i < argc-1 && argv[i][0] == '-'; i++) {
		switch (parse_option(argv[i])) {
		case EXCLUDE_BINARIES:
			exclude_binaries = true;
		break;
		case BLOCK_SIZE:
			errno = 0;
			block_size = strtol(argv[i+1], NULL, 10);
			CHECK_AND_PRINT_USAGE(0 != errno || i+1 == argc);
		break;
		default:
			PRINT_USAGE();
		}
	}

	if (-1 == find_files_by_content(query, path, block_size, exclude_binaries)) {
		fprintf(stderr, "error: find_files_by_content()\n");
		return -1;
	}

	return 0;
}