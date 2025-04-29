test_tree: tree_b.o test_tree.o
	gcc -o $@ $^

.PHONY: clean

clean:
	rm test_tree
	rm *.o

%.o:%.c
	$(CC) -c -O2 -o $@ $<
