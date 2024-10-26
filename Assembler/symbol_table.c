#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct {
    char symbol[100];
    int address;
} SymbolTableEntry;

static SymbolTableEntry symbol_table[TABLE_SIZE];
static int symbol_count = 0;

void symbol_table_init() {
    symbol_count = 0;
}

void symbol_table_add_entry(const char *symbol, int address) {
    strcpy(symbol_table[symbol_count].symbol, symbol);
    symbol_table[symbol_count].address = address;
    symbol_count++;
}

bool symbol_table_contains(const char *symbol) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].symbol, symbol) == 0) {
            return true;
        }
    }
    return false;
}

int symbol_table_get_address(const char *symbol) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].symbol, symbol) == 0) {
            return symbol_table[i].address;
        }
    }
    return -1;
}
