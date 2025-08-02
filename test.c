#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLIBP_MEM
#include "mem.h"

int main() {
	char *ptr = allocate(10);
	strcpy(ptr, "TEST");
	printf("%p: %s\n", ptr, ptr);

	ptr = free_n_reuse(ptr);
	strcpy(ptr, "NIGGER");
	printf("%p: %s | %zu\n", ptr, ptr, strlen((char *)ptr));
	pfree(ptr);
	return 0;
}
