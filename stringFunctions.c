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

#import <string.h>
#import <stdio.h>
#import "constants.h"
#import "dataStructures.h"

char *subString(const char target[], char to[], int begin, int end) {
	int a = 0;
	
	for(; begin<=end; begin++, a++) {
		to[a] = target[begin];
		if(target[begin] == '\0') break;
	}
	
	if(to[a-1] != '\0') //add a termniting null char if there wasn't one
		to[a] = '\0';
	
	return to;
}

int indexOf(const char target[], const char searchString[]) {
	if(strlen(searchString) == 0 || strlen(target) == 0) {
		printf("Your indexOf() params must be longer that 0");
		return -1;
	}
	
	long maxLength = strlen(target), searchLength = strlen(searchString), a = 0, b = 0, c = 0;
	BOOL found = false;
	
	for(; a<maxLength && !found; a++) {
		if(target[a] == searchString[0]) {
			for(b = a, c = 0; b<maxLength && target[b] == searchString[c]; b++, c++) {
				if(c+1 == searchLength) {
					found = true;
					break;
				}
			}
		}
	}
	
	if(found)
		return --a;
	else 
		return -1;
}

int lastIndexOf(const char target[], const char searchString[]) {
	if(strlen(searchString) == 0 || strlen(target) == 0) {
		printf("Your lastIndexOf() params must be longer that 0\n");
		return -1;
	}
	
	long maxLength = strlen(target), searchLength = strlen(searchString), a = maxLength-1, b = 0, c = 0;
	BOOL found = false;
	//not fully bug checked, if something goes wrong look here
	for(; a != 0 && !found; a--) {
		if(target[a] == searchString[0]) {
			for(b = a, c = 0; b<maxLength && target[b] == searchString[c]; b++, c++) {
				if(c+1 == searchLength) {
					found = true;
					break;
				}
			}
		}
	}
	
	if(found) {
		return ++a;
	} else { 
		return -1;
	}
}

//flushes out a string with nulls
//replaced my implementation with the built in bzero() call
char *flushString(char *string, int len) {
	bzero(string, len);
	/*
	for(; len != 0; len--, string++)
		*string = '\0';
	 */
	return &string[0];
}

char *flushStringWithString(char *string, int len, char *cpyStr) {
	char *startString = flushString(string, len);
	while(*string++ = *cpyStr++) ;
	return startString;
}

//compares the string for as long as the shortest string is
int css_strcmp(const char *str1, const char *str2) {
	int compareLimit = (strlen(str1)>strlen(str2))?strlen(str2):strlen(str1), a = 0;
	for(; a<compareLimit; a++) {
		if(str1[a] != str2[a])
			return true;
	}
	
	return false;
}

void swapStrings(char **ptr1, char **ptr2) {
    char *temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}
