#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
	FILE * fp;
	fp = fopen("input.txt", "w+");
	int counter = 0;
	int type = 0;

	fprintf(fp, "%d %d %d", counter, type, 2);
	counter++;
	type = 1;

	while(counter<=20000)
	{
		fprintf(fp, "\n%d %d %d", counter, type, 2);
		counter ++;
		if (type == 0)
			type = 1;
		else if (type == 1)
			type = 0;
	}

	return 0;
}