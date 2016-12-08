#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
	char string[] = "RUN add 0.7 0.1";			//example of INSTRUCTION
	char space[2] = " ";						//delimiter that will separate the words in the STRING
	char *word;									
	int i;										
	
	typedef char String[11];					//ten will be the maxium lenght of each variable ex. 0.12345678
	String arrayString[4];						//there will be an array of "String" with 4 positions 
	
	word = strtok(string, space);
	
	for(i=0;i<4;i++){							//4 is the lenght of the words in the instruction

		strcpy(arrayString[i], word);		
		printf("%s\n",arrayString[i]);
		word = strtok(NULL, space);
	}
	
	return 0;	
}
