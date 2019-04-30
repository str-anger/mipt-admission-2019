#pragma once

enum search_res {
	SEARCH_FOUND,
	SEARCH_NOT_FOUND,
	SEARCH_ERROR
};

enum get_next_res {
	GET_NEXT_EOF,
	GET_NEXT_SUCCESS,
	GET_NEXT_ERROR
};

typedef enum get_next_res (*get_next_fn)(int* read, int* status);

typedef struct {
	char* query;
	char* partial_match_table;
	int len;
} Query;

int QueryInit(const char* text_to_search, int len, Query* q);
void QueryDestroy(Query* q);

enum search_res kmp_grep(Query query, const char* buf, get_next_fn get_next, int* get_next_status);