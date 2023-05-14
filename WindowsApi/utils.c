#include <stdio.h>
#include "utils.h"

char* my_gets_s(char* str, size_t size)
{
	//int ch;
	//size_t i;

	//for (i = 0; (ch = getchar()) != '\n' && ch != EOF; ++i)
	//	str[i] = ch;

	//if (i == 0 && ch == EOF)
	//	return NULL;

	//str[i] = '\0';

	//return str;

	char* s;

	if ((fgets(str, size, stdin)) == NULL)
	{
		return NULL;
	}
	if ((s = strchr(str, '\n')) != NULL) // eliminate \n 
	{
		*s = '\0';
	}

	return str;
}

void clean_stdin_buff(void)
{
	int ch;

	while ((ch = getchar()) != '\n' && ch != EOF)
		;
}