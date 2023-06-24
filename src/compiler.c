#include "../inc/compiler.h"

void firstPass(char *source_code, struct tableEntry symbol_table[],
    int *data_counter,int *symbol_index, int memory[], int *instruction_counter, int flags[])
{
    FILE *source_ptr = fopen(source_code, "r");
    char statement[MAX_STATEMENT_SIZE];
    while (fgets(statement, MAX_STATEMENT_SIZE, source_ptr) != NULL)
    {
        char *tok = strtok(statement, " ");
        
        //Add line number to symbol table
        addSymbol(symbol_table, atoi(tok), 'L', *instruction_counter, symbol_index);
        
        //opcode
        tok = strtok(NULL, " ");
        if (strcmp(tok, "rem") == 0)
        {
            continue;
        }else if (strcmp(tok, "input") == 0)
        {
            //varriable about to be read
            tok = strtok(NULL, " ");
            int var_location = findSymbolLocation(symbol_table, tok[0], 'V', data_counter, symbol_index);
            memory[*instruction_counter] = READ * MEMORY_SIZE + var_location;
            ++(*instruction_counter);
        }else if (strcmp(tok, "let") == 0)
        {
            //varriable about to be assigned
            tok = strtok(NULL, " ");
            int result_location = findSymbolLocation(symbol_table, tok[0], 'V', data_counter, symbol_index);

            //'=' character
            tok = strtok(NULL, " ");
            //tok + 2 = start address of expression
            char postfix[MAX_STATEMENT_SIZE];
            convertToPostfix(tok + 2, postfix);
            evaluatePostFixExpression(postfix, symbol_table, symbol_index, memory, instruction_counter, data_counter);

            //load from temporary location
            memory[*instruction_counter] = LOAD * MEMORY_SIZE + findSymbolLocationLoosely(symbol_table, 'T', 'V', *symbol_index);
            if (*instruction_counter > 0 && memory[(*instruction_counter) - 1] / MEMORY_SIZE == STORE
                && memory[(*instruction_counter) - 1] % MEMORY_SIZE == memory[*instruction_counter] % MEMORY_SIZE)
            {
                (*instruction_counter) -= 2;
            }
            ++(*instruction_counter);
            
            //store back to varriable
            memory[*instruction_counter] = STORE * MEMORY_SIZE + result_location;
            ++(*instruction_counter);
        }else if (strcmp(tok, "print") == 0)
        {
            //varriable about to be read
            tok = strtok(NULL, " ");
            int var_location = findSymbolLocation(symbol_table, tok[0], 'V', data_counter, symbol_index);
            memory[*instruction_counter] = WRITE * MEMORY_SIZE + var_location;
            ++(*instruction_counter);
        }else if (strcmp(tok, "goto") == 0)
        {
            //line number to be gone to
            tok = strtok(NULL, " ");
            int line_number = atoi(tok);
            int line_number_location = findSymbolLocationLoosely(symbol_table, line_number, 'L', *symbol_index);
            if (line_number_location == -1)
            {
                flags[*instruction_counter] = line_number;
                memory[*instruction_counter] = BRANCH * MEMORY_SIZE;
            }else
            {
                memory[*instruction_counter] = BRANCH * MEMORY_SIZE + line_number_location;
            }
            ++(*instruction_counter);
        }else if (strcmp(tok, "if") == 0)
        {
            tok = strtok(NULL, " ");
            int left_location = 0;
            if (tok[0] >= '0' && tok[0] <= '9')
            {
                left_location = findSymbolLocationLoosely(symbol_table, atoi(tok), 'C', *symbol_index);
            }else
            {                
                left_location = findSymbolLocationLoosely(symbol_table, tok[0], 'V', *symbol_index);
            }
            tok = strtok(NULL, " ");
            char *op = malloc(sizeof(char) * (strlen(tok) + 1));
            strcpy(op, tok);
            tok = strtok(NULL, " ");
            int right_location = 0;
            if (tok[0] >= '0' && tok[0] <= '9')
            {
                right_location = findSymbolLocationLoosely(symbol_table, atoi(tok), 'C', *symbol_index);
            }else
            {                
                right_location = findSymbolLocationLoosely(symbol_table, tok[0], 'V', *symbol_index);
            }
            
            tok = strtok(NULL, " "); //"goto"
            //line number to be gone to
            tok = strtok(NULL, " ");
            int line_number = atoi(tok);
            int line_number_location = findSymbolLocationLoosely(symbol_table, line_number, 'L', *symbol_index);

            if (strcmp(op, "==") == 0 || strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0)
            {
                memory[*instruction_counter] = LOAD * MEMORY_SIZE + left_location;
                if (*instruction_counter > 0 && memory[(*instruction_counter) - 1] / MEMORY_SIZE == STORE
                    && memory[(*instruction_counter) - 1] % MEMORY_SIZE == memory[*instruction_counter] % MEMORY_SIZE)
                {
                    (*instruction_counter) -= 2;
                }
                ++(*instruction_counter);
                memory[*instruction_counter] = SUBSTRACT * MEMORY_SIZE + right_location;
                ++(*instruction_counter);
                if (line_number_location == -1)
                {
                    flags[*instruction_counter] = line_number;
                    memory[*instruction_counter] = BRANCHZERO * MEMORY_SIZE;
                }else
                {
                    memory[*instruction_counter] = BRANCHZERO * MEMORY_SIZE + line_number_location;
                }
                ++(*instruction_counter);
            }
            if (strcmp(op, "==") != 0)
            {
                if (op[0] == '<')
                {
                    memory[*instruction_counter] = LOAD * MEMORY_SIZE + left_location;
                    if (*instruction_counter > 0 && memory[(*instruction_counter) - 1] / MEMORY_SIZE == STORE
                        && memory[(*instruction_counter) - 1] % MEMORY_SIZE == memory[*instruction_counter] % MEMORY_SIZE)
                    {
                        (*instruction_counter) -= 2;
                    }
                    ++(*instruction_counter);
                    memory[*instruction_counter] = SUBSTRACT * MEMORY_SIZE + right_location;
                    ++(*instruction_counter);
                }else
                {
                    memory[*instruction_counter] = LOAD * MEMORY_SIZE + right_location;
                    if (*instruction_counter > 0 && memory[(*instruction_counter) - 1] / MEMORY_SIZE == STORE
                        && memory[(*instruction_counter) - 1] % MEMORY_SIZE == memory[*instruction_counter] % MEMORY_SIZE)
                    {
                        (*instruction_counter) -= 2;
                    }
                    ++(*instruction_counter);
                    memory[*instruction_counter] = SUBSTRACT * MEMORY_SIZE + left_location;
                    ++(*instruction_counter);
                }
                if (line_number_location == -1)
                {
                    flags[*instruction_counter] = line_number;
                    memory[*instruction_counter] = BRANCHNEG * MEMORY_SIZE;
                }else
                {
                    memory[*instruction_counter] = BRANCHNEG * MEMORY_SIZE + line_number_location;
                }
                ++(*instruction_counter);
            }
            free(op);
        }else if (strcmp(tok, "end") == 0)
        {
            memory[*instruction_counter] = HALT * MEMORY_SIZE;
            ++(*instruction_counter);
        }
    }
    fclose(source_ptr);
}

void secondPass(char *sml_code, int memory[], int flags[], struct tableEntry symbol_table[], int instruction_count, int symbol_count)
{
    FILE *sml_pointer = fopen(sml_code, "w");
    for (int i = 0; i < instruction_count; ++i)
    {
        if (flags[i] != -1)
        {
            memory[i] += findSymbolLocationLoosely(symbol_table, flags[i], 'L', symbol_count);
        }
        fprintf(sml_pointer, "%+06d\n", memory[i]);
    }

    for (int i = 0; i < symbol_count; ++i)
    {
        if (symbol_table[i].type == 'C') 
        {
            memory[symbol_table[i].location] = symbol_table[i].symbol;
        }
    }
    fclose(sml_pointer);
}

void compile(int memory[], int *instruction_counter, char *source_code, char *sml_code)
{
    struct tableEntry symbol_table[MEMORY_SIZE];    
    int flags[MEMORY_SIZE];
    int data_counter = MEMORY_SIZE;
    int symbol_index = 0;
    memset(flags, -1, sizeof(flags));
    firstPass(source_code, symbol_table, &data_counter, &symbol_index, memory, instruction_counter, flags);
    secondPass(sml_code, memory, flags, symbol_table, *instruction_counter, symbol_index);
}