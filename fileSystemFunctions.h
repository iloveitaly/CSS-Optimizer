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
#import "dataStructures.h"
#import <stdio.h>

int fileExists(const char name[]);
int hasExtension(const char *path);
int hasFileName(const char *path);
int isDir(const char *path);

char *getParentDirectorieFromFilePath(const char [], char *);
char *getFileExtension(const char target[], char to[]);
char *getFileName(const char filePath[], char to[], BOOL withFileExtension);
long getEOF(FILE *file);
int copy(const char *from, const char *to);

filePathString initPathStructWithPath(const char *path);
filePathString *initBOOLSForFilePath(filePathString *filePath);
filePathString *reInitOriginalString(filePathString *filePath);
