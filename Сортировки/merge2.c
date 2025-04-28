#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _list {
    char* str;
    struct _list* next;
} list;

void free_list(list* lst) {
    if (!lst) return;
    list* next = lst->next;
    free(lst->str);
    free(lst);
    free_list(next);
}

int compare_first_4(const char* a, const char* b) {
    return strncmp(a, b, 4);
}

int _split(list* lst, list** a, list** b) {
    if (!lst) {
        *a = NULL;
        *b = NULL;
        return 0;
    }
    *a = lst;
    return _split(lst->next, b, &lst->next);
}

int _merge(list* a, list* b, list** result) {
    if (!a) *result = b;
    else if (!b) *result = a;
    else if (compare_first_4(a->str, b->str) <= 0) {
        *result = a;
        return _merge(a->next, b, &a->next);
    } else {
        *result = b;
        return _merge(a, b->next, &b->next);
    }
}

int mergesort(list** result) {
    list* a = NULL, *b = NULL;
    if (!*result || !(*result)->next) return 0;
    _split(*result, &a, &b);
    mergesort(&a);
    mergesort(&b);
    _merge(a, b, result);
    return 0;
}

list* create_node(const char* str) {
    list* new_node = (list*)malloc(sizeof(list));
    new_node->str = strdup(str);
    new_node->next = NULL;
    return new_node;
}

void print_list(list* lst) {
    while (lst) {
        printf("%s\n", lst->str);
        lst = lst->next;
    }
}

list* read_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }
    list* head = NULL;
    list* tail = NULL;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        list* new_node = create_node(buffer);
        if (!head) {
            head = new_node;
        } else {
            tail->next = new_node;
        }
        tail = new_node;
    }
    fclose(file);
    return head;
}

void write_to_file(list* lst, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    while (lst) {
        fprintf(file, "%s\n", lst->str);
        lst = lst->next;
    }
    fclose(file);
}

int main() {
    const char* input_filename = "bigfile.txt";
    list* lst = read_from_file(input_filename);
    if (lst) {
        printf("Before sorting:\n");
        print_list(lst);
        mergesort(&lst);
        printf("\nAfter sorting by first 4 characters:\n");
        print_list(lst);
        const char* output_filename = "sorted_data.txt";
        write_to_file(lst, output_filename);
        free_list(lst);
    }
    return 0;
}
