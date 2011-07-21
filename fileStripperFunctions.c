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
#import <unistd.h>
#import <ctype.h>
#import <sysexits.h>
//R_OK & stuff are in <sys/unistd.h>

#import "dataStructures.h"
#import "constants.h"
#import "stringFunctions.h"
#import "fileSystemFunctions.h"
#import "cssStripFunction.h"
#import "argumentHandlerFunctions.h"
#import "fileStripperFunctions.h"
#import "recursiveFunctions.h"

int processCSSFile(char *input, char *output) {
	extern commandLineArguments gOptions;
	extern fileLink *gRoot;
	
	FILE *targetFile = fopen(input, "r");
	FILE *newFileStream = fopen(output, "w");
	
	//check for file IO errors
	if(targetFile == NULL || newFileStream == NULL) {
		if(targetFile == NULL && newFileStream == NULL) {
			printf("Error reading: %s\nAnd error writing: %s\n", output, input);
			exit(EX_IOERR);
		} else if(targetFile == NULL) {
			printf("Error reading: %s\n", input);
			exit(EX_IOERR);
		} else if(newFileStream == NULL) {
			printf("Error writing: %s\n", output);
			exit(EX_IOERR);
		}
	}

	long originalFileLength = 0, strippedFileLength = 0; //counters for file length
	originalFileLength = getEOF(targetFile); //counts the total file length
	char begin[originalFileLength], end[originalFileLength], *cpyPtr; //define to char buffers to hold the unstripped and stripped chars from the file
	cpyPtr = begin; //puts the generic char pointer at the beggining
	
								//make sure the gRoot is being used before we test it
	if(gOptions.html == true && ((gRoot != NULL && gRoot->type == HTMLFILETYPE) || !gOptions.recurseDir)) {
		char currChar, prevChar = '\0', *buffPtr, *tempBuff = (char*) malloc(originalFileLength+1);
		BOOL shouldSkip = false;
		buffPtr = tempBuff;
		
		while((*buffPtr++ = fgetc(targetFile)) != EOF) ;
		*buffPtr = '\0';
		buffPtr = tempBuff;
		
		while(*buffPtr++ != '\0') {
			shouldSkip = false;
			if(*buffPtr == '<' && *(buffPtr+1) != '/') {//found a opening html element
				*cpyPtr++ = *buffPtr++; //copy the <
				
				if(css_strcmp(buffPtr, "style") == 0) {//and the html element is a style tag
					*cpyPtr++ = *buffPtr; //copy the 's'
					while(*(++buffPtr) != '>') {//read till the ending >
						*cpyPtr++ = *buffPtr;
					}
					
					*cpyPtr++ = *buffPtr; //copy the >
					char *cssBuff = (char*)malloc(strlen(buffPtr)+1), *cssPtr; //create a buffer for the CSS information
					cssPtr = cssBuff; //set the ptr to the beggining of the buffer
					
					while(*(++buffPtr) != '<' && css_strcmp(buffPtr+1, "/style>") != 0) {//copy all the css information
						*cssPtr++ = *buffPtr;
					}
					
					*cssPtr = '\0'; //set the terminating null
					char *tempCssBuff = (char*) malloc(strlen(cssBuff)+1); //create a temporary buffer to hold the processed css information
					stripCSS(cssBuff, tempCssBuff);
					strcat(cpyPtr, cssBuff); //add the processed CSS information to the final buffer
					cpyPtr += strlen(cssBuff);
					
					//copy the </style>
					*cpyPtr++ = *buffPtr;
					strncpy(cpyPtr, buffPtr+1, 7);
					cpyPtr += 7;
					buffPtr += 7;
					
					shouldSkip = true; //we dont want to copy the > again
					
					free(tempCssBuff);
					free(cssBuff);
				}
			}
			
			if(!shouldSkip)
				*cpyPtr++ = *buffPtr;
		}
		
		strcpy(end, begin);
		free(tempBuff);
	} else {//simple copy the file into a buffer
		while((*cpyPtr++ = fgetc(targetFile)) != EOF) ;
		*(--cpyPtr) = '\0';
		stripCSS(begin, end);
	}
	
	if(strlen(begin) == 0) {//odd bug I found, check for zero length files
		printf("File Length of %s is zero!\n", input);
		//close the file streams just in case
		fclose(newFileStream);
		fclose(targetFile);
		return 0;
	}
		
	strippedFileLength = strlen(end);
	fwrite(end, sizeof(char), strippedFileLength, newFileStream);
	fclose(newFileStream);
	fclose(targetFile);
	
	if(strippedFileLength != originalFileLength) {
		if(gOptions.quiet) return 1;
		
		printf("---------------------------------------------------------\n");
		printf("CSS file successfully stripped of excess characters.\n");
		printf("Original file length: %u\n", originalFileLength);
		printf("Stripped file length: %u\n", strippedFileLength);
		printf("# of stripped characters: %u\n", originalFileLength-strippedFileLength);
		printf("Percent of file-size saved: %%%i\n", roundFloat(((originalFileLength-(float)strippedFileLength)/originalFileLength)*100));
		printf("Path of target CSS file: %s\n", gOptions.noBackup?output:input);
		printf("Path of stripped CSS file: %s\n", output);
		printf("---------------------------------------------------------\n\n");
	} else if(!gOptions.sameFile) {//if we are not using
		remove(output);
#if DEBUG >= 1
		printf("[processCSSFile()] No file-size saved, removing _stripped file %s\n", output);
#endif
	}

	return 1;
}

