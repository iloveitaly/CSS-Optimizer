/*
 Application: CSS Optimizer
 Copyright (C) 2005  Michael Bianco
 
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

#import "dataStructures.h"

#import <stdlib.h>
#import <stdio.h>
#import <string.h>
#import <sysexits.h>

void printUsage(void) {
	printf(
		"CSS Optimizer 1.2. http://mabblog.com/cssoptimizer/\n\n"
		"Usage: cssoptimizer [ -hlnsqio ] inputfile [ outputfile ]\n"
		"Usage: cssoptimizer -r [ -hlnsq ] rootdir\n\n"
		"More information can be found in the 'readme.html'\n"
	   );
}

//I didn't know about getopt() when I wrote this :P
void checkForAlternateCommands(const char *argv[], int argc) {
	if(argc == 1 || !strcmp(argv[1], "help") || !strcmp(argv[1], "--help")) {//checks to make sure there is at least a filename specified
		printUsage();
		exit(EX_OK);
	} else if(!strcmp(argv[1], "warrantee") || !strcmp(argv[1], "--warrantee") || !strcmp(argv[1], "license") || !strcmp(argv[1], "--license"))  {//display warantee
		printf("\n-----------------------------------------------------------------\n");
		printf(
			   "CSS Optimizer is a command line utility that strips all un-needed characters from a CSS file. V1.0\n"
			   "Copyright (C) 2005 Michael Bianco, MAB Web Design <http://software.mabwebdesign.com/>\n\n"
			   "This program is free software; you can redistribute it and/or\n"
			   "modify it under the terms of the GNU General Public License\n"
			   "as published by the Free Software Foundation; either version 2\n"
			   "of the License, or (at your option) any later version.\n\n"
			   "This program is distributed in the hope that it will be useful,\n"
			   "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
			   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
			   "GNU General Public License for more details.\n\n"
			   "You should have received a copy of the GNU General Public License\n"
			   "along with this program; if not, write to the Free Software\n"
			   "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n"
			   );
		printf("-----------------------------------------------------------------\n\n");
		exit(EX_OK);
	} else if(!strcmp(argv[1], "version") || !strcmp(argv[1], "--version")) {
		printf("CSS Optimizer V1.0. Copywrite 2005 Michael Bianco <software@mabwebdesign.com>, <http://developer.mabwebdesign.com>\n");
		exit(EX_OK);
	}
}

commandLineArguments checkForOptions(const char *argv[], int *argc) {	
	commandLineArguments arguments;
	memset(&arguments, 0, sizeof(arguments)); //initalize all elements of arguments to 0
	
	int a = *argc-1, b, c, //counter vars
		pathIndexes[] = {-1, -1}; //indexes of the paths in argv, should at max be two paths (input, output)
	for(; a > 0; a--) {//loop through all the command line arguments. > 1 because we dont want [0] which is the path of the binary
		if(argv[a][0] == '-') {//if we found an option
			for(b = 1; b<strlen(argv[a]); b++) {
				switch(argv[a][b]) {//determine what argument it is
					case 'R': //add support for standard -R argument
					case 'r': //then they want recursive
#if DEBUG >= 1
						printf("[checkForOptions()] Recursive dir == true\n");
#endif
						arguments.recurseDir = true;
						break;
					case 's': //then they want the same file to be processed
#if DEBUG >= 1
						printf("[checkForOptions()] Same file == true\n");
#endif
						arguments.sameFile = true;
						break;
					case 'n':
#if DEBUG >= 1
						printf("[checkForOptions()] No Backup == true\n");
#endif							
						arguments.noBackup = true;
						break;
					case 'l': //then they want no long to short conversion
#if DEBUG >= 1
						printf("[checkForOptions()] No long to short == true\n");
#endif														
						arguments.longToShort = false;
						break;
					case 'L':
#if DEBUG >= 1
						printf("[checkForOptions()] Long to short only == true\n");
#endif																				
						arguments.longToShortOnly = true;
						break;
					case 'h':
#if DEBUG >= 1
						printf("[checkForOptions()] html == true\n");
#endif																				
						arguments.html = true;
						break;
					case 'i':
#if DEBUG >= 1
						printf("[checkForOptions()] processStdin == true\n");
#endif												
						arguments.processStdin = true;
						break;
					case 'o':
#if DEBUG >= 1
						printf("[checkForOptions()] outputStdout == true\n");
#endif																				
						arguments.outputStdout = true;
						break;
					case 'q':
#if DEBUG >= 1
						printf("[checkForOptions()] quiet == true\n");
#endif																				
						
						arguments.quiet = true;
						break;
					default:
						printf("Invalid argument found: %c\n", argv[a][b]);
						break;
				}
			}
#if DEBUG >= 2
			printf("Argc Before-Decrement:%i\n", *argc);
#endif
			(*argc)--;
#if DEBUG >= 2
			printf("Argc After Decrement:%i\n", *argc);
#endif			
		} else {//record the indexes of the non options
			if(pathIndexes[0] == -1) {//if the first slot isn't taken
				pathIndexes[0] = a;
			} else if(pathIndexes[1] == -1) {//if the second slot wasn't taken
				pathIndexes[1] = a;
			} else {
				printf("More than two non-option arguments were entered.\n");
			}
		}
	}
	
	//swap the indexes around so the paths are always in the begginging
	//the parser will the find the right most non-option first
	//thus if we find two non-options (two paths that is) we must place the first one we found second, since that will be the output file
	//and the second one we found first since that is out inputfile
	if(pathIndexes[0] != -1) {
		swapStrings(&argv[pathIndexes[1] != -1?2:1], &argv[pathIndexes[0]]); 
	} else if(pathIndexes[1] != -1) {
		swapStrings(&argv[1], &argv[pathIndexes[1]]);
	}
	
	return arguments;
}
