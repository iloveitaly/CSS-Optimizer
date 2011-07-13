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

#import "constants.h"

#import <sys/param.h> /* MAXPATHLEN, linux & os x compatible */
#import <limits.h>
#import <stdbool.h>

#define BOOL bool

typedef struct {
	const char *props[MAXPROPLENGTH];
	int propLength;
	char propString[MAXPROPLENGTH][PROPBUFFSIZE];
	char finalString[FINALSTRINGSIZE]; //the shorthand name
	BOOL required[MAXPROPLENGTH];
	BOOL isBeingUsed;
} longHandPropertySet;

typedef struct {
	//bools
	BOOL hasFileExtension;
	BOOL hasFileName;
	BOOL isDir;
	//chars and strings
	char extension[NAME_MAX];
	char fileName[NAME_MAX];
	char filePath[MAXPATHLEN];
	char originalString[MAXPATHLEN+NAME_MAX];
} filePathString;

//option struct for command line arguments
typedef struct _commandLineArguments {
	BOOL recurseDir;
	BOOL sameFile;
	BOOL longToShort;
	BOOL noBackup;
	BOOL longToShortOnly;
	BOOL html;
	BOOL processStdin;
	BOOL outputStdout;
	BOOL quiet;
} commandLineArguments;

typedef struct _fileLink {
	char *fileName;
	int type;
	struct _fileLink *nextFile;
} fileLink;
