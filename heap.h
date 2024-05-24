#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>


header_t* find_free_block(size_t size);
void* heap_alloc(size_t size);
void* heap_calloc(size_t num, size_t size);
void* heap_realloc(void* block, size_t size);
void heap_free(void* block);
void print_heap();
