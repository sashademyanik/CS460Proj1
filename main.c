/*
 * Eggshell
 * CS460
 */


#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#define DEBUG 1
#define MAXARG 5

char *command;
char **arguments;
int id;

void initArgArray(void);


//Method Main: runs loop giving user a prompt to enter a command and its arguments, until the keyword 'exit' is recieved
int main(int argc, char *argv[]){

	char buffer[500];
	int argumentCount = 0;
	char *argBuffer;


	while(1){

		int maxArg = MAXARG;   //vriable used to resize argument array in the case more than MAXARG arguments are supplied
		initArgArray();        //initiates the argument array

		printf("esh/:-> ");    //print prompt
		fgets(buffer,500,stdin); //read in command and arguments
		argumentCount = 0;     //set argument count to 0

		command = strtok(buffer, " \n\r");  //tokenize command from input buffer

		argBuffer = strtok(NULL," \n\r");  //tokenize first argument (if not present, returns NULL)


		//check for exit command, breaks from loop
		if(!strcmp(command,"exit")){
			free(arguments);
			break;
		}

		//loop to tokenize arguments and store them in arguments array
		while(argBuffer != NULL){
			arguments[argumentCount] = strdup(argBuffer);  //assign argument to array
			argBuffer = strtok(NULL," \n\r");              //tokenize next argument
			argumentCount++;                               //increment argument counter
		

			//doubles the size of the argument array when more than MAXARG arguments are entered.	 
			if(argBuffer != NULL && argumentCount > maxArg){ //checks to make sure another arg exists
				maxArg = (argumentCount - 1) * 2; //set size of new array (double the original)
				arguments =  realloc(arguments,(maxArg * sizeof(char*))); //reallocate arguments array
			}
		}


              //---------------------------------------------------------------------
	     // test case for verifying command and arguments	                  //|
	  	if(DEBUG){                                                        //|
			printf("Command: %s \n", command);			  //|
			for(int i = 0; i < argumentCount; i++){                   //|
				printf("Argument %d: %s \n", i, arguments[i]);    //|
			}                                                         //|
		}                                                                 //|
		//--------------------------------------------------------------------


		//Fork child processs
		//if(id = fork()){                      (un-comment this line)
			//parent portion of fork	
	//	}
		//else{					(and this one)
			//child prtion of fork

	//	}

		free(arguments);  //free argument array
	}
	
	return 0;
}

//------------------------------------------------------------------------------------------------------------------
/*Method initArgArray: allocates memory for arguments array and init values to NULL*/
void initArgArray(void){
	arguments = malloc(MAXARG * sizeof(char*));
	for(int i = 0; i < MAXARG; i++){
		arguments[i] = NULL;
	}
}



