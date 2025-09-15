#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexical.h"

int main() {
    scanner("input.txt");
    struct token toke;
    while(toke.type != NULO){
        toke = next_token();
    }
    
    return 0;
}