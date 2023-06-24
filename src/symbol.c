#include "../inc/symbol.h"

int findSymbolLocationLoosely(struct tableEntry symbol_table[], int symbol, char type, int symbol_index)
{
    for (int i = 0; i < symbol_index; ++i)
    {
        if (symbol_table[i].symbol == symbol && symbol_table[i].type == type)
        {
            return symbol_table[i].location;
        }
    }
    return -1;
}

int findSymbolLocation(struct tableEntry symbol_table[], int symbol, char type, int *data_counter, int *symbol_index)
{
    int location = findSymbolLocationLoosely(symbol_table, symbol, type, *symbol_index);
    if (location != -1) return location;
    addSymbol(symbol_table, symbol, type, --(*data_counter), symbol_index);
    return *data_counter;
}

void addSymbol(struct tableEntry symbol_table[], int symbol, char type, int location, int *symbol_index)
{
    symbol_table[*symbol_index].symbol = symbol;
    symbol_table[*symbol_index].type = type;
    symbol_table[*symbol_index].location = location;
    ++(*symbol_index);
}