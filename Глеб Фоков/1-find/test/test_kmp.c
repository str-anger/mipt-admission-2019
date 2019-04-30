#include <assert.h>
#include <string.h>

#include "../kmp.h"

void test(const char* str, const char* query, enum search_res search_res) {
	int counter = 0;
	enum get_next_res get_next(int* read, int* status) {
		if (0 == counter) {
			*read = strlen(str);
			counter++;
			return GET_NEXT_SUCCESS;
		} else {
			return GET_NEXT_EOF;
		}
	}

	int len = strlen(query);
	Query q;
	assert (-1 != QueryInit(query, len, &q));
	assert(search_res == kmp_grep(q, str, get_next, NULL));
	QueryDestroy(&q);
}

int main() {

	const char* str = "hellohella";
	
	test(str, "ello", SEARCH_FOUND);
	test(str, "ohe", SEARCH_FOUND);
	test(str, "1llo", SEARCH_NOT_FOUND);

	return 0;

}