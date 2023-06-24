#include "../inc/virtual_machine.h"

void load(int simpletron_memory[], char filename[])
{
    FILE* programPtr = NULL;

    printf("%s\n", filename);
    programPtr = fopen(filename, "r");
    int command = 0;
    int address = 0;
    while (fscanf(programPtr, "%d", &command) != EOF)
    {
        printf("%02X ? %04d\n", address, command);
        simpletron_memory[address] = command;
        ++address;
    }
    printf("*** Program loading completed ***\n");    
    fclose(programPtr);
}

void execute(int simpletron_memory[], word *accumalator, int *instruction_counter, 
    word *instruction_register, int *operation_code, int *operand)
{
    printf("*** Program execution begins ***\n");
    int status_flag = CONTINUE;
    for (*instruction_counter = 0; *instruction_counter < MEMORY_SIZE && status_flag == CONTINUE; ++(*instruction_counter))
    {
        *instruction_register = simpletron_memory[*instruction_counter];
        *operation_code = *instruction_register / MEMORY_SIZE;
        *operand = *instruction_register % MEMORY_SIZE;
        switch (*operation_code)
        {
        case READ:
            scanf("%d", &simpletron_memory[*operand]);        
            while (simpletron_memory[*operand] > 99999 || simpletron_memory[*operand] < -99999)
            {
                printf("*** Invalid value, please enter value in range [-99999, +99999] ***\n"); 
                scanf("%d", &simpletron_memory[*operand]);       
            }
            break;
        case WRITE:
            printf("%05X\n", simpletron_memory[*operand]);
            break;
        case NEWLINE:
            putchar('\n');
            break;
        case READ_STR:
            char str[MAX_STR_SIZE];
            scanf("%s", str);
            size_t str_len = strlen(str);
            simpletron_memory[STRING_START_ADDR] = str_len * 100;
            for (int i = 0, address = STRING_START_ADDR; i < str_len; ++i)
            {
                simpletron_memory[address] += str[i] - ANCHOR_STRING_CHARATER;
                if (i < str_len - 1)
                {
                    ++address;
                    ++i;
                    simpletron_memory[address] = (str[i] - ANCHOR_STRING_CHARATER) * 100;
                }
            }
            break;
        case WRITE_STR:
            str_len = simpletron_memory[STRING_START_ADDR] / 100;
            for (int i = 0, address = STRING_START_ADDR; i < str_len; ++i)
            {
                putchar(simpletron_memory[address] % 100 + ANCHOR_STRING_CHARATER);
                if (i < str_len - 1)
                {
                    ++address;
                    ++i;
                    putchar(simpletron_memory[address] / 100 + ANCHOR_STRING_CHARATER);
                }
            }
            break;
        case LOAD:
            *accumalator = simpletron_memory[*operand];
            break;
        case STORE:
            simpletron_memory[*operand] = *accumalator;
            break;
        case ADD:
            *accumalator += simpletron_memory[*operand];
            break;
        case SUBSTRACT:
            *accumalator -= simpletron_memory[*operand];
            break;
        case DIVIDE:
            if (simpletron_memory[*operand] == 0)
            {
                printf("*** Attempt to divide by zero                  ***\n");
                status_flag = EXECPETION;
            }else
            {
                *accumalator /= simpletron_memory[*operand];
            }
            break;
        case MULTIPLY:
            *accumalator *= simpletron_memory[*operand];
            break;
        case REMAINDER:
            if (simpletron_memory[*operand] == 0)
            {
                printf("*** Attempt to divide by zero                  ***\n");
                status_flag = EXECPETION;
            }else
            {
                *accumalator %= simpletron_memory[*operand];
            }
            break;
        case EXPONENT:
            int base = *accumalator;
            *accumalator = 1;
            for (int i = 0; i < simpletron_memory[*operand]; ++i)
            {
                *accumalator *= base;
            }
            break;
        case BRANCH:
            *instruction_counter = *operand - 1; //for end-of-loop increment
            break;
        case BRANCHNEG:
            if (*accumalator < 0)
            {
                *instruction_counter = *operand - 1;
            }
            break;
        case BRANCHZERO:
            if (*accumalator == 0)
            {
                *instruction_counter = *operand - 1;
            }
            break;
        case HALT:
            status_flag = STOP;
            break;
        default:
            break;
        }
        if (*accumalator < -99999 || *accumalator > 99999)
        {
            printf("*** Arithmetic overflow exception              ***\n");
            status_flag = EXECPETION;
        }
    }
    if (status_flag == STOP)
    {
        printf("*** Simpletron execution terminated ***\n");
    }else if (status_flag == EXECPETION)
    {
        printf("*** Simpletron execution abnormally terminated ***\n");
    }
}

void dump(int simpletron_memory[], word accumalator, int instruction_counter,
    word instruction_register, int operation_code, int operand)
{
    printf("REGISTER:\n");
    printf("%-27s%08X\n", "accumulator", accumalator);
    printf("%-32s%03X\n", "instructionCounter", instruction_counter);
    printf("%-30s%05X\n", "instructionRegister", instruction_register);
    printf("%-33s%02X\n", "operationCode", operation_code);
    printf("%-32s%03X\n", "operand", operand);
    
    printf("\nMEMORY:\n");
    printf("%11X", 0);
    for (int i = 1; i < 16; ++i)
    {
        printf("%8X", i);
    }
    for (int i = 0; i < 63; ++i)
    {
        printf("\n%3X", i * 16);
        for (int j = 0; j < 16 && i * 16 + j < 1000; ++j)
        {
            printf("   %05x", simpletron_memory[i * 16 + j]);
        }
    }
    putchar('\n');
}

int main()
{
    /*MEMORY*/
    int simpletron_memory [MEMORY_SIZE]; //Assume that user code obey word size
    memset(simpletron_memory, 0, sizeof (simpletron_memory));

    /*REGISTERS*/
    word accumalator = 0;
    int instruction_counter = 0;
    word instruction_register = 0;
    int operation_code = 0;
    int operand = 0;

    char c[30] = "97*(34 +a)";
    char d[30];
    struct tableEntry abc[MEMORY_SIZE];
    convertToPostfix(c, d);
    printf("%s\n", d);
    evaluatePostFixExpression(d, abc);

    char filename[MAX_FILE_NAME + RESOURCE_FOLDER + 1] = "../resource/";
    //prompt
    printf("***                   Welcome to Simpletron                  ***\n");
    printf("***                                                          ***\n");
    printf("*** Please enter your program filename (no more than 20      ***\n");
    printf("*** characters, located at resource folder).                 ***\n");
    printf("*** Type the sentinel -999999 to stop entering your program. ***\n");
    scanf("%s", filename + 12);

    load(simpletron_memory, filename);
    execute(simpletron_memory, &accumalator, &instruction_counter, &instruction_register, &operation_code, &operand);
    dump(simpletron_memory, accumalator, instruction_counter, instruction_register, operation_code, operand);
    return 0;
}