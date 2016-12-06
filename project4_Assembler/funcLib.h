#ifndef FUNCLIB_H
#define FUNCLIB_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct miniMIPSInstruction{
	char* name;
	char* format;
	char* opCode;
	char* func;
};

// Get instruction table.
void getInstructionLib(struct miniMIPSInstruction *instruction);

// Convert Binary string to decimal integer
int binStr2Int(char* binStr);


// After reading the first word, return its instruction index.
int findInstruction(char* instrucName,struct miniMIPSInstruction *instruction);

// Get 2's complimentary and represent it in decimal number.
int TwosComplementary(int num);

// translate line to decimal code if it is in r-type format
// (for "and, or, xor, nor, add, sub, slt")
int r_code(char* buffer,int instrID,struct miniMIPSInstruction *instruction);

// translate line to decimal code if it is in i-type format1
// (for "andi, ori, xori, nori, addi, subi")
int i_code_1(char* buffer,int instrID,struct miniMIPSInstruction *instruction);

// translate line to decimal code if it is in i-type format2
// (for "lw, sw")
int i_code_2(char* buffer,int instrID,struct miniMIPSInstruction *instruction);

// translate line to decimal code if it is in i-type format3
// (for "beq")
int i_code_3(char* buffer, int instrID, int labelPosition,struct miniMIPSInstruction *instruction); 

// translate line to decimal code if it is in j-type format
// (for "jump")
int j_code(char* buffer, int instrID, int labelPosition,struct miniMIPSInstruction *instruction);

//Translate a arbitrary line to decimal code
int getCode(char* buffer,int instrID, int labelPosition,struct miniMIPSInstruction *instruction);

#endif
