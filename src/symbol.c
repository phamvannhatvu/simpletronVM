#include "../inc/symbol.h"

int findSymbolLocation(struct tableEntry symbol_table[], int symbol, char type, int *data_counter)
{
    for (int i = MEMORY_SIZE - 1; i >= *data_counter; --i)
    {
        if (symbol_table[i].symbol == symbol && symbol_table[i].type == type)
        {
            return symbol_table[i].location;
        }
    }
    addSymbol(symbol_table, symbol, type, data_counter);
    return *data_counter;
}

void addSymbol(struct tableEntry symbol_table[], int symbol, char type, int *data_counter)
{
    if (type == 'L')
    {

    }else
    {
        --(*data_counter);
        symbol_table[*data_counter].symbol = symbol;
        symbol_table[*data_counter].type = type;
        symbol_table[*data_counter].location = *data_counter;
    }
}