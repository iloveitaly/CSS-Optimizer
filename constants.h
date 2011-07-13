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

#import <stdbool.h>

//debug flag
#define DEBUG 0

#define ARG_ERR(msg) \
printf(msg"\n\n"); \
printUsage(); \
exit(EX_USAGE);

//various hack strings
#define ESCAPED_IE_HACK "\"\\\"}\\\"\""
#define ESCAPED_IE_HACK_LEN 7
#define COMMENT_HACK "/*/*/"

#define COUNT(x) (sizeof(x)/sizeof(x[0])) /* c.h has sizeofA() on os x, but linux doesn't seem to have that header file... */
 
//input buffer size for input gathering with -i
#define INPUT_BUFF_SIZE 100

 //constants for css propertys
#define FINALSTRINGSIZE 50
#define PROPBUFFSIZE 100
#define MAXPROPLENGTH 10

#define MAXFILEPATH MAXPATHLEN+NAME_MAX

#define CSSFILETYPE 5
#define HTMLFILETYPE 10
