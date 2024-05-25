# HeapAllocator
------------
This is a basic heap allocator that I wrote after learning about memory organization. It's meant to display my knowledge on malloc() and free() along with best fit allocation and immediate coalescing.

### How It's Made:
------------
#### Initialization:
To simulate an actual heap I decided to allocate a large chunk of "free space" using malloc.

The init_heap function is called once at the start of the program and sets up the heaps start and end bounds.

#### Overhead:
For this project I decided that each block of memory would have a header, if the block was free then it would also have a footer. 

For both the header and footer I initially created a header struct but then wrapped it with a union and char array of length 16 so that the header's address would always be a multiple of 16 bytes. 

Within the union, each block has a size attribute, free flag, previous block free flag, and a reference to the next block in front of it.

#### Allocation:
For allocation I decided to implement a best-fit algorithm to minimize fragmentation. The heap_malloc function takes a size in bytes and calls the find_free_block function which traverses the heap and returns the smallest free block that can accommodate the requested size. If the free block found is larger than what is required, the allocator will split the block into two, taking the space it needs and designating the remaining space as a new free block. The heap_alloc method also applies padding to the memory blocks to ensure that allocation adheres to double word alignment.

#### Freeing:
The heap_free function takes a pointer to the block that is attempting to be freed. After freeing the designated block it checks to see if the adjacent memory blocks are free, if either of them are free it will coalesce the blocks to reduce false fragmentation. 

#### Printing the Heap:
I created a print_heap function to display the contents of memory which helped with debugging as well as providing an easy visual representation of the memory allocation.

### Possible Optimizations:
------------
    - Multi-Access protection - preventing multiple access to malloc at once
    - Using an explicit free list
    - Different allocation algorithm to optimize speed
    - Minimize overhead, using p-bits and a-bits instead
    - More rigorous testing

### Lessons Learned:
------------
This was my first personal project so I know that there are many improvements to be had but I found while building this that it was very effective to first have a plan and create some skeleton code for me to fill in gradually. I also learned that I shouldn't hesitate to make commits because the more I have the easier it will be to debug and improve my code in the future.

### Compiling:
------------
The Makefile within the repository has the commands needed to compile and run the code. Running ./heap_test will perform a very basic demo that demonstrates allocation, freeing, and coalescing. 