int processCSSFileUsingOriginalFileName(filePathString outputFile) {
	extern commandLineArguments gOptions;
	
	char *input = strdup(outputFile.originalString);
	strcat(outputFile.fileName, "_unstripped");
	reInitOriginalString(&outputFile);
	
	copy(input, outputFile.originalString);
	processCSSFile(outputFile.originalString, input);
	
	if(gOptions.noBackup) {//if we dont want a backup then delete the file
		remove(outputFile.originalString);
	}
	
	free(input);
	return 1;
}

void fileCanBeProcessed(char *path) {
	extern fileLink *gRoot;
	filePathString temp = initPathStructWithPath(path);
	BOOL htmlFile = false;
	
#if DEBUG >= 2
	printf("[fileCanBeProcessed()] Looking at file: %s\n", path);
#endif

	if(strcmp(temp.extension, ".css") == 0 || 
	  (htmlFile = (strcmp(temp.extension, ".html") == 0 || strcmp(temp.extension, ".html") == 0))) {//if we have css extension
		gRoot->fileName = strdup(path);
		gRoot->type = (htmlFile)?HTMLFILETYPE:CSSFILETYPE;
		gRoot = gRoot->nextFile = (fileLink *) malloc(sizeof(fileLink));
		
		//set to NULL so we can compare to null
		gRoot->fileName = NULL;
		gRoot->nextFile = NULL;
		
#if DEBUG >= 1
		printf("[fileCanBeProcessed()] CSS File Found: %s\n", path);
#endif
	}
	return;
}

int recursiveProcessOfSiteDir(char *startDir) {
	extern commandLineArguments gOptions;
	extern fileLink *gRoot;
	gRoot = (fileLink *) malloc(sizeof(fileLink)); //make the root node
	fileLink *tempLink;
	tempLink = gRoot;
	
	listDir(strdup(startDir), fileCanBeProcessed);
	
	gRoot = tempLink;

	while(gRoot->nextFile != NULL) {//loop though the linear list and retrieve the names
		if(strlen(gRoot->fileName) > 0) {
			if(gOptions.sameFile) {//then we want same file
#if DEBUG >= 1
				printf("[recursiveProcessOfSiteDir()] Processing using original file name: %s\n", gRoot->fileName);
#endif
				processCSSFileUsingOriginalFileName(initPathStructWithPath(gRoot->fileName));
			} else {//then we want a _stripped file
				filePathString tempFile = initPathStructWithPath(gRoot->fileName);
				strcat(tempFile.fileName, "_stripped");
				reInitOriginalString(&tempFile);
				processCSSFile(gRoot->fileName, tempFile.originalString);
			}
		}
			
		tempLink = gRoot;
		gRoot = gRoot->nextFile;
		free(tempLink->fileName);
		free(tempLink);
	}
	
	return 1;
}

char *processCSSFromInput(void) {
	char inputBuff[INPUT_BUFF_SIZE+1], *inputFile = (char*)malloc(INPUT_BUFF_SIZE), curr, *outputFile;
	int boxPos = 0, buffSize = INPUT_BUFF_SIZE;
	
	while((curr = getchar()) != EOF) {
		inputBuff[boxPos++] = curr;
		if(boxPos == INPUT_BUFF_SIZE) {
			inputBuff[boxPos] = '\0';
			strncat(inputFile, inputBuff, INPUT_BUFF_SIZE);
			
			inputFile = realloc(inputFile, buffSize += INPUT_BUFF_SIZE);
			bzero(inputBuff, INPUT_BUFF_SIZE);
			boxPos = 0;
		}
	}
	
	strcat(inputFile, inputBuff);
	
	if(strlen(inputFile) == 0) {
		free(inputFile);
		ARG_ERR("-i option specified with no data in stdin");
	}
		
	outputFile = (char*) malloc(strlen(inputFile));
	stripCSS(inputFile, outputFile);
		
	free(inputFile);

	return outputFile;
}

//simple round() function
//use <math.h>'s lround() instead
int roundFloat(float num) {
	int intValue = num;
	float decimalValue = num-(float)intValue;
	if(decimalValue>=.5)
		return intValue+1;
	else 
		return intValue;
}
