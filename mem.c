#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Mem {
	void 	*ptr;
	size_t 	sz;
	int 	freed;
} Mem;

Mem **___MEMORY___ 			= NULL;
size_t ___MEMCOUNT___ 		= 0;
int ___MEM_DEBUG_MODE___ 	= 0;

__attribute__((constructor)) void init_app_memory() {
	___MEMORY___ = (Mem **)malloc(sizeof(Mem *) * 1);
	memset(___MEMORY___, '\0', sizeof(Mem *) * 1);

	/* Detect --debug-memory in cmdline */
	FILE *f = fopen("/proc/self/cmdline", "r");
	if(!f)
		return;

	char *data = (char *)malloc(1024);
	memset(data, '\0', 1024);
	fread(data, 1, 1024, f);

	if(!strcmp((data + strlen(data) + 1), "--debug-memory"))
		___MEM_DEBUG_MODE___ = 1;

	free(data);
	if(___MEM_DEBUG_MODE___)
		printf("[ON_START]: Initialized App Memory\n");
}

__attribute__((destructor)) void uninit_app_memory() {
	if(___MEMCOUNT___ == 0)
		return;

	if(___MEM_DEBUG_MODE___)
		printf("\x1b[31m[ x ]\x1b[39m Memory Warning \x1b[31m[ x ]\x1b[39m\n\t = > Free'ing memory that wasn't free'd via code!\n");

	for(int i = 0; i < ___MEMCOUNT___; i++) {
		Mem *c = ___MEMORY___[i];
		if(!c)
			break;

		if(___MEM_DEBUG_MODE___)
			printf("[ON_EXIT]: Free'd %p\n", c->ptr);

		free(___MEMORY___[i]->ptr);
		memset(___MEMORY___[i], '\0', sizeof(Mem));
		free(___MEMORY___[i]);
	}
}

// 0 for 1024
// -1 for 2048
// -2 for 4096
void *allocate(int size) {
	int sz = size;
	switch(sz) {
		case 0:
			sz = 1024;
			break;
		case -1:
			sz = 2048;
			break;
		case -2:
			sz = 4086;
			break;
	}

	void *p = malloc(sz);
	memset(p, '\0', sz);

	Mem *m = (Mem *)malloc(sizeof(Mem));
	m->ptr = p;
	m->sz = sz;

	___MEMORY___[___MEMCOUNT___] = m;
	___MEMCOUNT___++;
	___MEMORY___ = realloc(___MEMORY___, sizeof(Mem *) * ___MEMCOUNT___);

	if(___MEM_DEBUG_MODE___)
		printf("Allocate %d memory to %p\n", sz, p);

	return p;
}

static int find_ptr(void *p) {
	if(!p)
		return -1;

	for(int i = 0; i < ___MEMCOUNT___; i++)
		if(___MEMORY___[i]->ptr == p)
			return i;

	return -1;
}

void pfree(void *ptr) {
	if(!ptr || ___MEMCOUNT___ == 0)
		return;

	int pos = find_ptr(ptr);
	if(pos < 0)
		return;

	for(int i = pos; i < ___MEMCOUNT___; i++) {
		if(!___MEMORY___[i])
			break;

		if(i == pos) {
			free(___MEMORY___[i]->ptr);
			memset(___MEMORY___[i], '\0', sizeof(Mem));
			free(___MEMORY___[i]);
			if(___MEM_DEBUG_MODE___)
				printf("Free'd %p\n", ptr);
		}

		if(___MEMORY___[i + 1] == NULL)
			break;

		___MEMORY___[i] = ___MEMORY___[i + 1];
	}

	___MEMCOUNT___--;
}

void *free_n_reuse(void *ptr) {
	if(!ptr || ___MEMCOUNT___ == 0)
		return NULL;

	int pos = find_ptr(ptr);
	size_t sz = ___MEMORY___[pos]->sz;
	pfree(ptr);

	return allocate(sz);
}
