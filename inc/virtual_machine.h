#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

#include <stdio.h>
#include <string.h>
#include "compiler.h"

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

/*STATUS DEFINE*/
#define CONTINUE 0
#define STOP 1
#define EXECPETION 2

/*LIMITS DEFINE*/
#define MEMORY_SIZE 1000
#define MAX_STR_SIZE 400 // 2 * (1000 - 800)
#define WORD_SIZE 5
#define STOP_INPUT_CODE_SIZE 7
#define ANCHOR_STRING_CHARATER 'A'
#define MAX_FILE_NAME 20
#define RESOURCE_FOLDER 12
#define MAX_LENGTH_STATEMENT 100

/*SPECIAL ADDRESSED DEFINE*/
#define STRING_START_ADDR 800

/*TYPES DEFINE*/
typedef int word;

void load(int simpletron_memory[], char filename[]);

void execute(int simpletron_memory[], word *accumalator, int *instruction_counter, 
    word *instruction_register, int *operation_code, int *operand);

void dump(int simpletron_memory[], word accumalator, int instruction_counter,
    word instruction_register, int operation_code, int operand);

#endif