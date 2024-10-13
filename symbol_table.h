#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

void symbol_table_init();
void symbol_table_add_entry(const char *symbol, int address);
bool symbol_table_contains(const char *symbol);
int symbol_table_get_address(const char *symbol);

#endif
