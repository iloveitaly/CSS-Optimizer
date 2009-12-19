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
#import <string.h>

#import "dataStructures.h"
#import "constants.h"
#import "cssStripFunction.h"

/*
 Wrapper processing function, sends input CSS through all or some CSS processing routines depending on what arguments were specified.
 */
int stripCSS(char input[], char output[]) {
	extern commandLineArguments gOptions;
	
	long inputLen = strlen(input);
	if(gOptions.longToShortOnly) {
		longToShort(input, output);
	} else {
		stripCharsFromCSS(input, output);
		flushString(input, inputLen); //flush with nulls then send it into the longHandProccesser
		
		if(gOptions.longToShort) {//if we should do long to short conversion
			longToShort(output, input);
			flushString(output, inputLen);
		} else {//if not just cpy the file
			strcpy(input, output);
			flushString(output, inputLen);
		}
		
		endOfDeclarationOptimization(input, output);
	}
	
	return 0;
}

/*
 main css optimizer routine. This is called first before any other optimization routines
 strips comments, whitespace, tabs, and newlines
 */
int stripCharsFromCSS(char *input, char *output) {
	char prev = '\0', next = '\0';
	BOOL isMultilinedComment = false, isUselessChar = false, isInDecleration = false, isInPropertyData = false; 
	int singleQuoteCount = 0, doubleQuoteCount = 0, parenthCount = 0;

	do {
		isUselessChar = false;		
		next = *(input + 1); //grab the next char
		switch(*input) {
			case'\t':case '\r': //checks for stuff we dont want
				isUselessChar = true;
				break;
			case '*': //checks for multiline starting and stopping
				if(prev == '/') {
					isMultilinedComment = true;
				}
				break;
			case '/': //also checks for multiline ending and starting chars
				if(!css_strcmp(input, COMMENT_HACK)) {
#if DEBUG >= 1
					printf("[cssStripFunction] Found comment hack.\n");
#endif
				}
				
				if(prev == '*') {
					isMultilinedComment = false;
				}
				
				if(prev != '*' && next != '*' && isInPropertyData) {
					// then we could be in a font: 13%/13.em decleration... not sure if this check if foolproof
					break;
				}
				
				// although a slash isn't a useless char we assign the BOOL as true just for the convience
				// of not making another BOOL var to check at the end
				isUselessChar = true; 
									
				break;
			case ' ':
				//check for all places in which white space is not needed
				if(prev == ':' || prev == ' ' || prev == '}' || prev == '{' || prev == ',' || 
				   next == ';' || next == '{' || next == '}' || prev == ';' || (!isInPropertyData && isInDecleration)) 
					isUselessChar = true;
				break;
			case '\n':
				isUselessChar = true;
				break;
			case '\'':
				if(isMultilinedComment) break; //if were are in a comment then we dont worry about the quotes
				//if the count is 0, increment, if its not zero decrement
				if(singleQuoteCount == 0) singleQuoteCount++;
				else if(singleQuoteCount == 1) singleQuoteCount--;
			case '\"':
				if(isMultilinedComment) break;
				
				if(!css_strcmp(input, ESCAPED_IE_HACK)) {
#if DEBUG >= 1
					printf("[stripCharsFromCSS()] Hack found\n");
#endif
					output = stpcpy(output, ESCAPED_IE_HACK);
					input += strlen(ESCAPED_IE_HACK)-1; //dont go up the next char since the while() at the end will do it
					prev = '\"'; //set prev to the end of the IE hack
					continue;
				}
				
				if(doubleQuoteCount == 0) doubleQuoteCount++;
				else if(doubleQuoteCount == 1) doubleQuoteCount--;
				break;
			case '{': //check for a style decleration
				if(!isMultilinedComment && !doubleQuoteCount && !singleQuoteCount) //if we are not in quotes or a comment
					isInDecleration++;
				break;
			case '}': //check for an ending style decleration
				if(!isMultilinedComment && !doubleQuoteCount && !singleQuoteCount) { //if we are not in quotes or a comment
					isInDecleration--;
					isInPropertyData = false;
				}
				break;
			case ':': //check for the beggining of a property decleration
				if(isInDecleration && !isMultilinedComment && !doubleQuoteCount && !singleQuoteCount) //if we are in a decleration and not in a comment or string
					isInPropertyData++;
				break;
			case ';':
				if(isInDecleration && !isMultilinedComment && !doubleQuoteCount && !singleQuoteCount)
					isInPropertyData--;
				break;
			case '(':
				if(isMultilinedComment || doubleQuoteCount || singleQuoteCount) break; //make sure we are not surrounded by quotes or comments
				parenthCount++;
				break;
			case ')':
				if(isMultilinedComment || doubleQuoteCount || singleQuoteCount) break; //make sure we are not surrounded by quotes or comments
				parenthCount--;
				break;
		} 
		
		//if the char is not a useless char AND we are not in a multi line comment
		//OR if we are in quotes
		//OR if we are in 'parentese quotes'
		//then output the char
		if((!isUselessChar && !isMultilinedComment) || doubleQuoteCount || singleQuoteCount || parenthCount) {//add char to ouput string if everything is cool					
			*output++ = *input; //add the char to the output
		}

		prev = *input;
	} while(*input++);
	
	return 0;
}

//function to remove a semi-colon from the end of a CSS decleration
char *endOfDeclarationOptimization(char *input, char *output) {
	char next;
	do {
		next = *(input+1);
		if(*input == ';' && next == '}') {
			continue;
		}
		*output++ = *input;
	 } while(*input++);
	
	return &output[0];
}
