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
#import "longtoshort.h"
#import "dataStructures.h"
#import "constants.h"

//long hand to shorthand processer
char *longToShort(const char *input, char *output) {
	longHandPropertySet background = {
		.props = {//background propertys
			"background-color",
			"background-image",
			"background-repeat",
			"background-attachment",
			"background-position"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false,
			false,
			false,
		},
		.propLength = 5,
		.finalString = "background:",
		.isBeingUsed = false
	};
	
	longHandPropertySet font = {
		.props = {//all the long hand font properties
			"font-style",
			"font-variant",
			"font-weight",
			"font-size",
			"font-family"
		},
		.propString = {
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {
			false,
			false,
			false,
			true,
			true
		},
		.propLength = 5 ,
		.finalString = "font:",
		.isBeingUsed = false
	};
	
	longHandPropertySet margin = {
		.props = {//background propertys
			"margin-top",
			"margin-right",
			"margin-bottom",
			"margin-left"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			true,
			true,
			true,
			true
		},
		.propLength = 4,
		.finalString = "margin:",
		.isBeingUsed = false
	};
	
	longHandPropertySet padding = {
		.props = {//background propertys
			"padding-top",
			"padding-right",
			"padding-bottom",
			"padding-left"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			true,
			true,
			true,
			true
		},
		.propLength = 4,
		.finalString = "padding:",
		.isBeingUsed = false
	};
	
	longHandPropertySet listStyle = {
		.props = {//background propertys
			"list-style-type",
			"list-style-image",
			"list-style-position"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false
		},
		.propLength = 3,
		.finalString = "list-style:",
		.isBeingUsed = false
	};
	
	longHandPropertySet outline = {
		.props = {//background propertys
			"outline-color",
			"outline-style",
			"outline-width"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false,
		},
		.propLength = 3,
		.finalString = "outline:",
		.isBeingUsed = false
	};
	
	longHandPropertySet borderBottom = {
		.props = {//background propertys
			"border-bottom-width",
			"border-bottom-style",
			"border-bottom-color"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false
		},
		.propLength = 3,
		.finalString = "border-bottom:",
		.isBeingUsed = false
	};
	
	longHandPropertySet borderLeft = {
		.props = {//background propertys
			"border-left-width",
			"border-left-style",
			"border-left-color"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false
		},
		.propLength = 3,
		.finalString = "border-left:",
		.isBeingUsed = false
	};
	
	longHandPropertySet borderRight = {
		.props = {//background propertys
			"border-right-width",
			"border-right-style",
			"border-right-color"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false
		},
		.propLength = 3,
		.finalString = "border-right:",
		.isBeingUsed = false
	};
	
	longHandPropertySet borderTop = {
		.props = {//background propertys
			"border-top-width",
			"border-top-style",
			"border-top-color"
		},
		.propString = {//initalize all the strings to null
		{'\0'},
		{'\0'},
		{'\0'}},
		.required = {//whether or not the string should have a 'inherit'
			false,
			false,
			false
		},
		.propLength = 3,
		.finalString = "border-top:",
		.isBeingUsed = false
	};
	
	
	longHandPropertySet *allProps[] = {//store references to all the property buffers in here
		&background,
		&font,
		&margin,
		&padding,
		&listStyle,
		&outline,
		&borderBottom,
		&borderLeft,
		&borderRight,
		&borderTop
	};
	
	int a, b, c;
	char next = '\0', *declerationBuff, *declerationBuffPtr;
	BOOL shouldSkip = false, isInDecleration = false;
	
	do {
		shouldSkip = false;
		next = *(input + 1); //get the next char
		for(a = 0; a<COUNT(allProps); a++) {
			for(b = 0; b<allProps[a]->propLength; b++) {
				if(*input == allProps[a]->props[b][0] && css_strcmp(input, allProps[a]->props[b]) == 0 
				   && allProps[a]->propString[b][0] == '\0') {//if we have found a property and the property isn't being used
					allProps[a]->isBeingUsed = true;
					
					flushString(allProps[a]->propString[b], PROPBUFFSIZE); //flush the property string
					char *propStringPointer = allProps[a]->propString[b];
					
					//then read the property into the buffer
					while(*input++ != ':') {//get up the content of the decleration
						
#if DEBUG >= 2
						if(*input == '}' || *input == '{') {
							printf("[longToShort()] Braces found where they shouldn't be\n");
						}
#endif
					}

					do {//then add it to the buffer
						*propStringPointer++ = *input;
					} while(*(++input) != ';' && (*input != '}' || !strncmp(input-3, ESCAPED_IE_HACK, ESCAPED_IE_HACK_LEN))); //we dont want the ending ;
					*propStringPointer = '\0'; //add a null char at the end
					//printf("Final prop: %s\n",  allProps[a]->propString[b]);
#if DEBUG >= 2
					if(*input == '}' || *input == '{')
						printf("[longtoshort()] Braces found while getting CSS property\n");
#endif
					next = *(input + 1);
					shouldSkip = true; //dont skip here cause we need to finish the loop
				}
			}
		}
		
		/*
		 check to see if we have reached the end of a decleration.
		 Also check to see if the ending } that we have found is actually part of an IE hack, outlined here: http://diveintoaccessibility.org/examples/fontsize.html
		 */

		if(*input == '}' && strncmp(input-3, ESCAPED_IE_HACK, ESCAPED_IE_HACK_LEN) != 0) {
			isInDecleration = false;
			
			for(a = 0; a<COUNT(allProps); a++) {//output all the property buffers into the CSS decleration
				output += dumpLongHandSet(allProps[a], output, &shouldSkip); //add the amount of chars added to the pointer
			}
			
			*declerationBuffPtr = '\0';
			declerationBuffPtr = &declerationBuff[0];

			while(*declerationBuffPtr && (*output++ = *declerationBuffPtr++)) ;
			free(declerationBuff);
			shouldSkip = false;
		} else if(*input == '{') {
			*output++ = *input; //ouput the { first so it doesn't get put in the buffer
			isInDecleration = true;
			declerationBuff = (char *) malloc(strlen(input)+1);
			declerationBuffPtr = declerationBuff;
			shouldSkip = true; //skip since we already outputted the { and we dont want it in the buffer
		}
		
		if(shouldSkip) {//dont write the char to the ouput buffer if we dont need to
			continue; 
		}

		if(!isInDecleration) {
			*output++ = *input;
		} else {
			*declerationBuffPtr++ = *input;
		}
		
	} while(*input++);

	return &output[0];
}

int dumpLongHandSet(longHandPropertySet *input, char *output, BOOL *shouldSkip) {
	int pointerAdvance = 0;
	if(input->isBeingUsed) {
		int a;
		for(a = 0; a<input->propLength; a++) {
			if(input->required[a] && input->propString[a][0] == '\0') {
				pointerAdvance += outputLongHandFromLongHandSet(output, input);
				clearLongHandPropertySet(input);
				return pointerAdvance;
			}
		}

		char *prop = input->finalString;
		while('\0' != *prop && (*output++ = *prop++)) 
			pointerAdvance++; //copy the shorthand name, but not the null char
		
		for(a = 0; a<input->propLength; a++) {
			if(strlen(input->propString[a]) > 0) {
				if(*(output - 1) != ':') {//add a space as long as we are not in the beggining
					*output++ = ' ';
					pointerAdvance++;
				}
				char *propPtr = input->propString[a];
				do {
					*output++ = *propPtr;
					pointerAdvance++;
				} while(*(++propPtr));
			}
		}
				
		*output++ = ';';
		pointerAdvance++;
		*shouldSkip = true;
	}
	
	clearLongHandPropertySet(input);
	return pointerAdvance;
}

int outputLongHandFromLongHandSet(char *output, longHandPropertySet *set) {
	int a = 0, pointerAdvance = 0;
	
	for(; a<set->propLength; a++) {
		if(set->propString[a][0] != '\0') {
#if DEBUG >= 1
			printf("[outputLongHandFromLongHandSet()] Found long-hand property \"%s\" With value \"%s\" but not all required short-hands were found\n", set->props[a], set->propString[a]);
#endif
			strcat(output, set->props[a]);
			strcat(output, ":");
			strcat(output, set->propString[a]);
			strcat(output, ";");
			pointerAdvance += strlen(set->props[a]) + strlen(set->propString[a]) + 2;
		}
	}
	
	return pointerAdvance;
}

void clearLongHandPropertySet(longHandPropertySet *set) {
	set->isBeingUsed = false;
	
	int a = 0;
	for(; a<set->propLength; a++) {
		flushString(set->propString[a], PROPBUFFSIZE);
	}
}
