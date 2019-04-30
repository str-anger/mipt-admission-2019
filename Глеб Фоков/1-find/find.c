#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <stdio.h>

#include <stdbool.h>

#include <string.h>

#include "find.h"
#include "kmp.h"

enum isbinary_t {
	ISBINARY_ERR,
	ISBINARY_TRUE,
	ISBINARY_FALSE
};
static enum isbinary_t isbinary (int fd, char* buf, int block_size);

int find_files_by_content(const char* query, const char* dirpath, int block_size,
						  bool exclude_binary) {

	char* buf = calloc(block_size, 1);
	if (NULL == buf) {
		perror("calloc(block_size, 1)");
		return -1;
	}

	Query q;
	if (-1 == QueryInit(query, strlen(query), &q)) {
		fprintf(stderr, "error: QueryInit\n");
		goto err;
	}

	int examine_file_content(const char* fpath, const struct stat* sb,
                             int typeflag, struct FTW* ftwbuf) {

		if (FTW_F != typeflag) {
			if (FTW_NS == typeflag) {
				printf("cannot access %s\n", fpath);
			}

			return 0;
		}

		int fd = open(&(fpath[ftwbuf->base]), O_RDONLY);
		if (-1 == fd) {
			printf("cannot access %s\n", fpath); // TODO: deduplicate?
			return 0;
		}

		enum get_next_res get_next(int* read_, int* status) {
			*read_ = read(fd, buf, block_size);
			switch (*read_) {
			case 0:
				return GET_NEXT_EOF;
			break;
			case -1:
				if (NULL != status) {
					*status = errno;
				}
				return GET_NEXT_ERROR;
			break;
			default:
				return GET_NEXT_SUCCESS;
			}
		}

		if (exclude_binary) {
			switch (isbinary(fd, buf, block_size)) {
			case ISBINARY_TRUE:
				goto end;
			break;
			case ISBINARY_ERR:
				goto cannot_access;
			break;
			}
		}
		
		switch (kmp_grep(q, buf, get_next, NULL)) {
		case SEARCH_FOUND:
			printf("%s\n", fpath);
			goto end;
		break;
		case SEARCH_NOT_FOUND:
			goto end;
		default:
			goto cannot_access;
		break;
		}

		cannot_access:
			printf("cannot access %s\n", fpath);
		end:
			close(fd);
			return 0;
	}

	// TODO: choose `snopenfd` (limits/10 ?)
	if (-1 == nftw(dirpath, examine_file_content, 20 /*snopenfd*/, FTW_CHDIR)) {
		perror("nftw()");
		goto err;
	}

	free(buf);
	QueryDestroy(&q);
	return 0;

	err:
		free(buf);
		QueryDestroy(&q);
		return -1;
}

static enum isbinary_t isbinary (int fd, char* buf, int block_size) {
	int read_ = read(fd, buf, block_size);
	if (0 >= read) {
		return ISBINARY_ERR;
	}

	if (-1 == lseek(fd, 0, SEEK_SET)) {
		return ISBINARY_ERR;
	}

	for (int i = 0; i < read_; i++) {
		if (0 == buf[i]) {
			return ISBINARY_TRUE;
		}
	}

	return ISBINARY_FALSE;
}