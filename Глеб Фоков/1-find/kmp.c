#include <stdlib.h>
#include <string.h>

#include <limits.h>

#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include "kmp.h"
#include "utils.hpp"


// Knuth-Morris-Pratt's algorithm
// https://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm

static enum search_res _kmp_grep(const char*, int, const char*, const char*, get_next_fn, int*);
static int init_partial_match_table(const char* query, const int query_len, char** table);
static void fill_partial_match_table(const char* query, char* T, size_t len);
static void destroy_partial_match_table(char** table);

int QueryInit(const char* text_to_search, int len, Query* q) {

	assert(len > 0);

	q->query = NULL;
	q->partial_match_table = NULL;
	q->len = 0;

	char* query = calloc(len+1, 1);
	if (NULL == query) {
		perror("calloc");
		return -1;
	}
	memcpy(query, text_to_search, len);
	query[len] = '\0';

	char* T = NULL;
	if (-1 == init_partial_match_table(query, len, &T)) {
		fprintf(stderr, "error: init_partial_match_table\n");
		free(query);
		return -1;
	}

	q->query = query;
	q->partial_match_table = T;
	q->len = len;
	return 0;
}

void QueryDestroy(Query* q) {
	if (NULL != q->query) {
		free(q->query);
		q->query = NULL;
	}
	destroy_partial_match_table(&(q->partial_match_table));
	q->len = 0;
}

enum search_res kmp_grep(Query q, const char* buf, get_next_fn get_next, int* get_next_status) {
	return _kmp_grep(q.query, q.len, q.partial_match_table, buf, get_next, get_next_status);
}

static enum search_res _kmp_grep(
		const char* query, int query_len, 
		const char* partial_match_table, 
		const char* buf, get_next_fn get_next, int* get_next_status
) {
	const char* const T = partial_match_table;
	const char* const Q = query;
	const char* const B = buf;
	const int lenQ = query_len;


    int posQ = 0; //the position of the current character in query


    // TODO: add check for binary

    int maxB = 0;
    while (GET_NEXT_SUCCESS == get_next(&maxB, get_next_status)) {
    	int posB = 0; //the position of the current character in buf

	    while (posB < maxB) {
	        if (Q[posQ] == B[posB]) {
	            posB++;
	            posQ++;
	            if (posQ == lenQ) {
	                return SEARCH_FOUND;
	            }
	        }
	        else {
	            posQ = T[posQ];
	            if (posQ < 0) {
	                posB++; 
	            	posQ++;
	            }
	        }
	    }

	}

	if (GET_NEXT_EOF == get_next(&maxB, get_next_status)) {
    	return SEARCH_NOT_FOUND;
    }

    return SEARCH_ERROR;
}

static int init_partial_match_table(const char* query, const int query_len, char** table) {

	const int len = query_len;

	char* T = calloc(len, 1);
	if (NULL == T) {
		perror("calloc");
		return -1;
	}

	fill_partial_match_table(query, T, len);

	*table = T;
	return 0;
}

static void fill_partial_match_table(const char* query, char* T, size_t len) {
	int posT = 1; // the current position we are computing in T
	int cndW = 0; // the zero-based index in W of the next character of the current candidate substring

	T[0] = -1;
    while (posT < len) {
        if (query[posT] == query[cndW]) {
            T[posT] = T[cndW];
        }
        else {
            T[posT] = cndW;
            cndW = T[cndW];
            while (cndW >= 0 && query[posT] != query[cndW]) {
                cndW = T[cndW];
            }
        }
        posT++;
        cndW++;
    }
}

static void destroy_partial_match_table(char** table) {
	my_free((void**)table);
}