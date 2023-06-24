#ifndef VIRTUAL_MACHINE
#define VIRTUAL_MACHINE

#include <stdio.h>
#include <string.h>
#include "compiler.h"

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