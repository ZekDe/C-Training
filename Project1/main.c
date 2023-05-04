#include <stdio.h>
#include <string.h>
#include "walkdir.h"
#include "stdint.h"
#include "display_hexchar.h"

#define IS_FILE_ATTR()	(wfd->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "<DIR>" : ""

void ErrorHandler(void);

uint8_t o_fopen;

BOOL callback_walkdir(const WIN32_FIND_DATA *wfd, int level)
{ 
	printf("%*s%s %s\n", level * 4, "", wfd->cFileName, IS_FILE_ATTR());
	if (!_strcmpi(wfd->cFileName, "MD5"))
	{
		return FALSE;
	}
	return TRUE;
}

int main(int argc, char *argv[])
{
	/*if (argc != 2)
	{
		fprintf(stderr, "enter 2 argument..\n");
			exit(EXIT_FAILURE);
	}
	WalkDir(argv[1], callback_walkdir); */

	FILE* f;
	char buf[BUFSIZ];
	int ch;
	int i;


	o_fopen = ((f = fopen("test.txt", "r+")) == NULL);
	ErrorHandler();
	
	setbuf(f, buf);

	ch = fgetc(f);

	display_hexchar(buf, BUFSIZ, 16);

	fclose(f);

	return 0;
}

void ErrorHandler(void)
{
	o_fopen && (fprintf(stderr, "error..."), exit(EXIT_FAILURE), 0);
}





