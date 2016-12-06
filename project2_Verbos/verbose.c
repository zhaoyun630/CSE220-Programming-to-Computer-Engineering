#include <stdio.h>
#include <string.h>

#define MAX_USER 100
#define MAX_USERNAME_LENGTH 50
#define MAX_BUFFER_LENGTH 500

int main(int argc, char* argv[])
{
	FILE* textfile;
	char buffer[MAX_BUFFER_LENGTH];
	char *token;
	int numWords[MAX_USER] = {0};
	int numLetters[MAX_USER] = {0};
	int numMessage[MAX_USER] = {0};
	int i;
	char userName[MAX_USER][MAX_USERNAME_LENGTH];
	char* ret;

	// Check if the minimum inputs are satisfied.
	// In this case, we need at least one file and one user name to show chat statistics.
	if(argc < 2)
	{
		printf("No file or user name are provided.\n");
		printf("Please give at least one file and one user name.\n");
		return;
	}
	if(argc > 1 && argc < 3)
        { 
                printf("No user name is provided.\n");
		printf("Please give at least one user name.\n");
		return;
        }

        
	textfile = fopen(argv[1],"r");
	if(textfile == 0)
	{
		printf("Unable to open file. \n");
		return 0;
	}


	// Create <username> pattern.
        for(i=0;i < argc - 2;i++)
        {
                strcat(strcat(strcat(userName[i],"<"),argv[i+2]),">");
        }

	
	while(!feof(textfile))
	{
		fgets(buffer,MAX_BUFFER_LENGTH,textfile); //get line from file
		for(i=0;i < argc-2;i++)
		{
			ret = strstr(buffer,userName[i]); //check if <username> is in the line
			if(ret != NULL)
			{
			        numMessage[i] += 1;
                		token = strtok(ret," ,.?!-:;\"\\&@#$%^\t\r\n");
                		while(token != NULL)
               			{
                                        numWords[i]++;
                                        numLetters[i] += strlen(token);                        	
					token = strtok(NULL," ,.?!-:;\"\\&@#$%^\t\r\n");
                		}

				// As string ret starts from <username> which should not be taken account,
				// so we perform a substraction in below to exclude it.
				numWords[i] = numWords[i] -1;
				numLetters[i] = numLetters[i] - strlen(userName[i]);
				break;
			}
		}
		

	}
	
//	print user chat statistics.	
	for(i=0;i < argc -2;i++)
	{
		printf("User %s:\n",argv[i+2]);
//		printf("     Username is %s\n",userName[i]);
//		printf("     numMessage = %d\n",numMessage[i]);
//		printf("     Total number of letters: %d\n", numLetters[i]);
//		printf("     Total number of words: %d\n", numWords[i]);
		printf("     Average Message Length (Characters): %.3f\n",(float)numLetters[i]/numMessage[i]);
		printf("     Average Message Length (Words): %.3f\n",(float)numWords[i]/numMessage[i]);
		printf("     Average Word Length: %.3f\n", (float)numLetters[i]/numWords[i]);
	}

	fclose(textfile);
	return 0;
}

