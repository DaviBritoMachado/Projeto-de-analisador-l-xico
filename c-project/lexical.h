// \0 is null, end of string
#ifndef LEXICAL_H
#define LEXICAL_H
#include <stdbool.h>

typedef enum {
    IDENTIFIER, NUMBER, REL_OPERATOR, MATH_OPERATOR, ASSIGNMENT, RESERVED_WORD, BRACKETS, NULO /*NULO para retornar vazio*/
} token_type;

typedef struct node {
    char file_char;
    struct node * next;
    struct node * last;
}Node;

struct token {
    int type;
    char text[50]; 
};

Node * create_node_list(char c);
Node * add_node(char c, Node * n);
char next_node();
void last_node();
char scanner(char filename[]);
void create_reserver_list();
struct token next_token();
bool is_space(char c);
bool is_letter(char c);
bool is_digit(char c);
bool is_underline(char c);
bool is_point(char c);
bool is_math_operator(char c);
bool isRelOperator(char c);
bool is_equal(char c);
bool is_brackets(char c);
bool is_comment(char c);
bool is_big_comment(char c);
bool is_end_comment(char c);
int verify_reserd_word();
struct token assign_type(int i);
int verify_type(int i);
void error();
struct token is_error(char c, int i);
bool isEoL(char c);
bool isEoF();

#endif
