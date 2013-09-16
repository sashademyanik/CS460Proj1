/* 
 * Corey Amoruso, Michael Swiger, Sasha Demyanik
 * Eggshell
 * CS460 Project 1
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>


#define DEBUG      0
#define DIRSIZE    1024
#define BUFSIZE    500
#define MAXARG     5
#define MAXPATH    5
#define PATH_DELIM ":"
#define ARG_DELIM  " \n\r"


/*
 * Check if specified file exists.
 * Code snippet from: http://stackoverflow.com/a/230070
 */
int fileExists(const char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}


/* Merge a path and a filename into one string. */
char *mergePath(const char *path, const char *file) {
	int totalLen = strlen(path) + strlen(file) + 2;
	int pathLen = strlen(path);
	
	/* Allocate merged string to be length of path+file+slash+null. */
	char *merged = malloc(sizeof(char) * totalLen);
	memset(merged, '\0', totalLen);
	
	/* Copy path and file to merged string. */
	strncpy(merged, path, pathLen);
	strncpy(&merged[pathLen + 1], file, strlen(file));
	
	/* Add slash between path and file. */
	merged[pathLen] = '/';
	
	return merged;
}


/* Check the given path for the given file. */
int checkPath(const char *path, const char *file) {
	char *merged = mergePath(path, file);
	
	/* Check if merged path exists. */
	int r = fileExists(merged);
	
	free(merged);
	
	return r;
}


/*
 * Main Function: runs loop giving user a prompt to enter a command and its 
 * arguments, until the keyword 'exit' is recieved
 */
int main(int argc, char *argv[]){
	char buffer[BUFSIZE];        /* input buffer with command + args */
	char cwd[DIRSIZE];           /* current working directory */
	char *path = getenv("PATH"); /* PATH env var */
	char *execPath;              /* PATH where the command is located */	
	char *token;                 /* used for tokenizing args and paths */
	int pid;                     /* parent process id (for forking) */
	
	/* List of arguments parsed from buffer. */
	char **arguments = malloc(sizeof(char*) * MAXARG);
	int maxArgs = MAXARG;
	int numArgs = 0;
	
	/* List of paths parsed from PATH. */
	char **paths = malloc(sizeof(char*) * MAXPATH);
	int numPaths = 0;
	int maxPaths = MAXPATH;

	/* Split the path env var into an array with individual paths. */
	token = strtok(path, PATH_DELIM);
	
	do {
		/* Double the size of the paths array if it's full. */
		if (numPaths >= maxPaths - 1) {
			maxPaths *= 2;
			paths = realloc(paths, sizeof(char*) * maxPaths);
		}
		
		paths[numPaths] = token;
		numPaths++;
	} while ((token = strtok(NULL, PATH_DELIM)));
	
	/* Debug info for checking paths. */
	if (DEBUG) {
		for (int i = 0; i < numPaths; i++) {
			printf("Path %d: %s\n", i, paths[i]);
		}
	}

	/* Continuously prompt for and execute commands until "exit" is input. */
	while (1){
		numArgs = 0;     /* reset current arg count */
		execPath = NULL; /* reset execution path */
		
		/* Reset CWD. */
		getcwd(cwd, DIRSIZE);
		
		/* Null out arguments list. */
		for (int i = 0; i < maxArgs; i++) {
			arguments[i] = NULL;
		}

		/* Print prompt and read in command line. */
		
		printf("esh/:-> ");
		fgets(buffer, BUFSIZE, stdin);

		/* Pull the command from the input buffer. */
		token = strtok(buffer, ARG_DELIM);
		
		/* Tokenize arguments from buffer and store in arguments array. */
		do {
			/* Double the size of the arguments array if it's full. */
			if(token != NULL && numArgs >= maxArgs - 1){
				maxArgs *= 2;
				arguments = realloc(arguments, (maxArgs * sizeof(char*)));
			}
			
			arguments[numArgs] = token;
			numArgs++;
		} while ((token = strtok(NULL, ARG_DELIM)));
		
		/* Check for exit, break the loop if specified. */
		if(!strcmp(arguments[0], "exit")){
			free(arguments);
			break;
		}

		/* Debug info for verifying command and arguments. */
		if (DEBUG){
			printf("Command: %s \n", arguments[0]);
			for (int i = 1; i < numArgs; i++){
				printf("Argument %d: %s \n", i, arguments[i]);
			}
		}
		
		/* Check if command exists in environment paths. */
		for (int i = 0; i < numPaths; i++) {
			if (checkPath(paths[i], arguments[0])) {
				execPath = paths[i];
			}
		}
		
		/* Check if command exists in cwd (if not found in paths). */
		if (!execPath && checkPath(cwd, arguments[0])) {
			execPath = cwd;
		}
		
		/* If the file was not found at any path, check for new input. */
		if (!execPath) {
			printf("Command `%s' not found.\n", arguments[0]);
			continue;
		}
		
		/* If it was found, fork and execute. */
		pid = fork();
		
		/* 
		 * If this is the parent branch and & isn't last arg, wait for child
		 * to die.
		 */
		if (pid != 0) {
			if(strcmp("&", arguments[numArgs - 1]) == 0){
				continue;
			}else{
				wait(NULL);
			}
			
		}
		
		/* If this is the child branch, execute the command. */
		else {
			if (strcmp("&", arguments[numArgs - 1]) == 0) {
				arguments[numArgs - 1] = NULL;
			}
			
			char *merged = mergePath(execPath, arguments[0]);
			execv(merged, arguments);
			exit(0);
		}
	}
	
	return 0;
}
