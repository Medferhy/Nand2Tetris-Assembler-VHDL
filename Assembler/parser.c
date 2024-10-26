#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

static FILE *input_file;
static char current_command[MAX_LINE_LENGTH];

void parser_init(FILE *file) {
    input_file = file;
    current_command[0] = '\0';
}

bool parser_has_more_commands() {
    return fgets(current_command, MAX_LINE_LENGTH, input_file) != NULL;
}

void parser_advance() {
    char *comment = strstr(current_command, "//");
    if (comment) *comment = '\0';

    char *start = current_command;
    while (isspace(*start)) start++;
    memmove(current_command, start, strlen(start) + 1);

    char *end = current_command + strlen(current_command) - 1;
    while (end > current_command && isspace(*end)) end--;
    *(end + 1) = '\0';
}

CommandType parser_command_type() {
    if (current_command[0] == '@') return A_COMMAND;
    if (current_command[0] == '(') return L_COMMAND;
    if (strchr(current_command, '=') || strchr(current_command, ';')) return C_COMMAND;
    return INVALID_COMMAND;
}

char* parser_symbol() {
    static char symbol[MAX_LINE_LENGTH];
    
    
    if (parser_command_type() == A_COMMAND) {
    
        if (current_command[1] == 'R') {
        
            int value = atoi(current_command + 2);
            
            if (value >= 0 && value <= 15) {
                
                for (int i = 15; i >= 0; i--) {
                    symbol[15 - i] = (value & (1 << i)) ? '1' : '0';
                }
                symbol[16] = '\0'; 
            } else {
                
                printf( "ERROR: Invalid register number");
                return NULL;
                
            }
        } else if (isdigit(current_command[1])) {
            
            int value = atoi(current_command + 1);
            if (value >= 0 && value <= 15) {
        
                for (int i = 15; i >= 0; i--) {
                    symbol[15 - i] = (value & (1 << i)) ? '1' : '0';
                }
                symbol[16] = '\0'; 
            } else {
        
                
                printf("ERROR: Invalid register number");
                return NULL;
            }
        } else {
        
            strcpy(symbol, current_command + 1);
        }
    } else if (parser_command_type() == L_COMMAND) {
    
        strncpy(symbol, current_command + 1, strlen(current_command) - 2);
        symbol[strlen(current_command) - 2] = '\0';
    }
    
    return symbol;
}



char* parser_dest() {
    static char dest[MAX_LINE_LENGTH];
    if (parser_command_type() == C_COMMAND && strchr(current_command, '=')) {
        strncpy(dest, current_command, strchr(current_command, '=') - current_command);
        dest[strchr(current_command, '=') - current_command] = '\0';
    } else {
        dest[0] = '\0';
    }
    return dest;
}

char* parser_comp() {
    static char comp[MAX_LINE_LENGTH];
    char *start = strchr(current_command, '=') ? strchr(current_command, '=') + 1 : current_command;
    char *end = strchr(start, ';') ? strchr(start, ';') : start + strlen(start);
    strncpy(comp, start, end - start);
    comp[end - start] = '\0';
    return comp;
}

char* parser_jump() {
    static char jump[MAX_LINE_LENGTH];
    if (parser_command_type() == C_COMMAND && strchr(current_command, ';')) {
        strcpy(jump, strchr(current_command, ';') + 1);
    } else {
        jump[0] = '\0';
    }
    return jump;
}
