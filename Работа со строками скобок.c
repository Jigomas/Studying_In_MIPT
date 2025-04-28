#include <stdio.h>


int check(FILE* f, char expect, int flag) {
    int c = fgetc(f);

    switch(c) {
        case '\"' :  
            return check(f, expect, !flag);

        case EOF : 
            return !flag && expect == 0;
        
        case '(' :  
            return flag ? check(f, expect, flag) : check(f, ')', flag);

        case '{' : 
            return flag ? check(f, expect, flag) : check(f, '}', flag);

        case '[' : 
            return flag ? check(f, expect, flag) : check(f, ']', flag);

        case '}' :
            return flag ? check(f, expect, flag) : (expect == c);

        case ']' :  
            return flag ? check(f, expect, flag) : (expect == c);
            
        case ')' :  
            return flag ? check(f, expect, flag) : (expect == c);

        case '\\' :  
            return fgetc(f) ? check(f, expect, flag) : (!flag && expect == 0);

        default :  
            return check(f, expect, flag);
    }
}

int main(int argc, char* argv[]) {
    if (check(stdin, 0, 0)) printf("YES \n");
    else printf("NO \n");
    
    return 0;
}
