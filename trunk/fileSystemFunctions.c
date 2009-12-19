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
#import <sys/stat.h>
#import <sys/types.h>
#import <libgen.h>
#import <unistd.h>

#import "dataStructures.h"
#import "constants.h"
#import "stringFunctions.h"
#import "fileSystemFunctions.h"

int copy(const char *from, const char *to) {
	FILE *fromFile = fopen(from, "r"), *toFile = fopen(to, "w");
	
	if(fromFile == NULL || toFile == NULL) {
		printf("[copy()] Error opening %s or %s\n", from, to);
		return -1;
	}
	
	long len = getEOF(fromFile);
	void *buff = malloc(len);
	
	fread(buff, 1, len, fromFile);
	fwrite(buff, 1, len, toFile);
	
	free(buff);
	fclose(fromFile);
	fclose(fromFile);
	
	/*FILE *fromFile, *toFile;
	fromFile = fopen(from, "r");
	toFile = fopen(to, "w");
	
	char cpyChar;
	while((cpyChar = fgetc(fromFile)) != EOF)
		fputc(cpyChar, toFile);
	
	fclose(fromFile);
	fclose(toFile);*/
	return 0;
}

//gets the file path
//Deprecated & buggy, using dirname() instead
char *getFilePath(const char filePath[], char to[]) {
	char *strippedDir;
	if((strippedDir = strrchr(filePath, '/')) == NULL) return ""; //return empty string since the path must of been a root path
	unsigned int len = strlen(filePath)-strlen(strippedDir);
	subString(filePath, to, 0, len);
	return to;
}

//gets the filename from a directorie and copies it to to[]
//Deprecated & buggy, using basename() instead
char *getFileName(const char filePath[], char to[], BOOL withFileExtension) {
	char *fileName;
	if((fileName = strrchr(filePath, '/')) == NULL) 
		return strcpy(to, filePath); //return original file path cause there is no directories in path
	if(withFileExtension) {
		return strcpy(to, fileName+1); //instead of refencing fileName[0] which would include the / we refernce fileName[1]
	} else {
		//subtrace one for the end because it is referencing the .
		int end = lastIndexOf(fileName, ".");
		if(end == -1) end = strlen(fileName)-1;
		else end--; //stripp off the '.' that the index is reffering to
		return subString(fileName, to, 1, end); //start on 1 because the 0 char is the last /
	}
}

//function that grabs the files extension and copies it to the to[] string
//returns null if no extension is found
char *getFileExtension(const char target[], char to[]) {
	int extensionIndex = lastIndexOf(target, ".");
	if(extensionIndex != -1) 
		return subString(target, to, extensionIndex, strlen(target));
	else
		return NULL;
}

//gets the file length, rewinds the internal file pointer once its done
long getEOF(FILE *file) {
	long counter = 0;
	
	/*
	rewind(file);
	while(fgetc(file) != EOF) counter++;
	rewind(file);
	 */
	
	fseek(file, 0, SEEK_END);
	//printf("%i:%i\n", counter, ftell(file)); /* test to make sure the new method was just as good as the old method */
	counter = ftell(file);
	rewind(file);
	return counter;
}

//-------------------------------
//	BOOL/Checker functions
//-------------------------------

//function that checks if the file exists, returns 1 if the file was found 0 if the file was not found
int fileExists(const char name[]) {//function that checks if the file exists
	if(!access(name, F_OK)) {
		return true;
	} else {
		return false;
	}
	
	/*
	 struct stat file;
	if(stat(name, &file) == -1) {
		printf("Error/Access denied for file:%s:\n", name);
		return false;
	}
	
	if(S_ISREG(file.st_mode))
		return 1;
	else
		return 0;
	 */
}

//checks for a file extension
int hasExtension(const char *path) {
	char *last_slash;
	if((last_slash = strrchr(path, '/')) != NULL) {
		if(strchr(path, '.') == NULL)
			return false;
		else
			return true;
	} else {
		return false;
	}
}

//checks if a string contains a filename
int hasFileName(const char *path) {
	char *last_slash;
	if((last_slash = strrchr(path, '/')) != NULL) {
		last_slash++;
		if(strlen(last_slash) > 0)
			return true;
		else 
			return false;
	} else if(strlen(path) > 0)
		return true;
	else 
		return false;
}

int isDir(const char *name) {
	if(!fileExists(name)) return false;
	
	struct stat file;
	if(stat(name, &file) == -1) {
		printf("[isDir()] Error reading file:%s\n", name);
		return false;
	}
	
	if(S_ISDIR(file.st_mode)) {
		return true;	
	} else {
		return false;
	}
	
	/*
	struct stat file;
	
	if(stat(name, &file) == -1) {
		printf("[isDir()]Error reading file:%s\n", name);
		return false;
	}
	
	if(S_ISDIR(file.st_mode))
		return true;
	else 
		return false;
	*/
}

//------------------------------
//	File path functions
//------------------------------

//creates a path struct from a path
filePathString initPathStructWithPath(const char *path) {
	filePathString newPath = {0, 0, 0, '\0', '\0', '\0', '\0'};
	strcpy(newPath.originalString, path);
	
	initBOOLSForFilePath(&newPath);
	
	return newPath;
}

//inits the BOOLs for the file path struct
filePathString *initBOOLSForFilePath(filePathString *filePath) {
	filePath->hasFileName = hasFileName(filePath->originalString);
	filePath->hasFileExtension = hasExtension(filePath->originalString);
	
	if(!filePath->hasFileExtension && !filePath->hasFileName && isDir(filePath->originalString)) {
		filePath->isDir = true;
		return filePath;
	}
	
	//check for file extensions and assign if nessicary
	if(filePath->hasFileExtension) {
		getFileExtension(filePath->originalString, filePath->extension);
	} else {
		filePath->extension[0] = '\0';
	}
	//check for the file name and assign the property if nessicary
	if(filePath->hasFileName) {
		char *tempName = strdup(basename(filePath->originalString)); int i; //declare i so i dont have to call lastIndexOf() twice
		strncpy(filePath->fileName, tempName, (i = lastIndexOf(tempName, ".")) != -1?i:strlen(tempName));
		free(tempName);
	} else {
		filePath->fileName[0] = '\0';
	}
	
	//assign the file path
	strcpy(filePath->filePath, dirname(filePath->originalString));
	strcat(filePath->filePath, "/"); //dirname() doesn't keep the /
	
	return filePath;
}

//re-contructs the file path string from the different path components in the struct
filePathString *reInitOriginalString(filePathString *filePath) {
	char newOriginalString[MAXFILEPATH] = {'\0'};
	strcat(newOriginalString, filePath->filePath);	
	strcat(newOriginalString, filePath->fileName);
	strcat(newOriginalString, filePath->extension);
	flushString(filePath->originalString, MAXFILEPATH);
	strcpy(filePath->originalString, newOriginalString);
	return filePath;
}
