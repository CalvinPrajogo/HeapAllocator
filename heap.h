#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>


#define HEAP_SIZE 0x100000 // 1MB

void* heap_malloc(size_t size);
void heap_free(void* block);
void print_heap();
void init_heap();
