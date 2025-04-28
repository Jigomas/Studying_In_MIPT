#include <stdio.h>
#include <ctype.h>

void reverse_word(int c) {
    if (c == EOF || isspace(c)) {
        if (c != EOF) {
            putchar(c);
        }
        return;
    }

    int next_c = getchar();
    reverse_word(next_c); 
    putchar(c);
}

void process_input() {
    int c = getchar();
    if (c == EOF) {
        return;
    }

    if (isspace(c)) {
        putchar(c);
        process_input();
    } else {
        reverse_word(c);
        process_input();
    }
}

int main() {
    process_input();
    return 0;
}