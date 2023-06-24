#ifndef COMPILER
#define COMPILER

#include "symbol.h"
#include "expression_evaluator.h"

/* OPCODE DEFINE*/
#define READ 10
#define WRITE 11
#define NEWLINE 12
#define READ_STR 13
#define WRITE_STR 14
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBSTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define REMAINDER 34
#define EXPONENT 35
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43

/*LIMIT DEFINE*/
#define MEMORY_SIZE 1000
#define MAX_STATEMENT_SIZE 200

void compile(int memory[], int *instruction_counter, char *source_code, char *sml_code);

#endif