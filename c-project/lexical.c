#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexical.h"
#define LIMIT 1024

char reserved_words[5][6];
Node * file_contente = NULL;
Node * actual_node = NULL;

Node * create_node_list(char c){
    file_contente = (Node *) malloc(sizeof(Node));
    file_contente -> file_char = c;
    file_contente -> next = NULL;
    file_contente -> last = NULL;
    actual_node = file_contente;
    return(file_contente -> next);
}

Node * add_node(char c, Node * n){
    Node * new_node = (Node *) malloc(sizeof(Node));
    new_node -> file_char = c;
    new_node -> next = NULL;
    new_node -> last = n;
    n -> next = new_node;
    return(new_node);
}

char next_node(){
    char c = actual_node -> file_char;
    actual_node = actual_node -> next;
    return c;
}

void last_node(){
    actual_node = actual_node -> last;
}

char scanner(char filename[LIMIT]) {
    FILE *pFile = fopen(filename, "r");
    char buffer[LIMIT];

    if (!pFile){
        printf("Arquivo não foi aberto");
    }

    int x = 0;

    while(fgets(buffer, LIMIT, pFile) != NULL){
        x = 0;
        while(buffer[x] != '\n' && buffer[x] != '\0'){
            if(file_contente == NULL){
                create_node_list(buffer[x++]);
            }
            actual_node = add_node(buffer[x++], actual_node);
        }
        actual_node = add_node(buffer[x], actual_node);
    }
    actual_node = add_node(' ', actual_node);
    
    fclose(pFile);
    create_reserver_list();
    actual_node = file_contente;
    
}

void create_reserver_list(){
    strcpy(reserved_words[0], "print");
    strcpy(reserved_words[1], "float");
    strcpy(reserved_words[2], "int");
    strcpy(reserved_words[3], "if");
    strcpy(reserved_words[4], "else");
    
}

char token_content[50];
int line = 1;
int column = 0;

struct token next_token(){
    char current_char;
    int state = 0;
    int posi = 0;
    int v = 0;

    while(true){
        if(isEoF()){
            struct token nul;
            nul.type = NULO;
            return nul;
        }
        
        current_char = next_node();
        column++;

        switch (state)
        {
        case 0:
            posi = 0;
            if (is_letter(current_char)) {
                token_content[posi] = current_char;
				state = 1;
            } else if(is_digit(current_char)) {
                token_content[posi] = current_char;
				state = 2;
            } else if(isRelOperator(current_char)) {
                token_content[posi] = current_char;
				state = 4;
            } else if(is_math_operator(current_char)) {
                token_content[posi] = current_char;
				state = 5;
            } else if(is_equal(current_char)) {
                token_content[posi] = current_char;
                state = 6;
            } else if(is_brackets(current_char)) {
                token_content[posi] = current_char;
                state = 7;
            } else if(current_char == ' '){    
            } else if(isEoL(current_char)){
                column = 0;
                line++;
            } else{
				state 8;
            }
            break;
        case 1:
            if(is_letter(current_char) || is_digit(current_char)) {
                posi += 1;
				token_content[posi] = current_char;
			} else {
                v = verify_reserd_word();
                if(v != 0){
                    return assign_type(v); /*RESERVED_WORD*/
                }
				return assign_type(IDENTIFIER);
			}
            break;
        case 2:
            if(is_digit(current_char)) {
                posi += 1;
                token_content[posi] = current_char;
            } else if(is_point(current_char)) {
                posi += 1;
                token_content[posi] = current_char;
				state = 3;
            } else{
                return assign_type(NUMBER);
            }
            break;
        case 3:
            if(is_digit(current_char)) {
                posi += 1;
                token_content[posi] = current_char;
            } else{
                return assign_type(NUMBER);
            }
            break;
        case 4:
            if(is_equal(current_char)) {
                posi += 1;
                token_content[posi] = current_char;
                char t = next_node();
                return assign_type(REL_OPERATOR);
            }
            return assign_type(REL_OPERATOR);
        case 5:
            if(is_comment(current_char)){
                while(current_char != '\n'){
                    current_char = next_node();
                    if(isEoF()){
                        struct token nul;
                        nul.type = NULO;
                        return nul;
                    }
                }
                line++;
                state = 0;
                break;
            } else if(is_big_comment(current_char)){
                while(!is_end_comment(current_char)){
                    if(isEoL(current_char)){
                        line++;
                    }
                    current_char = next_node();
                    if(isEoF()){
                        struct token nul;
                        nul.type = NULO;
                        return nul;
                    }
                }
                state = 0;
                break;
            }
            return assign_type(MATH_OPERATOR);
        case 6:
            if(is_equal(current_char)) {
                posi += 1;
                token_content[posi] = current_char;
                char t = next_node();
                return assign_type(REL_OPERATOR);

            }
            return assign_type(ASSIGNMENT);
        case 7:
            return assign_type(BRACKETS);
        case 8:
			collumn--;
			state = 0;
            printf("ERRO NA LINHA %i E COLUNA %i\n", line, column);
			break;
        }
    }
}

bool is_letter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');		
}
	
bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool is_point(char c) {
    return c == '.';
}
	
bool is_math_operator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}
	
bool isRelOperator(char c) {
	return c == '>' || c == '<' || c == '!';
}

bool is_equal(char c) {
    return c == '=';
}

bool is_brackets(char c) {
    return c == '(' || c == ')';
}

bool is_comment(char c) {
    last_node();
    last_node();
    bool x = next_node() == '/';
    next_node();

    return (x && c == '/');
}

bool is_big_comment(char c) {
    last_node();
    last_node();
    bool x = next_node() == '/';
    next_node();
    
    return (x && c == '*');
}

bool is_end_comment(char c) {
    last_node();
    last_node();
    bool x = next_node() == '*';
    next_node();

    return (x && c == '/');
}

int verify_reserd_word(){
	int x = 0;
    for(x = 0; x < sizeof(reserved_words);x++){
        if(strcmp(token_content, reserved_words[x]) == 0){
            return RESERVED_WORD;
        }
    }
    return 0;
}

struct token assign_type(int i){
    last_node();
    column--;
    struct token ret;
    strcpy(ret.text, token_content);
    ret.type = i;
    printf("%s || ", ret.text);
    verify_type(ret.type);
    memset(token_content, '\0', sizeof(token_content));
	return ret;
}

int verify_type(int i){
    switch (i)
    {
    case IDENTIFIER:
        printf("IDENTIFIER\n");
        break;
    case NUMBER:
        printf("NUMBER\n");
        break;
    case REL_OPERATOR:
        printf("REL_OPERATOR\n");
        break;
    case MATH_OPERATOR:
        printf("MATH_OPERATOR\n");
        break;
    case ASSIGNMENT:
        printf("ASSIGNMENT\n");
        break;
    case RESERVED_WORD:
        printf("RESERVED_WORD\n");
        break;
    case BRACKETS:
        printf("BRACKETS\n");
        break;
    case NULO:
        printf("NULO");
        break;
    
    default:
        break;
    }
}

bool isEoL(char c) {
    return (c == '\n' || c == '\r');
}

bool isEoF() {
    if(actual_node -> next == NULL){
        return true;
    }
    return false;
}
