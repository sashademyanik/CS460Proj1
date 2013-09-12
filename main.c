/*
 * Eggshell
 * CS460
 */


#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#define DEBUG 1

char *command;
char **arguments;
int id;

//Method Main: runs loop giving user a prompt to enter a command and its arguments, until the keyword 'exit' is recieved
int main(int argc, char *argv[]){

	char buffer[500];
	int argumentCount = 0;
	char *argBuffer;
	int length = 0;

	while(1){

		printf("esh/:-> ");
		fgets(buffer,500,stdin); 
		
		command = strtok(buffer, " ");

		if(DEBUG){printf("Command: %s \n",command);}

		argBuffer = strtok(NULL," ");

		while(argBuffer != NULL){
			if(DEBUG){printf("%s \n", argBuffer);}

			length = strlen(argBuffer);

			if(DEBUG){printf("%d \n", length);}

			arguments[argumentCount] = (char*)malloc(sizeof (char)*length);	
			arguments[argumentCount] = strdup(argBuffer);

		        if(DEBUG) {printf("test\n");}

			argBuffer = strtok(NULL," ");
			argumentCount++;
		}

		if(DEBUG){
			printf("Command: %s  Arguments: %s \n", command, arguments);
	
		
		}

	}
	


	return 0;
}

