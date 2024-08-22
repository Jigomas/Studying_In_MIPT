#include <stdio.h>
void Function_(int a){
    static int c = 0;
//    static int c;
//    c = 0;

    if (a > 0){
        c+= 1;
        printf("%d\n", c);
    }
}

int main(){
    Function_(7);
    Function_(7);
        Function_(7);
    Function_(7);
}
