
#include "bsp/board.h"

void* memfind(void *mem, size_t mem_size, void *pattern, size_t pattern_size)
{
	uint32_t i,j;
	uint8_t *b=mem;
	uint8_t *p=pattern;
	uint8_t f=0;

	for(i=0;i<mem_size;i++)
	{
		if(b[i]==p[0])
		{
			f=0;
			for(j=0;j<pattern_size;j++)
			{
				if(b[i+j]!=p[j])
				{
					f=1;
					break;
				}
			}
			if(f==0) return mem+i;
		}
	}
	return NULL;
}
