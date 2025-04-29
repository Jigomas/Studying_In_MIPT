#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void* str_clone(const void* a) {
	return strdup((const char*)a);
}

int str_cmp(const void* a, const void* b) {

	return strcmp((const char*)a, (const char*) b);
}


int tree_print(Tree* t, int level, void* v) {
	//for(int i = 0; i < level; i++ ) printf("\t");
	//printf("%s\n", (char*)v);
	fputs((char*)v, stdout);
	return 0;
}


int main(int argc, char* argv[]) {
	char buf[1024];
	char id[1024];
	Tree *tree = tree_create(str_clone, str_cmp, free);
	FILE *f = fopen(argv[1], "r");
	while(fgets(buf, sizeof(buf), f)) {
		/*
		if (sscanf(buf, "A %s", id) == 1) {
			tree_insert(tree, id);
		} else if ( sscanf(buf, "D %s", id) == 1 ) {
			tree_delete(tree, id);
		} else if ( sscanf(buf, "S %s", id) == 1 ) {
			char* result;
			if ( tree_search(tree, id, (void**)&result)  ) {
				printf("%s Found\n", result);
			} else {
				printf("Not found\n");
			}
		} else {
			printf("unknown command\n");
		}
		printf("After insert\n");
		tree_traverse(tree, NULL, tree_print);
		*/
		tree_insert(tree, buf);
	}
	fclose(f);
	fprintf(stderr, "Reading\n");
	int count = 0;
	char* res;
	clock_t start = clock();
	while(fgets(buf, sizeof(buf), stdin)) {
		if(tree_search(tree, buf, &res)) count++;

	}
	fprintf(stderr, "%ld\n", clock()- start);
	fprintf(stderr, "Count: %d\n", count);
	//tree_traverse(tree, NULL, tree_print);
	tree_destroy(tree);

}
