#include "heap.h"

int main(int argc, char** argv) {
    init_heap();
    // Allocate 3 blocks
    void* ptr1 = heap_malloc(100);
    void* ptr2 = heap_malloc(2000);
    void* ptr3 = heap_malloc(300);
    print_heap();

    // Free the middle block
    heap_free(ptr2);
    print_heap();

    // Free the first block to test coalescing
    heap_free(ptr1);
    print_heap();
}
