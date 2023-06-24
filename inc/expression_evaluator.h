#ifndef EXPRESSION_EVALUATOR
#define EXPRESSION_EVALUATOR

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symbol.h"

/*LIMITS DEFINE*/
#define MEMORY_SIZE 1000

struct stackNode {
    int data;
    char type;//only 'C' or 'V' of symbol type
    struct stackNode *nextPtr;
};

typedef struct stackNode StackNode;
typedef StackNode* StackNodePtr;

void convertToPostfix(char infix[], char postfix[]);
bool isOperator(int c);
int precedence(int operator1, int operator2);
void push(StackNodePtr *topPtr, int value);
int pop(StackNodePtr *topPtr);
int stackTop(StackNodePtr topPtr);
bool isEmpty(StackNodePtr topPtr);
void printStack(StackNodePtr topPtr);

int calculate(int op1, int op2, char op1_type, char op2_type, int operation, struct tableEntry symbol_table[]);
void evaluatePostFixExpression(char *exptr, struct tableEntry symbol_table[]);

#endif