#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000
#define STACK_SIZE 100

int labelcnt = 0;

typedef struct
{
    char label[MAX_SIZE];
    int address;
} Label;

typedef struct
{
    char instruction[MAX_SIZE];
    char operand1[MAX_SIZE];
    char operand2[MAX_SIZE];
} Command;

typedef struct
{
    int AX, BX, CX, DX, AH, AL, BH, BL, CH, CL, DH, DL, DS, CS, SS;
    int zf;
} Register;

typedef struct
{
    int stack[STACK_SIZE];
    int top;
} Stack;

void initializeStack(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, int value)
{
    if (stack->top < STACK_SIZE - 1)
    {
        stack->stack[++stack->top] = value;
        printf("Pushed %d onto the stack\n", value);
    }
    else
    {
        printf("Stack overflow\n");
    }
}

int pop(Stack *stack)
{
    if (stack->top >= 0)
    {
        int value = stack->stack[stack->top--];
        printf("Popped %d from the stack\n", value);
        return value;
    }
    else
    {
        printf("Stack underflow\n");
        return -1;
    }
}

int isInvalid(char c)
{
    if (c >= '0' && c <= '9')
        return 0;
    if (c >= 'A' && c <= 'Z')
        return 0;
    if (c == '_')
        return 0;
    return 1;
}

void clean(char *token)
{
    int j = strlen(token) - 1;
    while (isInvalid(token[j]))
    {
        token[j--] = '\0';
    }
    return;
}

void parseLine(char line[], Command *command)
{
    // Handle other instructions
    int i = 0, j = 0;
    while (isInvalid(line[i]))
    {
        i++;
    }

    // Read instruction
    sscanf(line + i, "%s", command->instruction);
    i += strlen(command->instruction);

    // clean(command->instruction);
    // Skip spaces and tabs
    while (isInvalid(line[i]))
    {
        i++;
    }

    // Read operand1
    sscanf(line + i, "%s", command->operand1);
    i += strlen(command->operand1);
    // clean(command->operand1);
    // Skip spaces and tabs
    while (isInvalid(line[i]))
    {
        i++;
    }
    // Read operand2
    sscanf(line + i, "%s", command->operand2);
    // clean(command->operand2);
    return;
}

void printRegistersFile(Register *registers, FILE *outfile)
{
    fprintf(outfile, "---------------------------------------------------------------------------------\n");
    fprintf(outfile, "|\t\tAX: %d\t\t|", registers->AX);
    fprintf(outfile, "\t\tBX: %d\t\t|", registers->BX);
    fprintf(outfile, "\t\tCX: %d\t\t|", registers->CX);
    fprintf(outfile, "\t\tDX: %d\t\t|\n", registers->DX);
    fprintf(outfile, "|\tAH: %d\t", registers->AH);
    fprintf(outfile, "AL: %d\t|", registers->AL);
    fprintf(outfile, "\tBH: %d\t", registers->BH);
    fprintf(outfile, "BL: %d\t|", registers->BL);
    fprintf(outfile, "\tCH: %d\t", registers->CH);
    fprintf(outfile, "CL: %d\t|", registers->CL);
    fprintf(outfile, "\tDH: %d\t", registers->DH);
    fprintf(outfile, "DL: %d\t|\n", registers->DL);
    fprintf(outfile, "---------------------------------------------------------------------------------\n");
}

char *toString(int n)
{
    char *str = malloc(sizeof(char) * 5);
    int i = 3;
    while (n)
    {
        int d = n % 10;
        char ch = d + '0';
        str[i--] = ch;
        n /= 10;
    }
    while (i >= 0)
    {
        str[i--] = '0';
    }
    str[4] = '\0';
    return str;
}

