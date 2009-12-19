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

char *subString(const char target[], char to[], int begin, int end);
int indexOf(const char target[], const char searchString[]);
int lastIndexOf(const char target[], const char searchString[]);
char *flushString(char *string, int len);
int css_strcmp(const char[], const char[]);
char *flushStringWithString(char *string, int len, const char *cpyStr);
void swapStrings(char **, char **);
