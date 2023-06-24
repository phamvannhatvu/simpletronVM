#ifndef SYMBOL
#define SYMBOL

/*LIMITS DEFINE*/
#define MEMORY_SIZE 1000

/*FILENAMES DEFINE*/
#define RESOURCE_PATH "../resource/"

struct tableEntry {
    int symbol; //ASCII representation
    char type; //'C': constant, 'V': varriable, 'L': line number, 'T': temporary
    int location; //000 - 999
};

int findSymbolLocation(struct tableEntry symbol_table[], int symbol, char type, int *data_counter, int *symbol_index);
int findSymbolLocationLoosely(struct tableEntry symbol_table[], int symbol, char type, int symbol_index);
void addSymbol(struct tableEntry symbol_table[], int symbol, char type, int location, int *symbol_index);

#endif