#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "symbol_table.h"
#include "code.h"

#define MAX_SYMBOL_LENGTH 100


/// @brief 
/// @param argc 
/// @param argv 
/// @return 
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s [inputfile.asm]\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (!input_file) {
        perror("Failed to open input file");
        return 1;
    }

    char output_filename[256];
    strcpy(output_filename, argv[1]);
    strcpy(strrchr(output_filename, '.'), ".hack");

    FILE *output_file = fopen(output_filename, "w");
    if (!output_file) {
        perror("Failed to create output file");
        return 1;
    }

    parser_init(input_file);
    symbol_table_init();

    // First pass: Build the symbol table
    int rom_address = 0;
    while (parser_has_more_commands()) {
        parser_advance();
        CommandType command_type = parser_command_type();
        if (command_type == A_COMMAND || command_type == C_COMMAND) {
            rom_address++;
        } else if (command_type == L_COMMAND) {
            symbol_table_add_entry(parser_symbol(), rom_address);
        }
    }

    // Rewind file for second pass
    rewind(input_file);
    parser_init(input_file);

    // Second pass: Translate into binary
    int ram_address = 16;
    while (parser_has_more_commands()) {
        parser_advance();
        CommandType command_type = parser_command_type();

        if (command_type == A_COMMAND) {
            char *symbol = parser_symbol();
            int address;

            if (isdigit(symbol[0])) {
                address = atoi(symbol);
            } else {
                if (!symbol_table_contains(symbol)) {
                    symbol_table_add_entry(symbol, ram_address++);
                }
                address = symbol_table_get_address(symbol);
            }

            fprintf(output_file, "0%015d\n", address);

        } else if (command_type == C_COMMAND) {
            char *dest = code_dest(parser_dest());
            char *comp = code_comp(parser_comp());
            char *jump = code_jump(parser_jump());

            fprintf(output_file, "111%s%s%s\n", comp, dest, jump);
        }
    }

    fclose(input_file);
    fclose(output_file);

    printf("Assembly complete. Output written to %s\n", output_filename);

    return 0;
}
