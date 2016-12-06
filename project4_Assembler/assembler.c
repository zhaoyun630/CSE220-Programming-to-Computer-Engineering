#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "funcLib.h"

int main(int argc,char* argv[])
{
	FILE* inputFile;
	FILE* outputFile;
	char buffer[1000];
	char buffer2[1000];
	char* token;
	int i;
	int lineNum = 0;
	int loopNum = 0;
	int loopLine = 0;
	int instrID;
	int instrCode[100];
	int exitNum = 0;
	int exitLine = 0;
	int beqNum = 0;
	int beqLine = 0;
	struct miniMIPSInstruction instruction[17];
	inputFile = fopen(argv[1],"r");
	outputFile = fopen(argv[2],"w");
	
	// Check if a file is provided in command line	
	if(argc < 3)
	{
		printf("Please give a input file and output file!\n");
		return 0;
	}

	// Check if input file is valid
	if(inputFile == NULL)
	{
		printf("Unable to open file: %s\n",argv[1]);
		return 0;
	}

	// Check if an output file is created properly.
	if(outputFile == NULL)
	{
		printf("Failed to create file!\n");
		return 0;
	}

	// Write the first line of Machine code
	fprintf(outputFile,"v2.0 raw\n");

	// Load instruction table
	getInstructionLib(instruction);
	
	// get loop and exit position

	// Read file and translate it to machine code
	while(!feof(inputFile))
	{
		fgets(buffer2,1000,inputFile);
		strcpy(buffer,buffer2);
		token = strtok(buffer2, " \t:,");
		// get code for each line
		if(strcmp(token,"beq") == 0) 
			beqLine = lineNum;

		if(strcmp(token,"Loop") == 0)
		{
			loopLine = lineNum;
			loopNum++;
		}
		else if(strcmp(token,"Exit") == 0)
		{
			exitLine = lineNum;
			exitNum++;
			break;
		}
		else 
		{
			instrID = findInstruction(token,instruction);
			instrCode[lineNum-loopNum-exitNum] = getCode(buffer, instrID,loopLine,instruction);
		}
		lineNum++;

	}

	// add the Exit line number to Hex code for line starts with beq.
	// My function initially take all Exit as 0.	
	instrCode[beqLine - 1] += exitLine;

	for(i=0;i<lineNum-1;i++) fprintf(outputFile,"%05X ", instrCode[i]);
	fprintf(outputFile,"\n");
	fclose(inputFile);
	fclose(outputFile);
	return 0;
}
