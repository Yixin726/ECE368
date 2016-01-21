#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void sort(int list[], int n);

int * makeArray(char * argv[]) //make array from input file - testing purposes
{
	int count;
	int * list = malloc(sizeof(int) * 1000000);
	char temp[1000];
	char * filename = argv[1];
 	FILE * input = fopen(filename, "r");

 	for (count = 0; fgets(temp, 100, input) != NULL; ++count)
 	{
 		list[count] = atoi(temp);
 	}

 	return (list);
}

void printArray(int * Array, int n) //print array to output file - testing purposes
{
	int counter;
	FILE * output = fopen("prj2_output.txt", "w+");

	for (counter = 0; counter < n; counter ++)
	{
		fprintf(output, "%d\n", Array[counter]);
	}

	fclose(output);
}

int main(int argc, char * argv[])
{
	int * list = makeArray(argv);

	int n = atoi(argv[2]);

	clock_t start_t, end_t;

	start_t = clock();

	sort(list, n);

	end_t = clock();	

	double diff = end_t - start_t;

	printf("Simulation took %f seconds to execute \n", diff/CLOCKS_PER_SEC);

	int counter;
	int sorted;
	for (counter = 0; counter < (n-1); counter ++)
	{
		if (list[counter] <= list[counter+1])
			sorted = 1;
		else
		{
			sorted = 0;
			break;
		}
	}
	if (sorted == 1)
		printf("sorted correctly\n");
	else
		printf("did not sort correctly\n");

	printArray(list, n);

	return 0;
}