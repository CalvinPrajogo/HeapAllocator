run: heap_test
	./heap_test

heap_test: heap.c heap.h main.c
	gcc main.c heap.c -Wall -std=gnu99 -Wno-deprecated -Wno-unused -o heap_test

clean:
	rm -f heap_test