char *address(char *operand, Label *labels)
{
    if (strcmp(operand, "AX") == 0)
        return "0100";
    else if (strcmp(operand, "BX") == 0)
        return "0200";
    else if (strcmp(operand, "CX") == 0)
        return "0300";
    else if (strcmp(operand, "DX") == 0)
        return "0400";
    else if (strcmp(operand, "AH") == 0)
        return "0100";
    else if (strcmp(operand, "BH") == 0)
        return "0200";
    else if (strcmp(operand, "CH") == 0)
        return "0300";
    else if (strcmp(operand, "DH") == 0)
        return "0400";
    else if (strcmp(operand, "AL") == 0)
        return "0500";
    else if (strcmp(operand, "BL") == 0)
        return "0600";
    else if (strcmp(operand, "CL") == 0)
        return "0700";
    else if (strcmp(operand, "DL") == 0)
        return "0800";
    else
    {
        for (int i = 0; i < labelcnt; i++)
        {
            if (strcmp(operand, labels[i].label) == 0)
            {
                return toString(labels[i].address);
            }
        }
    }
    return "0441";
}

void generateMachineCode(Command *command, FILE *outputFile, Label *labels)
{
    if (strcmp(command->instruction, "MOV") == 0)
    {
        fprintf(outputFile, "B8%s|", address(command->operand2, labels), command->operand1, command->operand2);
    }
    else if (strcmp(command->instruction, "INT") == 0)
    {
        fprintf(outputFile, "CD%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "JMP") == 0)
    {
        fprintf(outputFile, "EA%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "INC") == 0)
    {
        fprintf(outputFile, "FE%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "DEC") == 0)
    {
        fprintf(outputFile, "FF%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "ADD") == 0)
    {
        fprintf(outputFile, "F2%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "SUB") == 0)
    {
        fprintf(outputFile, "2D%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "MUL") == 0)
    {
        fprintf(outputFile, "F6%s|", address(command->operand1, labels), command->operand1);
    }
    else if (strcmp(command->instruction, "DIV") == 0)
    {
        fprintf(outputFile, "F7%s|", address(command->operand1, labels), command->operand1);
    }
}

void printLabels(Label *labels)
{
    printf("\n\n");
    for (int i = 0; i < labelcnt; i++)
    {
        printf("LabelIndex: %d\t", i);
        printf("Label: %s\t", labels[i].label);
        printf("Address: %d\n", labels[i].address);
    }
}

void printLabelsFile(Label *labels, FILE *outfile)
{
    for (int i = 0; i < labelcnt; i++)
    {
        fprintf(outfile, "Label: %s\t||\t", labels[i].label);
        fprintf(outfile, "Address: %d\n", labels[i].address);
        fprintf(outfile, "-------------------------------------------------------\n");
    }
}

void setValueRegister(Register *registers, char *operand1, char *operand2)
{
    int value = atoi(operand2);
    if (strcmp(operand1, "AH") == 0)
    {
        registers->AH = value;
    }
    else if (strcmp(operand1, "AL") == 0)
    {
        registers->AL = value;
    }
    else if (strcmp(operand1, "BH") == 0)
    {
        registers->BH = value;
    }
    else if (strcmp(operand1, "BL") == 0)
    {
        registers->BL = value;
    }
    else if (strcmp(operand1, "CH") == 0)
    {
        registers->CH = value;
    }
    else if (strcmp(operand1, "CL") == 0)
    {
        registers->CL = value;
    }
    else if (strcmp(operand1, "DH") == 0)
    {
        registers->DH = value;
    }
    else if (strcmp(operand1, "DL") == 0)
    {
        registers->DL = value;
    }
    else if (strcmp(operand1, "CX") == 0)
    {
        registers->CX = value;
    }
}

void executeCommand(FILE *file, FILE *outfile, Command *command, Register *registers, Label *labels, Stack *stack)
{
    printRegistersFile(registers, outfile);

    if (strcmp(command->instruction, "MOV") == 0)
    {
        registers->zf = 0;
        if (isdigit(command->operand2[0]) || (command->operand2[0] == '-' && isdigit(command->operand2[1])))
        {
            setValueRegister(registers, command->operand1, command->operand2);
        }
        else
        {
            int labelIndex = atoi(command->operand2 + 1);
            setValueRegister(registers, command->operand1, labels[labelIndex].label);
        }
    }

    else if (strcmp(command->instruction, "XOR") == 0)
    {
        registers->zf = 0;
        int regIndex1 = command->operand1[1] - '0';
        int regIndex2 = command->operand2[1] - '0';

        switch (regIndex1)
        {
        case 0:
            registers->AL ^= registers->AL;
            break;
        case 1:
            registers->BL ^= registers->BL;
            break;
        case 2:
            registers->CL ^= registers->CL;
            break;
        case 3:
            registers->DL ^= registers->DL;
            break;
        }

        switch (regIndex2)
        {
        case 0:
            registers->AL ^= registers->AL;
            break;
        case 1:
            registers->BL ^= registers->BL;
            break;
        case 2:
            registers->CL ^= registers->CL;
            break;
        case 3:
            registers->DL ^= registers->DL;
            break;
        }
    }

    else if (strcmp(command->instruction, "MUL") == 0)
    {
        registers->zf = 0;
        int regIndex1 = command->operand1[1] - '0';

        switch (regIndex1)
        {
        case 0:
            registers->AX *= registers->AX;
            break;
        case 1:
            registers->BX *= registers->BX;
            break;
        case 2:
            registers->CX *= registers->CX;
            break;
        case 3:
            registers->DX *= registers->DX;
            break;
        }
    }

    else if (strcmp(command->instruction, "DIV") == 0)
    {
        registers->zf = 0;
        int regIndex1 = command->operand1[1] - '0';

        switch (regIndex1)
        {
        case 0:
            if (registers->AX != 0)
                registers->AX /= registers->AX;
            else
                printf("Error: Division by zero\n");
            break;
        case 1:
            if (registers->BX != 0)
                registers->BX /= registers->BX;
            else
                printf("Error: Division by zero\n");
            break;
        case 2:
            if (registers->CX != 0)
                registers->CX /= registers->CX;
            else
                printf("Error: Division by zero\n");
            break;
        case 3:
            if (registers->DX != 0)
                registers->DX /= registers->DX;
            else
                printf("Error: Division by zero\n");
            break;
        }
    }
    else if (strcmp(command->instruction, "PUSH") == 0)
    {
        int regIndex = command->operand1[1] - '0';

        switch (regIndex)
        {
        case 0:
            push(stack, registers->AX);
            break;
        case 1:
            push(stack, registers->BX);
            break;
        case 2:
            push(stack, registers->CX);
            break;
        case 3:
            push(stack, registers->DX);
            break;
        }
    }

    else if (strcmp(command->instruction, "POP") == 0)
    {
        int regIndex = command->operand1[1] - '0';

        switch (regIndex)
        {
        case 0:
            registers->AX = pop(stack);
            break;
        case 1:
            registers->BX = pop(stack);
            break;
        case 2:
            registers->CX = pop(stack);
            break;
        case 3:
            registers->DX = pop(stack);
            break;
        }
    }

    else if (strcmp(command->instruction, "ADD") == 0)
    {
        registers->zf = 0;
        switch (command->operand1[0])
        {
        case 'A':
            registers->AH += registers->AL;
            break;
        case 'B':
            registers->BH += registers->BL;
            break;
        case 'C':
            registers->CH += registers->CL;
            break;
        case 'D':
            registers->DH += registers->DL;
            break;
        }
    }

    else if (strcmp(command->instruction, "SUB") == 0)
    {
        switch (command->operand1[0])
        {
        case 'A':
        {
            registers->AH -= registers->AL;
            if (registers->AH == 0)
                registers->zf = 1;
            break;
        }
        case 'B':
        {
            registers->BH -= registers->BL;
            if (registers->BH == 0)
                registers->zf = 1;
            break;
        }
        case 'C':
        {
            registers->CH -= registers->CL;
            if (registers->CH == 0)
                registers->zf = 1;
            break;
        }
        case 'D':
        {
            registers->DH -= registers->DL;
            if (registers->DH == 0)
                registers->zf = 1;
            break;
        }
        }
    }

    else if (strcmp(command->instruction, "INC") == 0)
    {
        registers->zf = 0;
        switch (command->operand1[0])
        {
        case 'A':
            registers->AL++;
            break;
        case 'B':
            registers->BL++;
            break;
        case 'C':
            registers->CL++;
            break;
        case 'D':
            registers->DL++;
            break;
        }
    }

    else if (strcmp(command->instruction, "DEC") == 0)
    {
        switch (command->operand1[0])
        {
        case 'A':
        {
            registers->AX--;
            if (registers->AX == 0)
                registers->zf = 1;
            break;
        }
        case 'B':
        {
            registers->BX--;
            if (registers->BX == 0)
                registers->zf = 1;
            break;
        }
        case 'C':
        {
            registers->CX--;
            if (registers->CX == 0)
                registers->zf = 1;
            break;
        }
        case 'D':
        {
            registers->DX--;
            if (registers->DX == 0)
                registers->zf = 1;
            break;
        }
        }
    }

    else if (strcmp(command->instruction, "INT") == 0)
    {
        if (strcmp(command->operand1, "21H") == 0 && registers->AH == 2)
        {
            printf("Output: %d\n", ((int)(registers->DL) - '0'));
        }
    }

    else if (strcmp(command->instruction, "JMP") == 0)
    {
        int labelIndex = 0;
        int registerValue = 0;
        printf("Jumping to label %s\n", labels[labelIndex].label);

        // Set the file pointer to the line after the specified label
        fseek(file, labels[labelIndex].address, SEEK_SET);
    }

    else if (strcmp(command->instruction, "LEA") == 0)
    {
        int regIndex = atoi(command->operand2 + 2);
        int regIndex2 = command->operand1[1] - '0';
        switch (regIndex)
        {
        case 0:
            registers->AH = registers->AL;
            break;
        case 1:
            registers->BH = registers->BL;
            break;
        case 2:
            registers->CH = registers->CL;
            break;
        case 3:
            registers->DH = registers->DL;
            break;
        }
        printf("Performing LEA %s, %s\n", command->operand1, command->operand2);
    }

    else if (strcmp(command->instruction, "PROMPT") == 0)
    {
        int regIndex = command->operand1[1] - '0';
    }
}

void Assembler(FILE *file, FILE *outlabelfile, FILE *outfilereg, Label *labels, Register *registers, Stack *stack)
{
    char line[MAX_SIZE];
    int address = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        Command command;
        printf("\nLine Read: %s", line);
        if (line[0] == ';')
        {
            printf("\nIt is a comment, ignored.\n");
            continue; // Skip comments
        }

        else if (strstr(line, "MACRO") != NULL)
        {
            printf("\nA macro has been encountered! Entering macro!\n");
            parseLine(line, &command);
            strcpy(labels[labelcnt].label, command.instruction);
            labels[labelcnt].address = ftell(file);
            labelcnt++;
            printf("Label Count: %d", labelcnt);
        }

        else if (strstr(line, "END MAIN") != NULL)
        {
            printf("\nExecuting instruction: END MAIN");
            printf("\nWe have completed parsing the file!\nProgram is being terminated!\n");
            break;
        }

        if (strstr(line, ":") != NULL)
        {
            printf("A label has been encountered!\n");
            parseLine(line, &command);
            strcpy(labels[labelcnt].label, command.instruction);
            labels[labelcnt].address = ftell(file);
            labelcnt++;
            printf("Label Count: %d", labelcnt);
        }
        else
        {
            parseLine(line, &command);
            printf("\nExecuting following command::\n\t\tInstruction: %s\t\tOperand 1: %s\t\t Operand 2: %s\n", command.instruction, command.operand1, command.operand2);
        }

        printRegistersFile(registers, outfilereg);
        FILE *outfile = fopen("ObjectCode.txt", "a");
        generateMachineCode(&command, outfile, labels);
        fclose(outfile);
        address++;
    }

    printLabels(labels);
    FILE *labelsfile = fopen("Labels.txt", "a");
    printLabelsFile(labels, labelsfile);
    fclose(labelsfile);
}

int main()
{

    FILE *file = fopen("Calculator.asm", "r");
    if (file == NULL)
    {
        perror("Error faced in opening ASM file!");
        return 1;
    }

    FILE *outregfile = fopen("RegisterTable.txt", "a");
    if (outregfile == NULL)
    {
        perror("Error faced in creating register file!");
        return 1;
    }

    FILE *outlabelfile = fopen("Labels.txt", "a");
    if (outlabelfile == NULL)
    {
        perror("Error faced in creating label file!");
        return 1;
    }

    Label labels[MAX_SIZE];
    Register registers = {0};
    Stack stack;
    initializeStack(&stack);

    Assembler(file, outlabelfile, outregfile, labels, &registers, &stack);

    fclose(file);
    fclose(outregfile);
    fclose(outlabelfile);

    return 0;
}