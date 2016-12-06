#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "funcLib.h"
/*
struct miniMIPSInstruction{
        char* name;
        char* format;
        char* opCode;
        char* func;
};
*/
void getInstructionLib(struct miniMIPSInstruction *instruction)
{
	// assign instruction names
	instruction[0].name = "and";
	instruction[1].name = "or";
	instruction[2].name = "xor";
	instruction[3].name = "nor";
	instruction[4].name = "add";
	instruction[5].name = "sub";
	instruction[6].name = "slt";
	instruction[7].name = "andi";
	instruction[8].name = "ori";
	instruction[9].name = "xori";
	instruction[10].name = "nori";
	instruction[11].name = "addi";
	instruction[12].name = "subi";
	instruction[13].name = "lw";
	instruction[14].name = "sw";
	instruction[15].name = "j";
	instruction[16].name = "beq";

	// assign format
	int i;
	for(i=0;i<7;i++) instruction[i].format = "R";
	for(i=7;i<17;i++) instruction[i].format = "I";
	instruction[15].format = "J";

	// assign opCode
	for(i=0;i<7;i++) instruction[i].opCode = "0000";
	instruction[7].opCode = "0001";
	instruction[8].opCode = "0010";
	instruction[9].opCode = "0100";
	instruction[10].opCode = "0110";
	instruction[11].opCode = "1000";
	instruction[12].opCode = "1001";
	instruction[13].opCode = "1010";
	instruction[14].opCode = "1011";
	instruction[15].opCode = "1100";
	instruction[16].opCode = "1101";

	// assign function Code
	instruction[0].func = "00000";
	instruction[1].func = "00010";
	instruction[2].func = "00100";
	instruction[3].func = "00110";
	instruction[4].func = "01000";
	instruction[5].func = "01001";
	instruction[6].func = "01011";
}

// Convert Binary string to decimal integer
int binStr2Int(char* binStr)
{
	return (int)strtol(binStr,NULL,2);
}


// After reading the first word, return its instruction index.
int findInstruction(char* instrucName,struct miniMIPSInstruction *instruction)
{
	int i = 0;
	for(i=0;i<17;i++)
	{
		if(strcmp(instrucName,instruction[i].name) == 0) return i;
	}
	printf("The instruction is not found! Now exit!\n");
	return -1;
}

// Get 2's complimentary and represent it in decimal number.
int TwosComplementary(int num)
{
	if(num >= 0) return num;
	else return 256 - num;
}

// translate line to decimal code if it is in r-type format
// (for "and, or, xor, nor, add, sub, slt")
int r_code(char* buffer,int instrID,struct miniMIPSInstruction *instruction)
{
	int instrCode;
	int opcode,rd,rs,rt,funct;
	char* instructionToken;
	char* rdToken;
	char* rsToken;
	char* rtToken;
	
	instructionToken = strtok(buffer," \t:,");
	rdToken = strtok(NULL," \t:$t,");
	rsToken = strtok(NULL," \t:$t,");
	rtToken = strtok(NULL," \t:$t,");
	
	opcode = binStr2Int(instruction[instrID].opCode)<<14;
	rd = atoi(rdToken)<<11;
	rs = atoi(rsToken)<<8;
	rt = atoi(rtToken)<<5;
	funct = binStr2Int(instruction[instrID].func);
	instrCode = opcode + rd + rs + rt +funct;

	return instrCode;
}


// translate line to decimal code if it is in i-type format1
// (for "andi, ori, xori, nori, addi, subi")
int i_code_1(char* buffer,int instrID,struct miniMIPSInstruction *instruction)
{
	int instrCode;
	int opcode,rd,rs,immediate;
	char* instructionToken;
	char* rdToken;
	char* rsToken;
	char* immediateToken;

	instructionToken = strtok(buffer," \t:,");
	rdToken = strtok(NULL," \t:$t,");
	rsToken = strtok(NULL," \t:$t,");
	immediateToken = strtok(NULL," \t:$t,");	

	opcode = binStr2Int(instruction[instrID].opCode)<<14;
	rd = atoi(rdToken)<<11;
	rs = atoi(rsToken)<<8;
	immediate = TwosComplementary(atoi(immediateToken));
	
	instrCode = opcode + rd + rs + immediate;
	return instrCode;
}


// translate line to decimal code if it is in i-type format2
// (for "lw, sw")
int i_code_2(char* buffer,int instrID,struct miniMIPSInstruction *instruction)
{
	int instrCode;
	int opcode,rd,rs,immediate;
	char* instructionToken;
	char* rdToken;
	char* rsToken;
	char* immediateToken;

	instructionToken = strtok(buffer," \t:,");
	rdToken = strtok(NULL," \t:$t,/(/)");
	immediateToken = strtok(NULL," \t:$t,/(/)");
	rsToken = strtok(NULL," \t:$t,/(/)");

	opcode = binStr2Int(instruction[instrID].opCode)<<14;
	rd = atoi(rdToken)<<11;
	rs = atoi(rsToken)<<8;
	immediate = TwosComplementary(atoi(immediateToken));

	instrCode = opcode + rd + rs + immediate;
	return instrCode;
}



// translate line to decimal code if it is in i-type format3
// (for "beq")
int i_code_3(char* buffer, int instrID, int labelPosition,struct miniMIPSInstruction *instruction) 
{
	int instrCode;
	int opcode, rd,rs;
	char* instructionToken;
	char* rdToken;
	char* rsToken;

	instructionToken = strtok(buffer," :,");
	rdToken = strtok(NULL," \t:$t,");
	rsToken = strtok(NULL," \t:$t,");

	opcode = binStr2Int(instruction[instrID].opCode)<<14;
	rd = atoi(rdToken)<<11;
	rs = atoi(rsToken)<<8;
	
	instrCode = opcode + rd + rs + labelPosition;
	return instrCode;
}


// translate line to decimal code if it is in j-type format
// (for "jump")
int j_code(char* buffer, int instrID, int labelPosition,struct miniMIPSInstruction *instruction)
{
	int instrCode;
	int opcode;

	opcode = binStr2Int(instruction[instrID].opCode)<<14;
	instrCode = opcode + labelPosition;
	return instrCode;
}





//Translate a arbitrary line to decimal code
int getCode(char* buffer,int instrID, int labelPosition,struct miniMIPSInstruction *instruction)
{
	if(instrID<7) return r_code(buffer,instrID,instruction);
	if(instrID>6 && instrID<13) return i_code_1(buffer,instrID,instruction);
	if(instrID == 13|instrID == 14) return i_code_2(buffer,instrID,instruction);
	if(instrID == 16) return i_code_3(buffer, instrID, 0,instruction);
	if(instrID == 15) return j_code(buffer, instrID,labelPosition,instruction);
	printf("No Code is obtained!Error!\n");
	return -1;
}

