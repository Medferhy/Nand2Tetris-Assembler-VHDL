#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
    INVALID_COMMAND
} CommandType;

void parser_init(FILE *file);
bool parser_has_more_commands();
void parser_advance();
CommandType parser_command_type();
char* parser_symbol();
char* parser_dest();
char* parser_comp();
char* parser_jump();

#endif
