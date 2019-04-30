#pragma once

#include <stdlib.h>

static void my_free(void** p) {
	if (p != NULL) {
		free(*p);
		*p = NULL;
	}
}