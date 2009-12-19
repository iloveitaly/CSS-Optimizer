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

#import "fileSystemFunctions.h"
#import "recursiveFunctions.h"
#import "fileStripperFunctions.h"
#import "constants.h"

#import <stdio.h>
#import <dirent.h>
#import <sys/stat.h>
#import <stdlib.h>
#import <string.h>
#import <unistd.h>

int listDir(char *dirName, void (*funct)(char *)) {
	if(access(dirName, R_OK | F_OK) == -1) {
		printf("[listDir()]Not enough permissions to read directory:%s\n", dirName);
		return -1;
	}
	
	DIR *dir;
	if((dir = opendir(dirName)) == NULL) {
		printf("[listDir()]Error Cannot read dir:%s\n", dirName);
		return 0;
	}
		
	struct dirent *currFile;
	
	while((currFile = readdir(dir)) != NULL) {
		if(strcmp(currFile->d_name, ".") == 0 || strcmp(currFile->d_name, "..") == 0)
			continue;
		
#if DEBUG >= 3
		printf("[listDir()]Reading file name: %s From dir: %s\n", currFile->d_name, dirName);
#endif
		char *tempString = (char *) malloc(strlen(currFile->d_name)+strlen(dirName)+2);
				
		strcpy(tempString, dirName);
		if(tempString[strlen(tempString)-1] != '/')
			strcat(tempString, "/"); //add a slash if the directory path does not contain one
		strcat(tempString, currFile->d_name);
		
#if DEBUG >= 2
		printf("[listDir()]File: %s isDir:%i\n", tempString, isDir(tempString));
#endif
		
		if(isDir(tempString) == 1) {
			listDir(strdup(tempString), funct);
#if DEBUG >= 2
			printf("[listDir()]Now searching %s Dir\n", tempString);
#endif
		}
		
		funct(tempString);
		free(tempString);
	}
	
	free(dirName);
	closedir(dir);
	return 0;
}

int isLink(const char *name) {
	struct stat file;
	stat(name, &file);
	
	if(S_ISLNK(file.st_mode))
		return 1;
	else
		return 0;
}
