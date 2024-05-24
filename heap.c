#include "heap.h"

/*
 * Union that represents the header of a memory block.
 *
 */
union header {
    struct {
        size_t size;
        uint free; // 1 if free, 0 if not
        uint prev_free; // 1 if the previous block is free, 0 if not
        union header* next_block;
    } s;
    char padding[16]; // Ensures that each header is on an address that is a multiple of 16
};

typedef union header header_t;

// Pointer to the start and end of the heap
header_t* start = NULL;
header_t* end = NULL;


header_t* find_free_block(size_t size) {

    header_t* current = start;

    int best_fit_size = -1; // Size of the best fit block
    void* best_fit_header = NULL; // Pointer to the best fit block header
    void* best_fit_block; // Pointer to the best fit block
    // Traverse the free list until the end is reached or an exact fit is found - best fit
    while (current != end) {
        int current_size = current->s.size;
        // Check if the block is free
        if (current->s.free == 1) {
            //Check if there is an exact fit
            if (current_size == size) {
                return current;
            }
            else {
                // Check if the block is a better fit than the current best fit
                if (current_size > size && (best_fit_size == -1 || current_size < best_fit_size)) {
                    best_fit_size = current_size;
                    best_fit_header = current;
                    best_fit_block = best_fit_header + sizeof(header_t);
                }
            }
            // Move to the next block
            current = current->s.next_block;
        }
        else {
            // Move to the next block
            current = current->s.next_block;
        }
    }
    return best_fit_header;
}

/*
 * Allocates a block of memory of size bytes.
 * Returns a pointer to the beginning of the block of memory that was allocated
 */
void* heap_alloc(size_t size) {
    if (size < 1) {
        return NULL;
    }
    void* block;
    int total_size;
    int pre_padding_size = size + sizeof(header_t);

    // Pad the block size to be a multiple of 8 - double word alignment
    if (pre_padding_size % 8 != 0) {
        total_size = pre_padding_size + (8 - pre_padding_size % 8);
    } else {
        total_size = pre_padding_size;
    }
    // Find the best fit block in the free list
    header_t* best_fit_header = find_free_block(total_size);

    // If a block is found, allocate the block
    if (best_fit_header != NULL) {
        // If the size is an exact fit, allocate the block
        if (best_fit_header->s.size == total_size) {
            best_fit_header->s.free = 0;
            best_fit_header->s.next_block->s.prev_free = 0; // Update the next blocks prev_free
            block = (void*) best_fit_header + sizeof(header_t);
        }
        else {
            // If the size is not an exact fit, split the block
            header_t* new_block = best_fit_header + total_size;
            new_block->s.size = best_fit_header->s.size - total_size;
            new_block->s.free = 1;
            new_block->s.next_block = best_fit_header->s.next_block;
            new_block->s.prev_free = 0;
            new_block->s.next_block->s.prev_free = 1; // Update the next blocks prev_free
            best_fit_header->s.size = total_size;
            best_fit_header->s.free = 0;
            best_fit_header->s.next_block = new_block;
            block = (void*) best_fit_header + sizeof(header_t);
        }
    }
    // If no block is found, request more memory from the OS
    else {
        header_t* new_block = sbrk(total_size);
        // Check if the request was successful
        if (new_block == (void*) -1) {
            return NULL;
        }
        new_block->s.size = total_size;
        new_block->s.free = 0;
        new_block->s.next_block = end;
        new_block->s.prev_free = 0;
        block = (void*) new_block + sizeof(header_t);
        if (start == NULL) {
            start = new_block;
        }
        if (end != NULL) {
            end->s.next_block = new_block;
        }
        end = new_block;
    }
    return block;
}

void* calloc(size_t num, size_t size) {
    size_t total_size;
    void* block;

    // Check for null parameters
    if (num == NULL || size == NULL) {
        return NULL;
    }

    total_size = num * size;

    // Check for overflow
    if (total_size / num != size) {
        return NULL;
    }

    block = malloc(total_size);
    if (block == NULL) {
        return NULL;
    }

    // Zero out the block
    memset(block, 0, total_size);

    return block;
}

//TODO: Implement heap_realloc


//TODO: Implement heap_free
void heap_free(void* block) {
    if (block == NULL) {
        return;
    }

    header_t* current = block - sizeof(header_t);

    // Mark the block as free
    current->s.free = 1;

    // Update the next blocks prev_free
    current->s.next_block->s.prev_free = 1;

    // Create a footer for the block
    header_t* footer = (void*) current + current->s.size - sizeof(header_t);
    footer->s.size = current->s.size;

    // Coalesce adjacent free blocks - immediate coalescing

    // Check if the next block is free
    if (current->s.next_block != end && current->s.next_block->s.free == 1) {
        current->s.size += current->s.next_block->s.size;
        current->s.next_block = current->s.next_block->s.next_block;

        footer = (void*) current + current->s.size - sizeof(header_t);
        footer->s.size = current->s.size;
    }

    // Check if the previous block is free
    if (current->s.prev_free == 1) {
        header_t* prev_block_footer = (void*) current - sizeof(header_t);
        header_t* prev_block_header = (void*) current - prev_block_footer->s.size;
        prev_block_header->s.size += current->s.size;

        footer->s.size = prev_block_header->s.size;
    }
}