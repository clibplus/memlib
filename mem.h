#pragma once

typedef struct Mem {
	void 	*ptr;
	size_t 	sz;
	int 	freed;
} Mem;

extern Mem **___MEMORY___;
extern size_t ___MEMCOUNT___;
extern int ___MEM_DEBUG_MODE___;

__attribute__((constructor)) void init_app_memory();
__attribute__((destructor)) void uninit_app_memory();
static int find_ptr(void *p);

#if defined(CLIBP_MEM)

    typedef void *ptr;
    void *allocate(int size);
    void pfree(void *ptr);
    void *free_n_reuse(void *ptr);
#endif