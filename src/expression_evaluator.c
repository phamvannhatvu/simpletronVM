#include "../inc/expression_evaluator.h"

bool isOperator(int c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int precedence(int operator1, int operator2)
{
    if (operator1 == '+' || operator1 == '-')
    {
        return (operator2 == '+' || operator2 == '-') ? 0 : -1;
    }
    return (operator2 == '*' || operator2 == '/') ? 0 : 1;
}

void push(StackNodePtr *topPtr, int value)
{
    StackNodePtr newPtr = malloc(sizeof(StackNodePtr));
    newPtr->nextPtr = *topPtr;
    newPtr->data = value;
    *topPtr = newPtr;
}

int pop(StackNodePtr *topPTr)
{
    int poped_element = (*topPTr)->data;
    StackNodePtr top = *topPTr;
    *topPTr = (*topPTr)->nextPtr;
    free(top);
    return poped_element;
}

int stackTop(StackNodePtr topPtr)
{
    return topPtr->data;
}

bool isEmpty(StackNodePtr topPtr)
{
    return topPtr == NULL;
}

void printStack(StackNodePtr topPtr)
{
    while (topPtr != NULL)
    {
        printf("%c", topPtr->data);
        topPtr = topPtr->nextPtr;
    }
    printf("\n");
}

int calculate(int op1, int op2, char op1_type, char op2_type, int operation, struct tableEntry symbol_table[],
    int *symbol_index, int memory[], int *instruction_counter, int *data_counter)
{
    memory[*instruction_counter] = 20 * MEMORY_SIZE + findSymbolLocation(symbol_table, op1, op1_type, data_counter, symbol_index);
    ++(*instruction_counter);
    if (*instruction_counter > 0 && memory[(*instruction_counter) - 1] / MEMORY_SIZE == 21
        && memory[(*instruction_counter) - 1] % MEMORY_SIZE == memory[*instruction_counter] % MEMORY_SIZE)
    {
        (*instruction_counter) -= 2;
    }
    switch (operation)
    {
    case '+':
        memory[*instruction_counter] = 30 * MEMORY_SIZE + findSymbolLocation(symbol_table, op2, op2_type, data_counter, symbol_index);
        break;
    case '-':
        memory[*instruction_counter] = 31 * MEMORY_SIZE + findSymbolLocation(symbol_table, op2, op2_type, data_counter, symbol_index);
        break;
    case '/':
        memory[*instruction_counter] = 32 * MEMORY_SIZE + findSymbolLocation(symbol_table, op2, op2_type, data_counter, symbol_index);
        break;
    case '*':
        memory[*instruction_counter] = 33 * MEMORY_SIZE + findSymbolLocation(symbol_table, op2, op2_type, data_counter, symbol_index);
        break;
    default:
        break;
    }    
    ++(*instruction_counter);

    //'T': special varriable (uppercase) store temporary subexpression result
    memory[*instruction_counter] = 21 * MEMORY_SIZE + findSymbolLocation(symbol_table, 'T', 'V', data_counter, symbol_index);
    ++(*instruction_counter);   
    return 'T';
}

void convertToPostfix(char infix[], char postfix[])
{
    StackNodePtr topPtr = malloc(sizeof(StackNodePtr));
    topPtr->nextPtr = NULL;
    push(&topPtr, '(');
    int expression_size = strlen(infix);
    infix[expression_size] = ')';

    int posfix_index = 0;
    for (int i = 0; i <= expression_size; ++i)
    {
        if (isOperator(infix[i]))
        {
            while (isEmpty(topPtr) == false && isOperator(stackTop(topPtr)) && precedence(stackTop(topPtr), infix[i]) != -1)
            {
                postfix[posfix_index++] = ' ';
                postfix[posfix_index++] = pop(&topPtr);
            }
            push(&topPtr, infix[i]);
        }else if (infix[i] == ')')
        {
            while (stackTop(topPtr) != '(')
            {
                postfix[posfix_index++] = ' ';
                postfix[posfix_index++] = pop(&topPtr);
            }
            pop(&topPtr);
        }else if (infix[i] == '(')
        {
            push(&topPtr, infix[i]);
        }else if (infix[i] >= '0' && infix[i] <= '9')
        {
            postfix[posfix_index++] = ' ';
            postfix[posfix_index++] = infix[i];
            while (i + 1 <= expression_size && infix[i + 1] >= '0' && infix[i + 1] <= '9')
            {
                ++i;
                postfix[posfix_index++] = infix[i];
            }
        }else if (infix[i] >= 'a' && infix[i] <= 'z')
        {
            postfix[posfix_index++] = ' ';
            postfix[posfix_index++] = infix[i];
        }
    }

    postfix[posfix_index] = '\0';
    infix[expression_size] = '\0';

    while (topPtr != NULL)
    {
        StackNodePtr tmp = topPtr;
        topPtr = topPtr->nextPtr;
        free(tmp);
    }
}

void evaluatePostFixExpression(char *expr, struct tableEntry symbol_table[], int *symbol_index, int memory[],
    int *instruction_counter, int *data_counter)
{
    StackNodePtr topPtr = malloc(sizeof(StackNodePtr));
    int expr_len = strlen(expr);
    char *symbol_runner = strtok(expr, " ");
    while (symbol_runner != NULL)
    {
        if (isOperator(symbol_runner[0]))
        {
            char type1 = topPtr->type;
            int symbol1 = pop(&topPtr);
            char type2 = topPtr->type;
            int symbol2 = pop(&topPtr);
            push(&topPtr, calculate(symbol1, symbol2, type1, type2, symbol_runner[0], 
                symbol_table, symbol_index, memory, instruction_counter, data_counter));
            topPtr->type = 'V';
        }else
        {
            if (symbol_runner[0] >= '0' && symbol_runner[0] <= '9')
            {
                findSymbolLocation(symbol_table, atoi(symbol_runner), 'C', data_counter, symbol_index);
                push(&topPtr, atoi(symbol_runner));
                topPtr->type = 'C';
            }else
            {
                findSymbolLocation(symbol_table, symbol_runner[0], 'V', data_counter, symbol_index);
                push(&topPtr, symbol_runner[0]);
                topPtr->type = 'V';
            }
        }
        symbol_runner = strtok(NULL, " ");
    }
}
