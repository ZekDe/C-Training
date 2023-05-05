#include <stdio.h>

char* my_gets_s(char* str, size_t size)
{
	int ch;
	size_t i;

	for (i = 0; (ch = getchar()) != '\n' && ch != EOF; ++i)
		str[i] = ch;

	if (i == 0 && ch == EOF)
		return NULL;

	str[i] = '\0';

	return str;
}