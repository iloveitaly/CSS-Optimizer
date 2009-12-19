/*
 Application: CSS Optimizer
 Copyright (C) 2005  Michael Bianco <software@mabwebdesign.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#import <stdio.h>
#import <stdlib.h>
#import <string.h>
#import <sysexits.h>

#import "dataStructures.h"
#import "constants.h"
#import "stringFunctions.h"
#import "fileSystemFunctions.h"
#import "fileStripperFunctions.h"
#import "argumentHandlerFunctions.h"
#import "recursiveFunctions.h"

fileLink *gRoot = NULL; //global root variable for the recursive directory stuff
commandLineArguments gOptions; //global command line options variable

int main (int argc, const char *argv[]) {
#if DEBUG >= 1
	printf("[main()] Argc:%i\n", argc);
#endif
	/**
	 *argv[0] == the launching path of the application
	 *argv[1] == the file path of the file to process
	 *argv[2] == the name/directorie/name & directorie/name & directorie w-out extension/name w-out extension of the processed file ouput file	
	 **/

	//check for command line arguments
	extern commandLineArguments gOptions;
	checkForAlternateCommands(argv, argc);
	gOptions = checkForOptions(argv, &argc);
	
#if DEBUG >= 1
	printf("==== Argument information ====\n");
	printf("Argv[1] is:	%s\n", argv[1]);
	printf("Argv[2] is:	%s\n", argv[2]);
	printf("Argc is:	%i\n", argc);
	printf("==============================\n");
#endif
	
	if((argc == 1 && !gOptions.processStdin) || (argc == 1 && !gOptions.outputStdout)) {//check for invalid amount of arguments
		ARG_ERR("Invalid number of arguments");
	} if(!gOptions.processStdin && !gOptions.outputStdout && !fileExists(argv[1])) {//checks if the file exists or if its a directory
		ARG_ERR("Specified file does not exist");
	} else if(!gOptions.processStdin && !gOptions.outputStdout && isDir(argv[1]) && !gOptions.recurseDir) {//checks to make sure that we have a valid directory if -R is specified
		ARG_ERR("Directory specified but no directory option (-r) found");
	} else if(gOptions.recurseDir && !isDir(argv[1])) {//make sure if the -r option is specified that the inputfile is an argument
		ARG_ERR("You must specify a directory if using the -r option");
	} else if(argc == 3 && !strcmp(argv[1], argv[2])) {//if we have an inputfile and an outputfile but they are both the same
		ARG_ERR("The input and output paths cannot be the same! Use the -n option instead!");
	}
	
	if(gOptions.processStdin) {//-i option is specified
		if(gOptions.outputStdout) {//-io options
			char *outputCSS = processCSSFromInput();
			fwrite(outputCSS, sizeof(char), strlen(outputCSS), stdout);
			free(outputCSS);
			
			return EX_OK;
		} else {//just -i
			char *outputCSS = processCSSFromInput();
			FILE *newFile = fopen(argv[1], "w");
			if(newFile == NULL) {printf("Error opening %s\n", argv[1]); return 1;}
			
			fwrite(outputCSS, sizeof(char), strlen(outputCSS), newFile);
			fclose(newFile);
			free(outputCSS);
			
			return EX_OK;
		}
	} else if(gOptions.outputStdout) {//just -o option
		FILE *inputFile = fopen(argv[1], "r");
		if(inputFile == NULL) {printf("Error opening %s\n", argv[1]); return 1;}
		
		long len = getEOF(inputFile);
		char inputBuff[len], outputBuff[len];
		fread(inputBuff, sizeof(char), len, inputFile);
		stripCSS(inputBuff, outputBuff);
		fclose(inputFile);
		
		fwrite(outputBuff, sizeof(char), strlen(outputBuff), stdout);
		
		return EX_OK;
	}
	
	char *inputFile = strdup(argv[1]); //duplicate the inputfile path
	filePathString outputFile, newFile;

	if(!gOptions.sameFile && argc == 2) {//then normal conversion without a names ouputfile, just add _stripped to the end
		newFile = initPathStructWithPath(argv[1]);
		strcat(newFile.fileName, "_stripped");
		reInitOriginalString(&newFile);
	} else if(!gOptions.sameFile) {//then normal with user defined output file
		newFile = initPathStructWithPath(argv[2]);
	}

	if(!gOptions.recurseDir && !gOptions.sameFile) {//normal conversion
		processCSSFile(inputFile, newFile.originalString);
	} else if(gOptions.sameFile && !gOptions.recurseDir) {//if the user does not want resursive dir and does want same file
		filePathString sameInputFile = initPathStructWithPath(inputFile);
		processCSSFileUsingOriginalFileName(sameInputFile);
	} else if(gOptions.recurseDir) {//recursive directory
		recursiveProcessOfSiteDir(inputFile);
	}
	
	free(inputFile);
	
	return EX_OK;
}