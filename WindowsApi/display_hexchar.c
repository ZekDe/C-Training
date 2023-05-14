#include "display_hexchar.h"
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

void DisplayHexchar(const char* buf, size_t size, size_t line_num)
{
	size_t i, j, remainder;
	size_t idx_buf;
	bool o_newline;

	for (i = 0; i < size; ++i)
	{

		if (i % line_num == 0)
		{
			printf("%08X ", i);
		}

		printf("%02X ", buf[i]);

		o_newline = (i % line_num) == (line_num - 1);

		if (o_newline)
		{
			for (j = 1; j < line_num; ++j)
			{
				idx_buf = i - line_num + j;
				printf("%c", iscntrl(buf[idx_buf]) ? '.' : buf[idx_buf]);
			}
			putchar('\n');
		}
	}

	remainder = size % line_num;

	for (j = 0; j < 3 * (line_num - remainder); ++j)
	{
		putchar(' ');
	}
		
	for (j = 0; j < remainder; ++j)
	{
		printf("%c", iscntrl(buf[i - remainder + j]) ? '.' : buf[i - remainder + j]);
	}
	putchar('\n');
}
